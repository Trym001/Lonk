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

    # forward, left, right
    while int(q.get()['forward']) > 500:
        loop.run_until_complete(drive())

    if loop.is_running():
        loop.close()

    loop.run_until_complete(turn180())

