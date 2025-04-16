/*
  IMU sender

  This example scans for Bluetooth® Low Energy peripherals until one with the advertised service
  "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
  it will remotely control the Bluetooth® Low Energy peripheral's IMU receiver,

  This example code is in the public domain.
*/

#include "Arduino_BMI270_BMM150.h"

#include <ArduinoBLE.h>

// variables for button
const int buttonPin = 2;
int oldButtonState = LOW;

float gyroVal[3];

void setup() {
    Serial.begin(9600);
    while (!Serial);

    // configure the button pin as input
    pinMode(buttonPin, INPUT);

    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (1);
    }
    Serial.print("Gyroscope sample rate = ");
    Serial.print(IMU.gyroscopeSampleRate());
    Serial.println(" Hz");
    Serial.println();
    Serial.println("Gyroscope in degrees/second");
    Serial.println("X\tY\tZ");

    // initialize the Bluetooth® Low Energy hardware
    BLE.begin();

    Serial.println("Bluetooth® Low Energy Central - LED control");

    // start scanning for peripherals
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
}

void loop() {
    // check if a peripheral has been discovered
    BLEDevice peripheral = BLE.available();

    if (peripheral) {
        // discovered a peripheral, print out address, local name, and advertised service
        Serial.print("Found ");
        Serial.print(peripheral.address());
        Serial.print(" '");
        Serial.print(peripheral.localName());
        Serial.print("' ");
        Serial.print(peripheral.advertisedServiceUuid());
        Serial.println();

        if (peripheral.localName() != "IMUrecv") {
            return;
        }

        // stop scanning
        BLE.stopScan();

        controlLed(peripheral);

        // peripheral disconnected, start scanning again
        BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
    }
}

void controlLed(BLEDevice peripheral) {
    // connect to the peripheral
    Serial.println("Connecting ...");

    if (peripheral.connect()) {
        Serial.println("Connected");
    } else {
        Serial.println("Failed to connect!");
        return;
    }

    // discover peripheral attributes
    Serial.println("Discovering attributes ...");
    if (peripheral.discoverAttributes()) {
        Serial.println("Attributes discovered");
    } else {
        Serial.println("Attribute discovery failed!");
        peripheral.disconnect();
        return;
    }

    // retrieve the IMU receiver characteristic
    BLECharacteristic ledCharacteristic = peripheral.characteristic("19b10002-e8f2-537e-4f6c-d104768a1214");

    if (!ledCharacteristic) {
        Serial.println("Peripheral does not have IMU receiver characteristic!");
        peripheral.disconnect();
        return;
    } else if (!ledCharacteristic.canWrite()) {
        Serial.println("Peripheral does not have a writable IMU receiver characteristic!");
        peripheral.disconnect();
        return;
    }

    while (peripheral.connected()) {
        // while the peripheral is connected

        // read the button pin
        int buttonState = digitalRead(buttonPin);
        /* READ GYRO HERE */
        if (IMU.gyroscopeAvailable()) {
            float x, y, z;
            IMU.readGyroscope(x, y, z);
            gyroVal[0] = x;
            gyroVal[1] = y;
            gyroVal[2] = z;
        }
        else {
            for(int i = 0; i < 3; i++) gyroVal[i] = 0;
        }
        
        ledCharacteristic.writeValue(gyroVal,3*sizeof(float));
        delay(1000);
    }

    Serial.println("Peripheral disconnected");
}
