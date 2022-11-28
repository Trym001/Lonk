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

global heading
global speed
heading = 0
speed = 20

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

async def drive(speed, heading):
    # Drive forwards
    await rvr.drive_with_heading(
        speed=speed,
        heading=heading,
        flags=DriveFlagsBitmask.none.value
    )
    # Release task
    await asyncio.sleep(0)

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

def findCloseWall(left, right):
    if left > right:
        diff = left - right
        close_wall = "right"

    elif right > left:
        diff = right - left
        close_wall = "left"

    else:
        diff = 0
        close_wall = "none"

    return diff, close_wall

if __name__ == '__main__':
    loop.run_until_complete(initRVR())
    sens = distSens()
    dev1, dev2, dev3 = sens.initSens()
    q = mp.Queue()
    p = mp.Process(target=sens.readSens, args=(dev1, dev2, dev3, q, ))
    #p2 = mp.Process(target=printSens, args=(q, ))
    p.start()
    #p2.start()


    while True:
        # forward, left, right
        # left = q.get()['left']
        # front = q.get()['forward']
        # right = q.get()['right']
        sensorDic = q.get()
        front, left, right = sensorDic.items()
        front = front[1]
        left = left[1]
        right = right[1]



        while (int(front) > 100):
            speed = 30
            sensorDic = q.get()
            front, left, right = sensorDic.items()
            front = front[1]
            left = left[1]
            right = right[1]
            if (left < 150) or (right < 150):
                diff, wall = findCloseWall(left=left, right=right)
                if diff < 40:
                    heading = 0
                else:
                    if (wall == "left"):
                        heading += 1
                        heading %= 360

                    if (wall == "right"):
                        heading -= 1
                        heading %= 360
            loop.run_until_complete(drive(speed=speed, heading=heading))

        speed = 0
        loop.run_until_complete(drive(speed=speed, heading=heading))



