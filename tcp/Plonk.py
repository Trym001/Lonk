import os
import sys
import time
import qwiic
import json

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../')))
from sphero_sdk import SpheroRvrObserver, DriveFlagsBitmask, Colors, RvrLedGroups, RvrStreamingServices
from re_addressing_read_sensor import distSens
from py_tcp_server import TCPServer
from timer import timer
from Regulator import regulator
import threading
import picamera
import io
import struct
import socket

class Plonk:
    def __init__(self):
        self.rvr = SpheroRvrObserver()
        self.init_rvr()
        self.sens = distSens()
        self.devices = self.sens.initSens()
        self.speed = 0
        self.heading = 0
        self.reg_head = 0
        self.send_network = TCPServer(ip_adress="10.25.47.143", port=9093)
        self.recv_network = TCPServer(ip_adress="10.25.47.143", port=9094)
        self.camera_network = TCPServer(ip_adress="10.25.47.143", port=9092)
        self.reg = regulator()
        self._lock_sensors = threading.Lock()
        self._lock_speed = threading.Lock()
        self.leds_on(100, 0, 255)
        self._decision = None
        self._decision_prev = None
        self.front = 100
        self.left = 100
        self.right = 100
        self.imu_values = None
        self.yaw = 0

    def init_rvr(self):
        try:
            self.rvr.wake()
            # Give RVR time to wake up
            time.sleep(2)
            self.rvr.reset_yaw()
            time.sleep(0)
            self.rvr.set_all_leds(
                led_group=RvrLedGroups.all_lights.value,
                led_brightness_values=[color for _ in range(10) for color in Colors.off.value]
            )
        except KeyboardInterrupt:
            print('\nProgram terminated with keyboard interrupt.')
        print("RVR init done")

    def drive(self, s, h):
        h = self.heading % 360

        self.rvr.drive_with_heading(
            speed=s,  # Valid speed values are 0-255
            heading=h,  # Valid heading values are 0-359
            flags=DriveFlagsBitmask.none.value
        )
        time.sleep(0.1)

    def read_imu(self):
        self.rvr.sensor_control.add_sensor_data_handler(
            service=RvrStreamingServices.imu,
            handler=self.imu_handler)

        self.rvr.sensor_control.start(interval=100)
        time.sleep(0.01)

    def imu_handler(self, imu_data):
        imu = imu_data['IMU']
        if imu['is_valid']:
            self.yaw = round(imu['Yaw'])
            self.remap(self.yaw, -179, 179, 0, 359)
        else:
            pass

    def leds_on(self, r, g, b):
        self.rvr.set_all_leds(
            led_group=RvrLedGroups.all_lights.value,
            led_brightness_values=[color for _ in range(10) for color in [r, g, b]]
        )
        time.sleep(0.01)

    def remap(self, old_val, old_min, old_max, new_min, new_max):
        return (new_max - new_min) * (old_val - old_min) / (old_max - old_min) + new_min

    def format_data(self, l, f, r, s, h, y):
        data = {"sensors": {"left": l,
                            "front": f,
                            "right": r},

                "driving_data": {"speed": s,
                                 "heading": h},

                "imu": {"yaw": y}
                }
        return data

    def decision(self):
        try:
            if self._decision != self._decision_prev:
                if self._decision == "onwards":
                    self.speed = 30
                elif self._decision == "Left":
                    self.speed = 0
                    self.heading -= 90
                elif self._decision == "Right":
                    self.speed = 0
                    self.heading += 90
                elif self._decision == "Dead_end":
                    self.speed = 0
                    self.heading += 180
                else:
                    self.speed = 0
            self._decision_prev = self._decision
        except Exception as e:
            print("decision error:", e)

    def read_and_send(self):
        while True:
            try:
                print("\nWaiting for send_network connection....")
                self.send_network.listen()
                print("send_network connected!")
                count = 0
                while True:

                    with self._lock_sensors:
                        values = self.sens.readSens(self.devices[0],
                                                    self.devices[1],
                                                    self.devices[2])
                        self.read_imu()
                        self.left = values[1]
                        self.front = values[0]
                        self.right = values[2]

                        data = self.format_data(self.left, self.front, self.right,
                                                self.speed, self.heading, self.yaw)
                        #print(data)
                        self.send_network.send_message(server_msg=json.dumps(data))
                    count += 1
                    if count >= 20:
                        print("left:", self.left, "front:", self.front, "right:", self.front)
                        count = 0
                    time.sleep(0.02)
            except Exception as e:
                print("read_and_send:", e)
                break

    def recv(self):
        while True:
            try:
                print("\nWaiting for recv_network connection....")
                self.recv_network.listen()
                print("recv_network connected!")
                while True:
                    try:
                        incoming_msg = json.loads(self.recv_network.get_message())
                        if self._decision_prev != incoming_msg:
                            with self._lock_speed:
                                self._decision = incoming_msg
                                # self.speed = incoming_msg["speed"]
                                # self.heading = incoming_msg["heading"]
                                self.decision()
                            time.sleep(0.02)
                            print("incoming_msg:", incoming_msg, "heading:", self.heading)
                    except Exception as e:
                        print("recv error:", e)
                        break
            except Exception as e:
                print("Error with recv_network connection:", e)
                # ime.sleep(0.02)

    def driver(self):
        while True:
            try:
                if self.front > 70:
                    head_diff = abs(self.heading - self.reg_head)
                    while (self.left < 200 and self.right < 200) and (head_diff < 50):
                        with self._lock_speed:
                            self.heading = self.reg.pi_regulator(self.left,
                                                                 self.right,
                                                                 self.yaw)
                            self.reg_head = self.heading
                            self.leds_on(255, 255, 0)
                            self.drive(self.speed, self.heading)
                        time.sleep(0.02)
                        head_diff = abs(self.heading - self.reg_head)
                        # print("While:", "diff:", head_diff, "heading", self.heading,
                        #      "reg_head:", self.reg_head)
                    yaw_diff = abs(self.heading - self.yaw)
                    if yaw_diff >= 300 or yaw_diff <= 60:
                        self.reg_head = self.heading
                    self.leds_on(100, 0, 255)
                    self.drive(self.speed, self.heading)
                    # print("if:", "diff:", head_diff, "heading", self.heading,
                    #       "reg_head:", self.reg_head)

                else:
                    with self._lock_speed:
                        self.leds_on(255, 0, 0)
                        self.speed = 0
                        self.drive(self.speed, self.heading)
                    time.sleep(0.02)
            except Exception as e:
                print("Driver error:", e)
                break
            time.sleep(0.02)

    def camera_send(self):
        try:
            print("\nWaiting for recv_network connection....")
            self.camera_network.listen()
            print("recv_network connected!")
            #source: https://picamera.readthedocs.io/en/release-1.13/recipes2.html task: 4.9 Rapid capture and streaming
            with picamera.PiCamera() as camera:
                camera.resolution = (640, 480)
                camera.framerate = 30
                time.sleep(0.5)
                stream = io.BytesIO()
                # Use the video-port for captures...
                for foo in camera.capture_continuous(stream, 'jpeg',
                                                     use_video_port=True):
                    self.camera_network.send_vid(struct.pack('<L', stream.tell()))
                    stream.seek(0)
                    self.camera_network.send_vid(stream.read())
                    stream.seek(0)
                    stream.truncate()
                    time.sleep(0.0000001)



        except Exception as e:
            print(e)
            #N1.listen()
            #Send.connected

if __name__ == '__main__':
    plonk = Plonk()
    x = threading.Thread(target=plonk.read_and_send, daemon=True)
    x2 = threading.Thread(target=plonk.recv, daemon=True)
    x3 = threading.Thread(target=plonk.driver, daemon=False)
    x4 = threading.Thread(target=plonk.camera_send, daemon=True)
    threads = [x, x2, x3, x4]
    for t in threads:
        t.start()
