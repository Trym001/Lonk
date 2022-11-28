import asyncio
import qwiic
import os
import sys
import time
import multiprocessing as mp
from sphero_sdk import SpheroRvrAsync, SerialAsyncDal, RawMotorModesEnum, DriveFlagsBitmask
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
    await rvr.wake()
    await asyncio.sleep(2)
    await rvr.reset_yaw()
    # Release task
    await asyncio.sleep(0)
    print("RVR init done")

async def drive():
    # Drive forwards
    await rvr.drive_with_heading(
        speed=128,
        heading=0,
        flags=DriveFlagsBitmask.none.value
    )

    # Release task
    await asyncio.sleep(0)
    # await rvr.close()

async def turn180():

    await rvr.drive_with_heading(
        speed=0,
        heading=180,
        flags=DriveFlagsBitmask.none.value
    )
    await asyncio.sleep(1)

async def exitCode():
    p.join()
    # p2.join()
    #if loop.is_running():
    #    loop.close()
    await asyncio.sleep(0)
    await rvr.close()
    await asyncio.sleep(0)

if __name__ == '__main__':
    loop.run_until_complete(initRVR())
    sens = distSens()
    dev1, dev2, dev3 = sens.initSens()
    q = mp.Queue()
    p = mp.Process(target=sens.readSens, args=(dev1, dev2, dev3, q, ))
    # p2 = mp.Process(target=printSens, args=(q, ))
    p.start()
    # p2.start()

    while True:
        # forward, left, right
        while int(q.get()['forward']) > 100:
            print("just chillin")

        print("Wææh this is my nono zone!, shutting down! <:c")

        if loop.is_running():
            loop.close()

        loop.run_until_complete(turn180())



