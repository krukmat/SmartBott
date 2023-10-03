#include <ArduinoBLE.h>
#include <VL53L0X.h>

// Create a VL53L0X object
VL53L0X sensor;

// Define the BLE service and characteristic
BLEService bottleService("19B10001-E8F2-537E-4F6C-D104768A1214");
BLEUnsignedIntCharacteristic bottleCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);

// Define the bottle types and sensor thresholds
enum BottleType { LITRE_1_5, LITRE_0_5, LITRE_1 };
BottleType bottleType = LITRE_0_5;
const int THRESHOLD_FULL = 60;
//const int THRESHOLD_HALF_1_5 = 190;
//const int THRESHOLD_HALF_0_5 = 130;
const int THRESHOLD_EMPTY_1_5 = 300;
const int THRESHOLD_EMPTY_1 = 270;
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

  // Read Handler
  // Set up the read handler for the bottleCharacteristic
  bottleCharacteristic.setEventHandler(BLEWritten, [](BLEDevice central, BLECharacteristic characteristic) {
    // This function will be called when the central device writes to the characteristic.
    // You can include your custom logic here to handle the written value.
    // For example, you might want to update the bottle type based on the received value.
    Serial.print("+++++++++++++++++++++++++++++++++++++++++++++++++");

    printData(characteristic.value(), characteristic.valueLength());
    
    uint8_t datas[characteristic.valueLength()];
  characteristic.readValue(datas, characteristic.valueLength());
    
    if (datas[0] == 215){
      int receivedValue = bottleCharacteristic.value(); // Retrieve the written value
      Serial.println("Reset the bottle count");
      bottleCount = 0;
    } else {
      Serial.println("Define capacity");      
      if (datas[0] == 223) {
        // 1 litre
        bottleType = LITRE_1;
        Serial.println("1 litre");
      } else if (datas[0] == 219) {
        // 0.5 litre
        Serial.println("0.5 litre");
        bottleType = LITRE_0_5;
      } else{
          bottleType = LITRE_1_5;
          Serial.println("1.5 litre");        
      }
    } 
   Serial.print("+++++++++++++++++++++++++++++++++++++++++++++++++");

    // Include your logic here to handle the receivedValue
  });

  Serial.println("Hydro reader");
}

void printData(const unsigned char data[], int length) {
  for (int i = 0; i < length; i++) {
    unsigned char b = data[i];
    //not sure what this if statement does, but I think it adds 0 to small hex values that would otherwise be printed without them
    if (b < 16) {
      Serial.print("0");
    }
    //Prints each b value individually, I want to store the whole thing to a variable
    Serial.print(b, HEX);
  }
}

void loop() {
  // Check the sensor reading and update the bottle count
  BLEDevice central = BLE.central();
  int reading = sensor.readRangeSingleMillimeters();
  updateBottleCount(reading);
  Serial.print("Reading: ");
  Serial.println(reading);

  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    digitalWrite(ledPin, HIGH); // changed from LOW to HIGH  
    if (central.connected() && bottleCount > 0) {
      transmitBottleCount();
      //central.disconnect();
    }
  }
  delay(1000);
}

void updateBottleCount(int reading) {
  // Update the bottle count based on the sensor reading
  Measure currentMeasure;
  if (reading <= THRESHOLD_FULL) {
    currentMeasure = FULL;
  } else if ( (bottleType == LITRE_1_5 && reading > THRESHOLD_FULL && reading < THRESHOLD_EMPTY_1_5) || 
              (bottleType == LITRE_0_5 && reading >= THRESHOLD_FULL && reading < THRESHOLD_EMPTY_0_5) ||  
              (bottleType == LITRE_1 && reading >= THRESHOLD_FULL && reading < THRESHOLD_EMPTY_1)) {
    currentMeasure = HALF;
  } else {
    currentMeasure = EMPTY;
  }
  //Serial.print("Current Measure: ");
  //Serial.println(currentMeasure);
  //Serial.print("Bottle count:");
  //Serial.println(bottleCount);
  if ((previousMeasure == HALF && currentMeasure == EMPTY) || (previousMeasure == FULL && currentMeasure == EMPTY)) {
    bottleCount++;
    //bottleCount = 80;
    Serial.println(bottleCount);
  }

  previousMeasure = currentMeasure;
}

void transmitBottleCount() {
  // Transmit the bottle count over BLE
  Serial.print("New bottlecount to send: ");
  Serial.println(bottleCount);
  bottleCharacteristic.writeValue(bottleCount);
}
