from tkinter import Tk, Label, Button

class MainApp:
    def __init__(self, mainWindow):
        self.mainWindow = mainWindow
        mainWindow.title("Arno Laser Main Window")

# Full Screening
        pad = 3
        self.defaultGeometry = mainWindow.geometry
        # mainWindow.geometry("{0}x{1}+0+0".format(
        #     mainWindow.winfo_screenwidth() - pad, mainWindow.winfo_screenheight() - pad))
        # mainWindow.bind('<Escape>', self.toggleFullscreen)

        self.version = "0.1"
        self.tempUnit = "°C"
        self.waterFlow = "TBD"
        self.CNCI2CStatus = "TBD"
        self.MCUI2CStatus = "TBD"

# Headline
        self.headlineLabel = Label(mainWindow, text="Arno Laser "+self.version)
        self.headlineLabel.grid(column=0, row=0, columnspan=4)

# Fans
        self.fansSwitchLabel = Label(mainWindow, text="Fans:")
        self.fansSwitchLabel.grid(column=0, row=1)
        self.fansSwitch = Button(mainWindow, text="On/Off", command=self.switchFan)
        self.fansSwitch.grid(column=1, row=1)

        self.fansSpeedLabel = Label(mainWindow, text="Speed: ")
        self.fansSpeedLabel.grid(column=0, row=2)
        self.fansSpeedValueLabel = Label(mainWindow, text="TBD")
        self.fansSpeedValueLabel.grid(column=1, row=2)

# Water
        self.waterPumpSwitchLabel = Label(mainWindow, text="Water Pump:")
        self.waterPumpSwitchLabel.grid(column=2, row=1)
        self.waterPumpSwitch = Button(mainWindow, text="On/Off", command=self.switchWaterPump)
        self.waterPumpSwitch.grid(column=3, row=1)

        self.waterFlowLabel = Label(mainWindow, text="Water Flow: ")
        self.waterFlowLabel.grid(column=2, row=2)
        self.waterFlowValueLabel = Label(mainWindow, text=self.waterFlow)
        self.waterFlowValueLabel.grid(column=3, row=2)

        self.waterTempLabel = Label(mainWindow, text="Water Temp: ")
        self.waterTempLabel.grid(column=2, row=3)
        self.waterTempValueLabel = Label(mainWindow, text="123"+self.tempUnit)
        self.waterTempValueLabel.grid(column=3, row=3)

# Air Pump
        self.airPumpSwitchLabel = Label(mainWindow, text="Air Pump:")
        self.airPumpSwitchLabel.grid(column=0, row=3)
        self.airPumpSwitch = Button(mainWindow, text="Air Pump:", command=self.switchAirPump)
        self.airPumpSwitch.grid(column=1, row=3)

# MCU + CNC Statuses
        self.cncI2CStatusLabel = Label(mainWindow, text="CNC: ")
        self.cncI2CStatusLabel.grid(column=0, row=4)
        self.cncI2CStatusValueLabel = Label(mainWindow, text=self.CNCI2CStatus)
        self.cncI2CStatusValueLabel.grid(column=1, row=4)

        self.mcuI2CStatusLabel = Label(mainWindow, text="MCU: ")
        self.mcuI2CStatusLabel.grid(column=0, row=5)
        self.mcuI2CStatusValueLabel = Label(mainWindow, text=self.MCUI2CStatus)
        self.mcuI2CStatusValueLabel.grid(column=1, row=5)

#       self.close_button = Button(mainWindow, text="Close", command=mainWindow.quit)
#       self.close_button.pack()

    def toggleFullscreen(self, event):
        geom = self.mainWindow.winfo_geometry()
        print(geom, self.defaultGeometry)
        self.mainWindow.geometry(self.defaultGeometry)
        self.defaultGeometry = geom

    def switchFan(self):
        print("SwitchFan")

    def switchWaterPump(self):
        print("switchWaterPump")

    def switchAirPump(self):
        print("switchAirPump")

root = Tk()
my_gui = MainApp(root)
root.mainloop()