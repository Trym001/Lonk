
import os, sys, time
import qwiic
import asyncio
from sphero_sdk import SpheroRvrAsync
from sphero_sdk import SerialAsyncDal
from sphero_sdk import RawMotorModesEnum
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../')))
distance = 600

# Needed for the robot events
loop = asyncio.get_event_loop()
rvr = SpheroRvrAsync(
    dal=SerialAsyncDal(
        loop
    )
)

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

async def main():
    """ This program has RVR drive around in different directions.
    """

    await rvr.wake()

    # Give RVR time to wake up
    await asyncio.sleep(2)

    await rvr.reset_yaw()

    await rvr.raw_motors(
        left_mode=RawMotorModesEnum.forward.value,
        left_speed=128,  # Valid speed values are 0-255
        right_mode=RawMotorModesEnum.forward.value,
        right_speed=128  # Valid speed values are 0-255
    )

    # Delay to allow RVR to drive
    await asyncio.sleep(1)

    await rvr.raw_motors(
        left_mode=RawMotorModesEnum.forward.value,
        left_speed=0,  # Valid speed values are 0-255
        right_mode=RawMotorModesEnum.forward.value,
        right_speed=0  # Valid speed values are 0-255
    )

    # Delay to allow RVR to drive
    await asyncio.sleep(1)


    await rvr.raw_motors(
        left_mode=RawMotorModesEnum.forward.value,
        left_speed=128,  # Valid speed values are 0-255
        right_mode=RawMotorModesEnum.reverse.value,
        right_speed=128  # Valid speed values are 0-255
    )



    # Delay to allow RVR to drive
    await asyncio.sleep(2)

    await rvr.close()

if __name__ == '__main__':

    while distance >= 200:
        # Start Measurements
        ToF.start_ranging()
        time.sleep(.005)

        # Take Measurements
        distance = ToF.get_distance()
        time.sleep(.005)

        # Stop Measurements
        ToF.stop_ranging()

        print("Distance(mm): %s" % distance)


    try:
        loop.run_until_complete(
            main()
        )

    except KeyboardInterrupt:
        print('\nProgram terminated with keyboard interrupt.')

        loop.run_until_complete(
            rvr.close()
        )

    finally:
        if loop.is_running():
            loop.close()




