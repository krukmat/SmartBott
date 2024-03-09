#include <ArduinoBLE.h>
#include <VL53L0X.h>

// Create a VL53L0X object
VL53L0X sensor;

// Define the BLE service and characteristic
BLEService bottleService("19B10001-E8F2-537E-4F6C-D104768A1214");
BLEUnsignedIntCharacteristic bottleCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);

// Define the bottle types and sensor thresholds
enum BottleType { LITRE_1_5, LITRE_0_5, LITRE_1, LITRE_8};
BottleType bottleType = LITRE_8;
const int THRESHOLD_FULL = 60;
//const int THRESHOLD_HALF_1_5 = 190;
//const int THRESHOLD_HALF_0_5 = 130;
const int THRESHOLD_EMPTY_1_5 = 300;
const int THRESHOLD_EMPTY_1 = 270;
const int THRESHOLD_EMPTY_0_5 = 230;

const int THRESHOLD_EMPTY_8 = 320;


boolean counterEnabled = true;

// Define the bottle count and previous measure
int bottleCount = 0;
enum Measure { EMPTY, HALF, FULL, UNAVAILABLE };
Measure previousMeasure = EMPTY;
const int ledPin = LED_BUILTIN; // D10; // pin to use for the LED

void setup() {
  Serial.begin(9600);
  Wire.begin();
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

    
    uint8_t datas[characteristic.valueLength()];
  characteristic.readValue(datas, characteristic.valueLength());
    Serial.println(datas[0]);
    
    if (datas[0] == 215){
      int receivedValue = bottleCharacteristic.value(); // Retrieve the written value
      Serial.println("Reset the bottle count");
      bottleCount = 0;
    } else {
      bottleCount = 0;
      Serial.println("Define capacity");      
      if (datas[0] == 223) {
        // 1 litre
        bottleType = LITRE_1;
        Serial.println("1 litre");
      } else if (datas[0] == 219) {
        // 0.5 litre
        Serial.println("0.5 litre");
        bottleType = LITRE_0_5;
      } else if (datas[0] == 231) {
          bottleType = LITRE_1_5;
          Serial.println("1.5 litre");        
      } else {
        // toggle the bottle count init
        counterEnabled = !counterEnabled;
      }
    } 
   Serial.print("+++++++++++++++++++++++++++++++++++++++++++++++++");

    // Include your logic here to handle the receivedValue
  });

  Serial.println("Hydro reader");
}

void loop() {
  // Check the sensor reading and update the bottle count
  digitalWrite(ledPin, LOW);
  BLEDevice central = BLE.central();
  int reading = sensor.readRangeSingleMillimeters();
  Serial.print("Reading: ");
  Serial.println(reading);
  Serial.print("bottleType:");
  Serial.println(bottleType);

  if (bottleType != LITRE_8){
    updateBottleCount(reading);    
  }
  else {
    // contar la cantidad de litros remanente
    countRemainingBottles(reading);
  }

  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    if (central.connected()){
        if ((bottleType != LITRE_8 && bottleCount > 0) || (bottleType == LITRE_8))
          transmitBottleCount();
      }
  }
  delay(1000);
}

void countRemainingBottles(int reading) {
  int remainingLitres = 0;
  if (reading >= 340) {
    bottleCount = 0;
  } else if (reading >= 330 && reading < 340) {
    bottleCount = 1;
  } else if (reading >= 313 && reading < 330) {
    bottleCount = 2;
  } else if (reading >= 300 && reading < 313) {
    bottleCount = 3;
  } else if (reading >= 260 && reading < 300) {
    bottleCount = 4;
  } else if (reading >= 245 && reading < 260) {
    bottleCount = 5;
  } else if (reading >= 223 && reading < 245) {
    bottleCount = 6;
  } else if (reading >= 193 && reading < 223) {
    bottleCount = 7;
  } else if (reading >= 175 && reading < 193) {
    bottleCount = 8;
  } else if (reading >= 160 && reading < 175) {
    bottleCount = 9;
  } else if (reading >= 140 && reading < 160) {
    bottleCount = 10;
  } else if (reading >= 120 && reading < 140) {
    bottleCount = 11;
  } else if (reading >= 102 && reading < 120) {
    bottleCount = 12;
  } else if (reading >= 85 && reading < 102) {
    bottleCount = 13;
  } else if (reading >= 70 && reading < 85) {
    bottleCount = 14;
  } else {
    bottleCount = 15;
  }
}


void updateBottleCount(int reading) {
  // Update the bottle count based on the sensor reading
  Measure currentMeasure;
  if (reading>1000 || reading < THRESHOLD_FULL){
    currentMeasure = UNAVAILABLE;
    counterEnabled = false;
    return;
  }
  counterEnabled = true;

  if (reading > 0 && reading <= THRESHOLD_FULL) {
    currentMeasure = FULL;
  } else if ( (bottleType == LITRE_1_5 && reading > THRESHOLD_FULL && reading < THRESHOLD_EMPTY_1_5) || 
              (bottleType == LITRE_0_5 && reading > THRESHOLD_FULL && reading < THRESHOLD_EMPTY_0_5) ||  
              (bottleType == LITRE_1 && reading > THRESHOLD_FULL && reading < THRESHOLD_EMPTY_1))  {
    currentMeasure = HALF;
  } else {
    currentMeasure = EMPTY;
  }
  Serial.print("PreviousMeasure: ");
  Serial.println(previousMeasure)
  Serial.print("Current Measure: ");
  Serial.println(currentMeasure);
  Serial.print("Bottle count:");
  Serial.println(bottleCount);
  Serial.print("Counter enabled:");
  Serial.println(counterEnabled);
  if (counterEnabled && ((previousMeasure == HALF && currentMeasure == EMPTY) || (previousMeasure == FULL && currentMeasure == EMPTY))) {
    bottleCount++;
    //counterEnabled = false;
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
