#include <ArduinoBLE.h>
int drive1 = D2;
int dir1a = D3;
int dir1b = D4;
int drive2 = D7;
int dir2a = D5;
int dir2b = D6;
int drive4 = A0;
int dir4a = A1;
int dir4b = A2;
int dir3a=A3;
int dir3b = A4;
int drive3 = A5;
void setup() {
  // put your setup code here, to run once:
  pinMode(drive1, OUTPUT);
  pinMode(dir1a, OUTPUT);
  pinMode(dir1b, OUTPUT);
  pinMode(drive2, OUTPUT);
  pinMode(dir2a, OUTPUT);
  pinMode(dir2b, OUTPUT);
  pinMode(drive3, OUTPUT);
  pinMode(dir3a, OUTPUT);
  pinMode(dir3b, OUTPUT);
  pinMode(drive4, OUTPUT);
  pinMode(dir4a, OUTPUT);
  pinMode(dir4b, OUTPUT);

  Serial.begin(9600);
  delay(1000);
  if (!BLE.begin()){
    Serial.println("BLE failed");
    while (1);
  }
  pinMode(LEDG, OUTPUT);
  pinMode(LEDR, OUTPUT);
  Serial.println("BLE initialized. Hoorah!");
  BLE.scanForName("Arduino Direction Sensor");

}

void loop() {
  // put your main code here, to run repeatedly:
  /*analogWrite(drive1, 255);
  digitalWrite(dir1a, HIGH);
  digitalWrite(dir1b, LOW);

  analogWrite(drive2, 255);
  digitalWrite(dir2a, HIGH);
  digitalWrite(dir2b, LOW);

  analogWrite(drive3, 255);
  digitalWrite(dir3a, HIGH);
  digitalWrite(dir3b, LOW);

  analogWrite(drive4, 255);
  digitalWrite(dir4a, HIGH);
  digitalWrite(dir4b, LOW);*/
  if (!BLE.scanning()) {
    BLE.scanForName("Arduino Direction Sensor");
  }
  BLEDevice peripheral = BLE.available();
  Serial.println("Scanning...");
  if (peripheral){
    BLE.stopScan();
    Serial.println("Device found!");
    if (!peripheral.connect()) {
      delay(1000);
      Serial.println("Connection failed, rescanning...");
      BLE.scanForName("Arduino Direction Sensor");
      return;
    }
    Serial.println("Connected!");
    delay(1000);  // ← settle time HERE, after connect

    if (!peripheral.discoverAttributes()) {
      Serial.println("Attribute discovery failed, rescanning...");
      BLE.scanForName("Arduino Direction Sensor");
      return;
    }

    BLECharacteristic dirChar = peripheral.characteristic("2A56");
    if (!dirChar || !dirChar.canSubscribe()) {
      Serial.println("Characteristic not found, rescanning...");
      BLE.scanForName("Arduino Direction Sensor");
      return;
    }

    dirChar.subscribe();
    Serial.println("Subscribed! Waiting for data...");

    while (peripheral.connected()) {
      if (dirChar.valueUpdated()) {
        Serial.println("RECEIVING");
        int len = dirChar.valueLength();
        const uint8_t* val = dirChar.value();
        String result = "";
        for (int i = 0; i < len; i++) {result += (char)val[i];}
        digitalWrite(LEDG, LOW);
        digitalWrite(LEDR, HIGH);
        Serial.println(result);
        if (result=="0"){
          analogWrite(drive1, 100);
          digitalWrite(dir1a, HIGH);
          digitalWrite(dir1b, LOW);

          analogWrite(drive2, 128);
          digitalWrite(dir2a, HIGH);
          digitalWrite(dir2b, LOW);

          analogWrite(drive3, 100);
          digitalWrite(dir3a, HIGH);
          digitalWrite(dir3b, LOW);

          analogWrite(drive4, 128);
          digitalWrite(dir4a, HIGH);
          digitalWrite(dir4b, LOW);
        }
        if (result=="1"){
          analogWrite(drive1, 100);
          digitalWrite(dir1b, HIGH);
          digitalWrite(dir1a, LOW);

          analogWrite(drive2, 128);
          digitalWrite(dir2b, HIGH);
          digitalWrite(dir2a, LOW);

          analogWrite(drive3, 100);
          digitalWrite(dir3b, HIGH);
          digitalWrite(dir3a, LOW);

          analogWrite(drive4, 128);
          digitalWrite(dir4b, HIGH);
          digitalWrite(dir4a, LOW);
        }
        if (result=="2"){
          analogWrite(drive1, 100);
          digitalWrite(dir1b, HIGH);
          digitalWrite(dir1a, LOW);

          analogWrite(drive2, 128);
          digitalWrite(dir2b, LOW);
          digitalWrite(dir2a, HIGH);

          analogWrite(drive3, 100);
          digitalWrite(dir3b, HIGH);
          digitalWrite(dir3a, LOW);

          analogWrite(drive4, 128);
          digitalWrite(dir4b, LOW);
          digitalWrite(dir4a, HIGH);
        }
        if (result=="3"){
          analogWrite(drive1, 100);
          digitalWrite(dir1b, LOW);
          digitalWrite(dir1a, HIGH);

          analogWrite(drive2, 128);
          digitalWrite(dir2b, HIGH);
          digitalWrite(dir2a, LOW);

          analogWrite(drive3, 100);
          digitalWrite(dir3b, LOW);
          digitalWrite(dir3a, HIGH);

          analogWrite(drive4, 128);
          digitalWrite(dir4b, HIGH);
          digitalWrite(dir4a, LOW);
        }
        if (result=="4"){
          analogWrite(drive1, 0);
          digitalWrite(dir1b, HIGH);
          digitalWrite(dir1a, LOW);

          analogWrite(drive2, 0);
          digitalWrite(dir2b, HIGH);
          digitalWrite(dir2a, LOW);

          analogWrite(drive3, 0);
          digitalWrite(dir3b, HIGH);
          digitalWrite(dir3a, LOW);

          analogWrite(drive4, 0);
          digitalWrite(dir4b, HIGH);
          digitalWrite(dir4a, LOW);
        }
        Serial.print("Received: ");
        Serial.println(result);
      }
    }
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, HIGH);
    Serial.println("Disconnected, rescanning...");
    BLE.scanForName("Arduino Direction Sensor");
  }else{
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, HIGH);

  }
  Serial.println("No connection...");
}
