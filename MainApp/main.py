#
# Arno-Laser MainApp
# Main.py created on 2019-03-15 by Arnaud Thiercelin
#

from tkinter import *
from i2c import *

class MainController:
    def __init__(self, mainWindow):
        self.mainWindow = mainWindow
        mainWindow.title("Arno Laser")

        # Full Screening
        pad = 3
        self.defaultGeometry = mainWindow.geometry
        # mainWindow.geometry("{0}x{1}+0+0".format(
        #     mainWindow.winfo_screenwidth() - pad, mainWindow.winfo_screenheight() - pad))
        # mainWindow.bind('<Escape>', self.toggleFullscreen)

        self.version = "0.1"
        self.tempUnit = "C"
        self.waterFlow = "TBD"
        #
        # States
        #
        self.CNCI2CStatus = "TBD"
        self.MCUI2CStatus = "TBD"
        self.isWaterPumpOn = False
        self.isAirPumpOn = False
        self.areFansOn = False
   
        self.initUI()

        self.i2cController = I2CController()

    def initUI(self):
        # Headline
        self.headlineLabel = Label(self.mainWindow, text="Arno Laser "+self.version)
        self.headlineLabel.grid(column=0, row=0, columnspan=4)

        # Fans
        self.fansSwitchLabel = Label(self.mainWindow, text="Fans:")
        self.fansSwitchLabel.grid(column=0, row=1)
        self.fansSwitch = Button(self.mainWindow, text="On/Off", command=self.switchFan)
        self.fansSwitch.grid(column=1, row=1)

        self.fansSpeedLabel = Label(self.mainWindow, text="Speed: ")
        self.fansSpeedLabel.grid(column=0, row=2)
        self.fansSpeedValueLabel = Label(self.mainWindow, text="TBD")
        self.fansSpeedValueLabel.grid(column=1, row=2)

        # Water
        self.waterPumpSwitchLabel = Label(self.mainWindow, text="Water Pump:")
        self.waterPumpSwitchLabel.grid(column=2, row=1)
        self.waterPumpSwitch = Button(self.mainWindow, text="On/Off", command=self.switchWaterPump)
        self.waterPumpSwitch.grid(column=3, row=1)

        self.waterFlowLabel = Label(self.mainWindow, text="Water Flow: ")
        self.waterFlowLabel.grid(column=2, row=2)
        self.waterFlowValueLabel = Label(self.mainWindow, text=self.waterFlow)
        self.waterFlowValueLabel.grid(column=3, row=2)

        self.waterTempLabel = Label(self.mainWindow, text="Water Temp: ")
        self.waterTempLabel.grid(column=2, row=3)
        self.waterTempValueLabel = Label(self.mainWindow, text="123"+self.tempUnit)
        self.waterTempValueLabel.grid(column=3, row=3)

        # Air Pump
        self.airPumpSwitchLabel = Label(self.mainWindow, text="Air Pump:")
        self.airPumpSwitchLabel.grid(column=0, row=3)
        self.airPumpSwitch = Button(self.mainWindow, text="Air Pump:", command=self.switchAirPump)
        self.airPumpSwitch.grid(column=1, row=3)

        # MCU + CNC Statuses
        self.cncI2CStatusLabel = Label(self.mainWindow, text="CNC: ")
        self.cncI2CStatusLabel.grid(column=0, row=4)
        self.cncI2CStatusValueLabel = Label(self.mainWindow, text=self.CNCI2CStatus)
        self.cncI2CStatusValueLabel.grid(column=1, row=4)

        self.mcuI2CStatusLabel = Label(self.mainWindow, text="MCU: ")
        self.mcuI2CStatusLabel.grid(column=0, row=5)
        self.mcuI2CStatusValueLabel = Label(self.mainWindow, text=self.MCUI2CStatus)
        self.mcuI2CStatusValueLabel.grid(column=1, row=5)

#       self.close_button = Button(mainWindow, text="Close", command=mainWindow.quit)
#       self.close_button.pack()

#
# UI Controls Commands
#
    def toggleFullscreen(self, event):
        geom = self.mainWindow.winfo_geometry()  # type: Geometry
        print(geom, self.defaultGeometry)
        self.mainWindow.geometry(self.defaultGeometry)
        self.defaultGeometry = geom

    def switchFan(self):
        if self.areFansOn:
            #turn off
            self.sendToMCU(0x21, 0)
            self.areFansOn = False
        else:
            #turn on
            self.sendToMCU(0x20, 0)
            self.areFansOn = True
            
    def switchWaterPump(self):
        if self.isWaterPumpOn:
            #turn off
            self.sendToMCU(0x31, 0)
            self.isWaterPumpOn = False
        else:
            #turn on
            self.sendToMCU(0x30, 0)
            self.isWaterPumpOn = True

    def switchAirPump(self):
        if self.isAirPumpOn:
            #turn off
            self.sendToMCU(0x11, 0)
            self.isAirPumpOn = False
        else:
            #turn On
            self.sendToMCU(0x10, 0)
            self.isAirPumpOn = True



mainApp = Tk()
mainGui = MainController(mainApp)
mainApp.mainloop()
