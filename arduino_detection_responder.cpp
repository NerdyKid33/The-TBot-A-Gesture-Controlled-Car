#include "detection_responder.h"
#include "Arduino.h"
#include <mbed.h>

UART mySerial(digitalPinToPinName(11), digitalPinToPinName(12), NC, NC);

void RespondToDetection(tflite::ErrorReporter* error_reporter,
                        const char* label, float confidence) {
  static bool is_initialized = false;
  if (!is_initialized) {
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    mySerial.begin(9600);
    is_initialized = true;
  }

  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDB, HIGH);

  Serial.println(label);
  Serial.println(confidence);

  const char* sendchar = "4";
  if (confidence < 0.3f) {
    digitalWrite(LEDR, LOW);
    sendchar = "4";
    Serial.println("Nothing...confidence too low");
  } else {
    Serial.println("Something detected...");
    digitalWrite(LEDG, LOW);
    if (strcmp(label, "dislike") == 0)      sendchar = "1";
    else if (strcmp(label, "fist") == 0)    sendchar = "2";
    else if (strcmp(label, "like") == 0)    sendchar = "0";
    else if (strcmp(label, "nothing") == 0) sendchar = "4";
    else if (strcmp(label, "ok") == 0)      sendchar = "3";
    else                                     sendchar = "4";
  }

  mySerial.write(sendchar, 1);
  mySerial.write("\n", 1);
}