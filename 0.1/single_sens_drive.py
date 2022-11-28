
import os, sys, time
import qwiic

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

print("Available device addresses: ")
print("Hex: ", [hex(x) for x in avail_addresses])
print("Dec: ", [int(x) for x in avail_addresses])

# Display Mux Configuration
print("Mux Configuration:")
print("-------------------")
mux.list_channels()

# Enable Channel
try:
    mux.enable_channels(int(2))
except Exception as e:
    print(e)

# Scans I2C addresses
avail_addresses = qwiic.scan()

print("Available device addresses: ")
print("Hex: ", [hex(x) for x in avail_addresses])
print("Dec: ", [int(x) for x in avail_addresses])

# Display Mux Configuration
print("Mux Configuration:")
print("-------------------")
mux.list_channels()

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

while True:
    try:
        # Start Measurements
        ToF.start_ranging()
        time.sleep(.005)

        # Take Measurements
        distance = ToF.get_distance()
        time.sleep(.005)

        # Stop Measurements
        ToF.stop_ranging()

        print("Distance(mm): %s" % distance)

    except Exception as e:
        print(e)




