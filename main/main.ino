#include <ArduinoBLE.h>
#include <VL53L0X.h>

// Create a VL53L0X object
VL53L0X sensor;

// Define the BLE service and characteristic
BLEService bottleService("19B10001-E8F2-537E-4F6C-D104768A1214");
BLEUnsignedIntCharacteristic bottleCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);

// Define the bottle types and sensor thresholds
enum BottleType { LITRE_1_5, LITRE_0_5 };
BottleType bottleType = LITRE_1_5;
const int THRESHOLD_FULL = 60;
const int THRESHOLD_HALF_1_5 = 190;
const int THRESHOLD_HALF_0_5 = 130;
const int THRESHOLD_EMPTY_1_5 = 300;
const int THRESHOLD_EMPTY_0_5 = 240;

// Define the bottle count and previous measure
int bottleCount = 0;
enum Measure { EMPTY, HALF, FULL };
Measure previousMeasure = EMPTY;
const int ledPin = LED_BUILTIN; // pin to use for the LED

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // changed from LOW to HIGH     

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Hydro");
  BLE.setAdvertisedService(bottleService);

  // add the characteristic to the service
  bottleService.addCharacteristic(bottleCharacteristic);

  // add service
  BLE.addService(bottleService);

  // set the initial value for the characeristic:
  bottleCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  // Initialize the sensor
  sensor.init();
  sensor.setTimeout(500);

  Serial.println("Hydro reader");
}

void loop() {
  // Check the sensor reading and update the bottle count
  int reading = sensor.readRangeSingleMillimeters();
  updateBottleCount(reading);

  // Transmit the bottle count over BLE when a device is connected
  if (BLE.central()) {
    transmitBottleCount();
  }
}

void updateBottleCount(int reading) {
  // Update the bottle count based on the sensor reading
  Measure currentMeasure;
  if (reading <= THRESHOLD_FULL) {
    currentMeasure = FULL;
  } else if ((bottleType == LITRE_1_5 && reading >= THRESHOLD_HALF_1_5 && reading < THRESHOLD_EMPTY_1_5) || (bottleType == LITRE_0_5 && reading >= THRESHOLD_HALF_0_5 && reading < THRESHOLD_EMPTY_0_5)) {
    currentMeasure = HALF;
  } else {
    currentMeasure = EMPTY;
  }

  if ((previousMeasure == HALF && currentMeasure == EMPTY) || (previousMeasure == FULL && currentMeasure == EMPTY)) {
    bottleCount++;
  }

  previousMeasure = currentMeasure;
}

void transmitBottleCount() {
  // Transmit the bottle count over BLE
  bottleCharacteristic.writeValue(bottleCount);
  bottleCount = 0;
}
