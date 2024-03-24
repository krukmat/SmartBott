#include <ArduinoBLE.h>
#include <VL53L0X.h>

// Create a VL53L0X object
VL53L0X sensor;

// Define the BLE service and characteristic
BLEService bottleService("FB6D03F2-9F80-411B-BE52-FA618EBFF138");
BLEUnsignedIntCharacteristic bottleCharacteristic("FB6D03F3-9F80-411B-BE52-FA618EBFF138", BLERead | BLENotify | BLEWrite);

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

// bottle definitions
const float radio = 8; // en cm
const float altura_total = 32; // en cm
const float ajuste = 7;
const float capacidad_total = 7.5; // en litros

// Define the bottle count and previous measure
int bottleCount = 0;
enum Measure { EMPTY, HALF, FULL, UNAVAILABLE };
Measure previousMeasure = EMPTY;
const int ledPin = D10; // pin to use for the LED

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
  BLE.setLocalName("HydroTank");
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
  // Iniciar medición
  sensor.startContinuous();
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

float capacity(int distancia){
  // Convertir distancia a altura del líquido (en cm)
  float altura_liquido = altura_total - (distancia / 10.0 - ajuste); // convertir de mm a cm
  Serial.println(altura_liquido);
  if (altura_liquido < 0){
    altura_liquido = 0;
  }
  // Calcular el volumen del líquido en el recipiente (en litros)
  float volumen_liquido = PI * pow(radio, 2) * altura_liquido / 1000.0;
  Serial.println(volumen_liquido);
  return volumen_liquido;
}

void loop() {
  // Check the sensor reading and update the bottle count
  digitalWrite(ledPin, LOW);
  BLEDevice central = BLE.central();
  int reading = sensor.readRangeContinuousMillimeters();
  if (!sensor.timeoutOccurred()) {
    Serial.print("Reading: ");
    Serial.println(reading);
    Serial.print("bottleType:");
    Serial.println(bottleType);
    bottleCount = capacity(reading) * 100;
  } 
  if (central) {
     Serial.print("Connected to central: ");
     // print the central's MAC address:
     Serial.println(central.address());
     if (central.connected()){
         transmitBottleCount();
     }
  }
  
  delay(1000);
}

void transmitBottleCount() {
  // Transmit the bottle count over BLE
  Serial.print("New bottlecount to send: ");
  Serial.println(bottleCount);
  bottleCharacteristic.writeValue(bottleCount);
}
