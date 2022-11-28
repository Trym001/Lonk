import os
import sys
import time
import qwiic
import multiprocessing as mp
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../')))

from sphero_sdk import SpheroRvrObserver, DriveFlagsBitmask
from re_addressing_read_sensor import distSens

rvr = SpheroRvrObserver()

def initRVR():
    try:
        rvr.wake()
        # Give RVR time to wake up
        time.sleep(2)
        rvr.reset_yaw()
    except KeyboardInterrupt:
        print('\nProgram terminated with keyboard interrupt.')
    print("RVR init done")

def drive(s, h):
    rvr.drive_with_heading(
            speed=s,  # Valid speed values are 0-255
            heading=h,  # Valid heading values are 0-359
            flags=DriveFlagsBitmask.none.value
        )
    time.sleep(2)


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
        front, left, right = sens.readSens(dev1, dev2, dev3)
        print(front, left, right)
        if front < 200:
            print("Forward!")
            drive(20, 0)
        if left < 200:
            print("Left triggered")
            drive(0, 0)
        time.sleep(0.001)

    #drive(20, 0)
