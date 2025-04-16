/*
  IMU receiver

  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to print IMU data.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

BLEService imuService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy IMU Service

// Bluetooth® Low Energy IMU Data Characteristic - custom 128-bit UUID, read and writable by central
BLECharacteristic switchCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 3*sizeof(float), true);

const int ledPin = LED_BUILTIN; // pin to use for the LED
const int led1 = 2;
const int led2 = 3;
const int led3 = 4;

float gyroVal[3] =  {0, 0, 0};

void setup() {
    Serial.begin(9600);
    while (!Serial);

    // set LED pin to output mode
    pinMode(ledPin, OUTPUT);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    digitalWrite(led2, HIGH);

    // begin initialization
    if (!BLE.begin()) {
        Serial.println("starting Bluetooth® Low Energy module failed!");

        while (1);
    }

    // set advertised local name and service UUID:
    BLE.setLocalName("IMUrecv");
    BLE.setAdvertisedService(imuService);

    // add the characteristic to the service
    imuService.addCharacteristic(switchCharacteristic);

    // add service
    BLE.addService(imuService);

    // set the initial value for the characteristic:
    //switchCharacteristic.writeValue(0);

    // start advertising
    BLE.advertise();

    Serial.println("BLE LED Peripheral");
}

void loop() {
    // listen for Bluetooth® Low Energy peripherals to connect:
    BLEDevice central = BLE.central();

    // if a central is connected to peripheral:
    if (central) {
        Serial.print("Connected to central: ");
        // print the central's MAC address:
        Serial.println(central.address());

        // while the central is still connected to peripheral:
        while (central.connected()) {
            // if the remote device wrote to the characteristic,
            // use the value to control the LED:
            if (switchCharacteristic.written()) {
                switchCharacteristic.readValue(gyroVal, 3*sizeof(float));
                for(int i = 0; i < 3; i++) {
                    Serial.print(gyroVal[i]);
                    Serial.print(" ");
                }
                Serial.println();

                for(int i = 0; i < 3; i++)
                    if(gyroVal[i] < 0)
                        gyroVal[i] = -gyroVal[i];
                int high = 0;
                if(gyroVal[1] > gyroVal[high]) high = 1;
                if(gyroVal[2] > gyroVal[high]) high = 2;

                switch(high) {
                    case 0:
                        digitalWrite(led1, HIGH); 
                        digitalWrite(led2, LOW); 
                        digitalWrite(led3, LOW);
                        break;
                    case 1:
                        digitalWrite(led1, LOW); 
                        digitalWrite(led2, HIGH); 
                        digitalWrite(led3, LOW);
                        break;
                    case 2:
                        digitalWrite(led1, LOW); 
                        digitalWrite(led2, LOW); 
                        digitalWrite(led3, HIGH);
                        break;
                }
            }
        }

        // when the central disconnects, print it out:
        Serial.print(F("Disconnected from central: "));
        Serial.println(central.address());
    }
}
