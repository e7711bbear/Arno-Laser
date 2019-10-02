from smbus2 import SMBus

class I2CUnavailableError(Exception):
    def __init__(self, arg1, arg2=None):
        self.arg1 = arg1
        self.arg2 = arg2
        super(I2CUnavailableError, self).__init__(arg1)

class I2CController:
    #
    # I2C
    #
    def __init__(self):
        try:
            self.i2cBus = SMBus(1)  # 0 = /dev/i2c-0 (port I2C0), 1 = /dev/i2c-1 (port I2C1)
        except:
            print("Error opening I2C bus")
            raise I2CUnavailableError("Error opening I2C bus")
            return
        self.MCUI2CAddress = 0x8
        self.CNCI2CAddress = 0x10

    def checkI2CStatus(self):
        self.i2cBus.read_byte_data(80, 0)
        # Write a single register
        # cmd = 0x32
        # value = 0x80
        # bus.write_byte_data(self.MCUI2CAddress, cmd, value)

        # Write an array of registers
        # values = [0xff, 0xff, 0xff, 0xff, 0xff, 0xff]
        # bus.write_i2c_block_data(self.MCUI2CAddress, cmd, values)

    def sendToMCU(self, cmdId, value):
        self.i2cBus.write_byte_data(self.MCUI2CAddress, cmdId, value)
        return

    def sendToCNC(self, cmdId, value):
        self.i2cBus.write_byte_data(self.CNCI2CAddress, cmdId, value)
        return

    def closeI2C(self):
        self.i2cBus.close()
