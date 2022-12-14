import os
import sys
import threading
import time
import qwiic
import json
from multiprocessing import Process, Queue, Lock
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../')))
from sphero_sdk import SpheroRvrObserver, DriveFlagsBitmask, Colors, RvrLedGroups, RvrStreamingServices
from re_addressing_read_sensor import distSens
from py_tcp_server import TCPServer
from timer import timer
from Regulator import regulator

rvr = SpheroRvrObserver()

def initRVR():
    try:
        rvr.wake()
        # Give RVR time to wake up
        time.sleep(2)
        rvr.reset_yaw()
        time.sleep(0)
        rvr.set_all_leds(
            led_group=RvrLedGroups.all_lights.value,
            led_brightness_values=[color for _ in range(10) for color in Colors.off.value]
        )
    except KeyboardInterrupt:
        print('\nProgram terminated with keyboard interrupt.')
    print("RVR init done")

def drive(s, h):
    rvr.drive_with_heading(
            speed=s,  # Valid speed values are 0-255
            heading=h,  # Valid heading values are 0-359
            flags=DriveFlagsBitmask.none.value
        )
    time.sleep(0.01)

def drive2(q):
    data = q.get()
    rvr.drive_with_heading(
            speed=data,  # Valid speed values are 0-255
            heading=0,  # Valid heading values are 0-359
            flags=DriveFlagsBitmask.none.value
        )
    time.sleep(0.01)

def reverse(s, h):
    rvr.drive_with_heading(
            speed=s,  # Valid speed values are 0-255
            heading=h,  # Valid heading values are 0-359
            flags=DriveFlagsBitmask.drive_reverse.value
        )
    time.sleep(0.5)

def readACC():
    rvr.sensor_control.add_sensor_data_handler(
        service=RvrStreamingServices.imu,
        handler=imu_handler)

    rvr.sensor_control.add_sensor_data_handler(
        service=RvrStreamingServices.accelerometer,
        handler=accelerometer_handler)

    rvr.sensor_control.start(interval=250)
    time.sleep(0.1)

def imu_handler(imu_data):
    #print('IMU data response: ', imu_data)
    pass

def accelerometer_handler(accelerometer_data):
    #print('Accelerometer data response: ', accelerometer_data)
    pass

def LEDSON(r, g, b):
    rvr.set_all_leds(
            led_group=RvrLedGroups.all_lights.value,
            led_brightness_values=[color for _ in range(10) for color in [r, g, b]]
        )
    time.sleep(0)

def formatData(l, f, r):
    data = {"left": l,
            "front": f,
            "right": r}
    return data

def jsonSend(data):
    try:
        N1.send_message(server_msg=json.dumps(data))
    except Exception as e:
        print(e)

def send_data(dev1, dev2, dev3, q):
    while True:
        try:
            print("\nWaiting for N1 connection....")
            N1.listen()
            while True:
                sens.readSens(dev1, dev2, dev3, q)
                sensors = q.get()
                print("Sensors:", sensors, "type:", type(sensors))
                left = sensors[0]
                front = sensors[1]
                right = sensors[2]
                data = formatData(left, front, right)
                msg = N1.send_message(server_msg=json.dumps(data))
        except Exception as e:
            print(e)

def recv_data(q):
    while True:
        try:
            print("\nWaiting for N2 connection....")
            N2.listen()
            while True:
                try:
                    incoming_msg = json.loads(N2.get_message())
                    drive(incoming_msg["speed"], 0)
                    #q.put(incoming_msg["speed"])
                except Exception as e:
                    print("Recieve error er:", e)
                    break
        except Exception as e:
            print(e)

def drive_with_cmd(q):
    while True:
        try:
            instructions = q.get()
            drive(int(instructions), 0)
        except Exception as e:
            print("Error in drive_with_cmd:", e)
            break


if __name__ == '__main__':
    # initiate --------------------------------------------------
    t = timer()
    sens = distSens()
    dev1, dev2, dev3 = sens.initSens()
    initRVR()
    #N1 = TCPServer(ip_adress="10.25.47.143", port=9093)
    N2 = TCPServer(ip_adress="10.25.47.143", port=9092)
    q = Queue()
    q_recv = Queue()
    p = Process(target=recv_data, args=(q_recv, ))

    p.start()

