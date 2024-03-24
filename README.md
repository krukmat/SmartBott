# SmartBott

This project utilizes a Seeed Studio XIAO board based on the 32bit Risc ESP32C chipset, along with a VL53L0X sensor, to create a smart bottle counter. The code provided in `main.ino` implements the functionality of the smart bottle counter.

## Prototype
![Alt text](https://github.com/krukmat/SmartBott/blob/bd77f27ea00c005a26e891ed0e93af6fd5c0e222/images/1.jpg "First prototype")
![Alt text](https://github.com/krukmat/SmartBott/blob/bd77f27ea00c005a26e891ed0e93af6fd5c0e222/images/2.jpg "Sensor")

## Mobile app - Android
![Alt text](https://github.com/krukmat/SmartBott/blob/59062e45b8b63f9ca055d93f5d3271169c726023/images/android-1.png "Home")

## Mobile app - IOS


## Prerequisites

To run this code, you will need the following components:

- Seeed Studio XIAO board (based on the nRF52840 or ESP32C chipset)
- VL53L0X sensor
- ArduinoBLE library
- VL53L0X library

Make sure the required libraries are installed before uploading the code to your Seeed Studio XIAO board.

## Setup

1. Connect the VL53L0X sensor to the Seeed Studio XIAO board according to the pin configuration in the code.
2. Connect the Seeed Studio XIAO board to your computer via USB.
3. Open the `main.ino` file in the Arduino IDE or your preferred code editor.
4. Upload the code to the Seeed Studio XIAO board.

Arduino Code (ESP32-C3)
The Arduino code reads data from a VL53L0X distance sensor to measure the liquid level in a water bottle. Below is a high-level explanation of the code:

Overview
The code utilizes the ArduinoBLE library to establish Bluetooth Low Energy (BLE) communication and send water consumption data to mobile devices.

## Main Components: Arduino
Initialization: Configures the BLE module and the VL53L0X distance sensor.
BLE Service and Characteristic: Defines a BLE service for sending consumption data and a characteristic for notifying and writing this data.
BLE Write Handling: Manages writes of data from the mobile device and updates the bottle counter and bottle type based on the received information.
Capacity Function: Calculates the volume of liquid in the bottle using the height measured by the distance sensor.
Main Loop: Continuously reads the distance measured by the sensor, updates the bottle counter, and transmits the data via BLE.
## React Native Code
The mobile application developed in React Native is used to visualize and control water consumption measured by the ESP32-C3 device. Below is a high-level explanation of the code:

### Overview
The mobile application scans and connects to available BLE devices to receive water consumption data. It allows the user to select a device, view the current water level, and control different display modes.

### Main Components
BLE Device Scanning: Scans nearby BLE devices and displays a list of available devices.
Connection and Data Reading: Connects to the selected device and reads water consumption data.
Local Data Storage: Uses AsyncStorage to store consumption data locally on the mobile device.
Display Mode Selection: Allows the user to select between different data display modes, such as liters, milliliters, etc.
Visualization Charts: Utilizes the GaugeChart library to graphically display the current water level.
## Troubleshooting

If you encounter any issues while running the smart bottle counter, consider the following:

- Make sure the Seeed Studio XIAO board is properly connected to the computer and the sensor is correctly wired.
- Verify that the required libraries (`ArduinoBLE` and `VL53L0X`) are installed.
- Double-check the pin configurations in the code to ensure they match your hardware setup.

If the problem persists, consult the documentation for the Seeed Studio XIAO board, sensor, and libraries for further assistance.

## License

This project is licensed under the [MIT License](LICENSE). Feel free to modify and use it according to your needs.

## Acknowledgments

- This project utilizes the [ArduinoBLE](https://www.arduino.cc/en/Reference/ArduinoBLE) library for Bluetooth Low Energy communication.
- The VL53L0X sensor functionality is made possible by the [VL53L0X library](https://github.com/pololu/vl53l0x-arduino).
- The hardware for this project is based on the Seeed Studio XIAO board, which is built around the nRF52840 chipset. The Seeed Studio XIAO is a compact and powerful development board that supports various features, including BLE, making it an ideal choice for this smart bottle counter project.
