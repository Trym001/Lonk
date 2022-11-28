import qwiic
import os, sys, time


class distSens:
    def __init__(self):
        # Connect to Mux
        self.mux = qwiic.QwiicTCA9548A()
        # Initially Disable All Channels
        self.mux.disable_all()

    def initSens(self):
        device1_address = self.change_address(current_channel=3, old_address=41, new_address=85)
        device2_address = 41
        device3_address = self.change_address(current_channel=7, old_address=41, new_address=10)

        for en_ch in [2, 3, 7]:
            self.enable_mux(en_ch)
            if en_ch == 2:
                self.initialize_address(41, do_return=False)
        return device1_address, device2_address, device3_address

    def change_address(self, current_channel: int, old_address: int, new_address: int):
        print(f"Running VL53L1X I2C Address Change.\nChanging old address: {old_address} to "
              f"new address: {new_address}.\nChannel to enable: {current_channel}")

        # Scans I2C addresses
        avail_addresses = qwiic.scan()

        # Check I2C addresses for Pi Servo pHat
        while 0x40 in avail_addresses:
            if 0x70 in avail_addresses:
                pca = qwiic.QwiicPCA9685()
                pca.set_addr_bit(0, 0)

                avail_addresses = qwiic.scan()

            # Remove Pi Servo pHat from avail_address list
            try:
                avail_addresses.remove(0x40)
            except ValueError:
                print("Addresses for Pi Servo pHat (0x40) not in scanned addresses.")

        while 0x29 not in avail_addresses:
            print("VL53L1X ToF sensor not detected on I2C bus at old address %s. or %s" % (41, hex(41)))

            self.enable_mux(current_channel)
            avail_addresses = qwiic.scan()
            print(avail_addresses)
            if not {new_address}.difference(avail_addresses):   # if sensor has correct address, function don't change it.
                self.initialize_address(new_address, do_return=False)
                return new_address

        ToF = self.initialize_address(old_address)
        try:  # changing sensor address
            ToF.set_i2c_address(new_address)
        except Exception as e:
            if e == OSError or e == IOError:
                print("Issue connecting to device.")
                print(e)
            else:
                print(e)
        avail_addresses = qwiic.scan()
        if new_address in avail_addresses:
            print(f"Address change to new address ('dec: {new_address}', 'hex: {hex(new_address)}') is complete."
                  f"\n\n")

        self.mux.disable_all()
        return new_address

    def initialize_address(self, address: int, do_return=True):
        print("Initializing Device")
        ToF = qwiic.QwiicVL53L1X(address)
        ToF.sensor_init()
        if do_return:
            return ToF
        else:
            return

    def enable_mux(self, en_ch: int):
        #print("=== Mux channels before enabling ===")
        #self.mux.list_channels()
        try:
            self.mux.enable_channels(en_ch)
        except Exception as e:
            print(e)
        #print("=== Mux channels after enabling ===")
        #self.mux.list_channels()
        return

    def readSens(self, device1_address, device2_address, device3_address):
        ToF_left = qwiic.QwiicVL53L1X(device1_address)
        ToF_front = qwiic.QwiicVL53L1X(device2_address)
        ToF_right = qwiic.QwiicVL53L1X(device3_address)

        try:
            # Start Measurements
            ToF_front.start_ranging()
            ToF_left.start_ranging()
            ToF_right.start_ranging()
            time.sleep(.005)

            # Take Measurements
            fwd_distance = ToF_front.get_distance()
            left_distance = ToF_left.get_distance()
            right_distance = ToF_right.get_distance()
            time.sleep(.005)

            # Stop Measurements
            ToF_front.stop_ranging()
            ToF_left.stop_ranging()
            ToF_right.stop_ranging()
            time.sleep(.005)

            return fwd_distance, left_distance, right_distance
        except Exception as e:
            print(e)


if __name__ == '__main__':
    distance = distSens()
    left, front, right = distance.initSens()

    distance.readSens(left, front, right, q=None)
