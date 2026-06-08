#include <ArduinoBLE.h>

BLEService customService("180C");
BLEStringCharacteristic ble_direction("2A56", BLENotify, 1);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);  // RX=D0, TX=D1 on Nano IoT

  while (!BLE.begin()) {
    Serial.println("BLE failed");
    delay(100);
  }
  BLE.setLocalName("Arduino Direction Sensor");
  BLE.setAdvertisedService(customService);
  customService.addCharacteristic(ble_direction);
  BLE.addService(customService);
  BLE.advertise();
  Serial.println("Relay ready!");
}

void loop() {
  BLE.poll();
  if (Serial1.available()) {
    String cmd = Serial1.readStringUntil('\n');
    cmd.trim();
    if (cmd.length() > 0) {
      ble_direction.writeValue(cmd);
      Serial.print("Relayed: ");
      Serial.println(cmd);
    }
  }
}