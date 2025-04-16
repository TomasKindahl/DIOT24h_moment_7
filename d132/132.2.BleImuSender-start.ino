/*
  IMU sender

  This example scans for Bluetooth® Low Energy peripherals until one with the advertised service
  "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
  it will remotely control the Bluetooth® Low Energy peripheral's IMU receiver,

  This example code is in the public domain.
*/

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
        /* READ GYRO HERE!! */
        /* ELSE IF NOT AVAILABLE, DO: */
        for(int i = 0; i < 3; i++) gyroVal[i] = 0;

        if (oldButtonState != buttonState) {
            // button changed
            oldButtonState = buttonState;

            if (buttonState) {
                Serial.println("button pressed");

                // button is pressed, write 0x01 to turn the LED on
                gyroVal[0] = 1;
                ledCharacteristic.writeValue(gyroVal,3*sizeof(float));
            } else {
                Serial.println("button released");

                // button is released, write 0x00 to turn the LED off
                gyroVal[0] = 0;
                ledCharacteristic.writeValue(gyroVal,3*sizeof(float));
            }
        }
    }

    Serial.println("Peripheral disconnected");
}
