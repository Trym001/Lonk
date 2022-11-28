import asyncio
import qwiic
import os
import sys
import time
import multiprocessing as mp
from sphero_sdk import SpheroRvrAsync, SerialAsyncDal, RawMotorModesEnum
from re_addressing_read_sensor import distSens
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../')))
loop = asyncio.get_event_loop()

rvr = SpheroRvrAsync(
    dal=SerialAsyncDal(
        loop
    )
)

def initSens():
    # Connect to Mux
    mux = qwiic.QwiicTCA9548A()
    # Initially Disable All Channels
    mux.disable_all()

    os.system('i2cdetect -y 1')

    # Scans I2C addresses
    avail_addresses = qwiic.scan()

    while 0x40 in avail_addresses:
        if 0x70 in avail_addresses:
            # Disable the General Call Address if it is (shared with Mux)
            pca = qwiic.QwiicPCA9685()
            pca.set_addr_bit(0, 0)

            # Re-scans I2C addresses
            avail_addresses = qwiic.scan()
        try:
            avail_addresses.remove(0x40)
        except ValueError:
            print("Addresses for Pi Servo pHat (0x40) not in scanned addresses.")

    # Enable Channel
    try:
        mux.enable_channels(int(2))
    except Exception as e:
        print(e)

    # Scans I2C addresses
    avail_addresses = qwiic.scan()

    print("Initializing Device")
    ToF = qwiic.QwiicVL53L1X(int(41))

    try:
        ToF.sensor_init()
        print("Forward sensor initialized.")

    except Exception as e:
        if e == OSError or e == IOError:
            print("Issue connecting to device.")
            print(e)
        else:
            print(e)
    return ToF

def readSens(q):
    while True:
        # Start Measurements
        ToF.start_ranging()
        time.sleep(.005)
        # Take Measurements
        distance = ToF.get_distance()
        time.sleep(.005)
        # Stop Measurements
        ToF.stop_ranging()
        # print("Distance(mm): %s" % distance)
        q.put(distance)

def printSens(q):
    while True:
        print(q.get())



async def initRVR():
    # Initial setup
    # execute the three line below, once, the very first time the robot starts
    await rvr.wake()
    await asyncio.sleep(2)
    await rvr.reset_yaw()
    # Give access to others and finish the function
    await asyncio.sleep(0)
    print("RVR init done")
    # await rvr.close()

async def drive():

    # Drive forwards
    await rvr.raw_motors(
        left_mode=RawMotorModesEnum.forward.value,
        left_speed=128,  # Valid speed values are 0-255
        right_mode=RawMotorModesEnum.forward.value,
        right_speed=128  # Valid speed values are 0-255
    )

    # Have to have this sleep in order to give access to other tasks.
    # When value is 0, it doesnt slow down the program
    await asyncio.sleep(0)
    # await rvr.close()


async def turn180():

    # stop
    # await asyncio.sleep(0)
    await rvr.raw_motors(
        left_mode=RawMotorModesEnum.forward.value,
        left_speed=0,  # Valid speed values are 0-255
        right_mode=RawMotorModesEnum.forward.value,
        right_speed=0  # Valid speed values are 0-255
    )

    # let the robot stand still for a second, then turn around
    await asyncio.sleep(1)
    await rvr.raw_motors(
        left_mode=RawMotorModesEnum.forward.value,
        left_speed=128,  # Valid speed values are 0-255
        right_mode=RawMotorModesEnum.reverse.value,
        right_speed=128  # Valid speed values are 0-255
    )

    # Give access to others and finish the function
    await asyncio.sleep(0)
    # await rvr.close()


if __name__ == '__main__':
    loop.run_until_complete(initRVR())
    sens = distSens()
    dev1, dev2, dev3 = sens.initSens()
    q = mp.Queue()
    p = mp.Process(target=sens.readSens, args=(dev1, dev2, dev3, q, ))
    p.start()

    while int(q.get()['forward']) > 500:
        loop.run_until_complete(drive())

    if loop.is_running():
        loop.close()

    loop.run_until_complete(turn180())

