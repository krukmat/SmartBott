# SmartBott

This project utilizes an Arduino board with Bluetooth Low Energy (BLE) capabilities and a VL53L0X sensor to create a smart bottle counter. The code provided in `main.ino` implements the functionality of the smart bottle counter.

## Prerequisites

To run this code, you will need the following components:

- Arduino board (with BLE support)
- VL53L0X sensor
- ArduinoBLE library
- VL53L0X library

Make sure the required libraries are installed before uploading the code to your Arduino board.

## Setup

1. Connect the VL53L0X sensor to the Arduino board according to the pin configuration in the code.
2. Connect the Arduino board to your computer via USB.
3. Open the `main.ino` file in the Arduino IDE or your preferred code editor.
4. Upload the code to the Arduino board.

## Functionality

The smart bottle counter utilizes Bluetooth Low Energy (BLE) to transmit the bottle count to a connected device. It uses the VL53L0X sensor to measure the distance to the liquid level in a bottle and determine the bottle's fullness.

The code sets up a BLE service and characteristic to enable communication with connected devices. It defines different bottle types (1.5 liters and 0.5 liters) and corresponding sensor thresholds for different levels of fullness.

The main functionality is implemented in the `loop()` function, which performs the following steps:

1. Reads the sensor data to obtain the distance to the liquid level in the bottle.
2. Updates the bottle count based on the current sensor reading using the `updateBottleCount()` function.
3. If a device is connected via BLE, transmits the bottle count using the `transmitBottleCount()` function.

The `updateBottleCount()` function determines the current state of the bottle (FULL, HALF, or EMPTY) based on the sensor reading and updates the bottle count accordingly. It keeps track of the previous measure to detect transitions from HALF to EMPTY or FULL to EMPTY and increments the bottle count accordingly.

The `transmitBottleCount()` function sends the current bottle count over BLE to the connected device using the `bottleCharacteristic`.

## Customization

You can customize the behavior of the smart bottle counter by modifying the following parameters:

- `BottleType`: Enumerates the different bottle types (`LITRE_1_5` and `LITRE_0_5`). Adjust these according to your specific bottle sizes.
- `THRESHOLD_FULL`: Defines the sensor reading threshold for a bottle to be considered full.
- `THRESHOLD_HALF_1_5` and `THRESHOLD_HALF_0_5`: Define the sensor reading thresholds for a bottle to be considered half-full for each bottle type.
- `THRESHOLD_EMPTY_1_5` and `THRESHOLD_EMPTY_0_5`: Define the sensor reading thresholds for a bottle to be considered empty for each bottle type.

You can adjust these values based on your sensor's calibration and the desired level of accuracy for detecting bottle fullness.

## Troubleshooting

If you encounter any issues while running the smart bottle counter, consider the following:

- Make sure the Arduino board is properly connected to the computer and the sensor is correctly wired.
- Verify that the required libraries (`ArduinoBLE` and `VL53L0X`) are installed.
- Double-check the pin configurations in the code to ensure they match your hardware setup.

If the problem persists, consult the documentation for the Arduino board, sensor, and libraries for further assistance.

## License

This project is licensed under the [MIT License](LICENSE). Feel free to modify and use it according to your needs.

## Acknowledgments

- This project utilizes the [ArduinoBLE](https://www.arduino.cc/en/Reference/ArduinoBLE) library for Bluetooth Low Energy communication.
- The VL53L0X sensor functionality is made possible by the [VL53L0X library](https://github.com/pololu/vl53l0x-arduino).
