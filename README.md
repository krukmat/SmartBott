# SmartTank - Smarter Hydration

This project utilizes a Seeed Studio XIAO board based on the 32bit Risc ESP32C chipset, along with a VL53L0X sensor, to create a smart bottle counter. The code provided in `main.ino` implements the functionality of the smart bottle counter.

## Prototype
![Alt text](https://github.com/krukmat/SmartBott/blob/891acec7f37bfd816daf892cae49eb40f58c4b02/images/4.jpg "First prototype")
![Alt text](https://github.com/krukmat/SmartBott/blob/891acec7f37bfd816daf892cae49eb40f58c4b02/images/5.jpg "Sensor")
![Alt text](https://github.com/krukmat/SmartBott/blob/891acec7f37bfd816daf892cae49eb40f58c4b02/images/7.jpg "Sensor")

## Salesforce - Report
![Alt text](https://github.com/krukmat/SmartBott/blob/891acec7f37bfd816daf892cae49eb40f58c4b02/images/salesforce-1.jpg "Home")



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

## Functionality

### Calculation of Water Volume
Measuring Distance: The ToF sensor provides distance measurements in millimeters, stored in the variable reading.

Calculating Liquid Height: Using the measured distance (reading), the code calculates the height of the liquid in the jug. This calculation involves subtracting the measured distance from the total jug height and applying any necessary adjustments for accuracy.

Calculating Liquid Volume: With the liquid height determined, the code employs the formula for the volume of a cylinder to calculate the volume of liquid present in the jug. This calculation considers the known radius of the jug's base and the measured height of the liquid.

### Bluetooth Communication
Once the water volume calculation is complete, the Arduino code utilizes Bluetooth Low Energy (BLE) communication to transmit this information to external devices. This is achieved through the ArduinoBLE library, which allows the Arduino device to advertise services and characteristics for data transmission

## Customization

You can customize the behavior of the smart bottle counter by modifying the following parameters:

// bottle definitions
`
const float radio = 3.5; // cms it's the recipient radius
const float altura_total = 20; // cms it's the total height on the recipient
const float ajuste = 5; // cms the adjustment needed when the recipient is irregular. 
const float capacidad_total = 0.75; // total capacity in litres
`

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
