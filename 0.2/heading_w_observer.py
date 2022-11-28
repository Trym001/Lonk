import os
import sys
import time
import qwiic
import multiprocessing as mp
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../')))

from sphero_sdk import SpheroRvrObserver, DriveFlagsBitmask, Colors, RvrLedGroups, RvrStreamingServices
from re_addressing_read_sensor import distSens

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


def main():
    """ This program has RVR drive around in different directions using the function drive_with_heading.
    """

    try:
        rvr.wake()

        # Give RVR time to wake up
        time.sleep(2)

        rvr.reset_yaw()

        rvr.drive_with_heading(
            speed=128,  # Valid speed values are 0-255
            heading=0,  # Valid heading values are 0-359
            flags=DriveFlagsBitmask.none.value
        )

        # Delay to allow RVR to drive
        time.sleep(1)

        rvr.drive_with_heading(
            speed=128,  # Valid speed values are 0-255
            heading=0,  # Valid heading values are 0-359
            flags=DriveFlagsBitmask.drive_reverse.value
        )

        # Delay to allow RVR to drive
        time.sleep(1)

        rvr.drive_with_heading(
            speed=128,  # Valid speed values are 0-255
            heading=90,  # Valid heading values are 0-359
            flags=DriveFlagsBitmask.none.value
        )

        # Delay to allow RVR to drive
        time.sleep(1)

        rvr.drive_with_heading(
            speed=128,  # Valid speed values are 0-255
            heading=270,  # Valid heading values are 0-359
            flags=DriveFlagsBitmask.none.value
        )

        # Delay to allow RVR to drive
        time.sleep(1)

        rvr.drive_with_heading(
            speed=0,  # Valid heading values are 0-359
            heading=0,  # Valid heading values are 0-359
            flags=DriveFlagsBitmask.none.value
        )

        # Delay to allow RVR to drive
        time.sleep(1)

    except KeyboardInterrupt:
        print('\nProgram terminated with keyboard interrupt.')

    finally:
        rvr.close()


if __name__ == '__main__':
    sens = distSens()
    dev1, dev2, dev3 = sens.initSens()
    initRVR()

    while True:
        try:
            front, left, right = sens.readSens(dev1, dev2, dev3)
            #print(front, left, right)
            if front > 200:
                #print("Just keep swimin! :)")
                drive(20, 0)
                LEDSON(100, 0, 255)
                readACC()
            else:
                #print("WALL!")
                drive(0,0)
                LEDSON(255, 0, 0)
            time.sleep(0.001)
        except KeyboardInterrupt:
            print('\nProgram terminated with keyboard interrupt.')
            LEDSON(100, 0, 255)
            rvr.close()
            break
