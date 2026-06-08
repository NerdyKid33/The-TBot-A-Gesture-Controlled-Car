#include <TensorFlowLite.h>
#include <Wire.h>
#include <Arduino_OV767X.h>
#include "main_functions.h"
//#include <ArduinoBLE.h>
//#include <mbed.h>

/*BLEService customService("180C");
BLEStringCharacteristic ble_direction("2A56", BLENotify, 1);
volatile bool busy = false;*/

#include "detection_responder.h"
#include "image_provider.h"
#include "model_settings.h"
#include "person_detect_model_data.h"
#include "tensorflow/lite/micro/tflite_bridge/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"

/*mbed::Ticker bleTicker;
void pollBLE(){ BLE.poll(); }*/

namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
constexpr int kTensorArenaSize = 136 * 1024;
static uint8_t tensor_arena[kTensorArenaSize];
}

void ov7670_write(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(0x21);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  delay(500);

  /*while (!BLE.begin()){
    Serial.println("BLE failed");
    delay(100);
  }
  BLE.setLocalName("Arduino Direction Sensor");
  BLE.setAdvertisedService(customService);
  customService.addCharacteristic(ble_direction);
  BLE.addService(customService);
  BLE.advertise();
  bleTicker.attach(&pollBLE, 0.005f);  // 5ms polling
  Serial.println("Bluetooth active!:)");*/

  delay(1000);
  Wire.begin();
  ov7670_write(0x07, 0x00);
  ov7670_write(0x10, 0x05);
  ov7670_write(0x04, 0x00);
  ov7670_write(0x13, 0xC0);

  Serial.println("Camera ready.");
  delay(1000);

  while (!Camera.begin(QCIF, GRAYSCALE, 5)) {
    Serial.println("Failed to initialize camera!");
    delay(100);
  }
  Serial.println("CAMERA BEGAN SUCCESSFULLY");
  delay(500);

  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  model = tflite::GetModel(g_person_detect_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter, "Model schema version mismatch.");
    return;
  }

  static tflite::AllOpsResolver micro_op_resolver;

  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    Serial.println("ALLOCATE TENSORS FAILED");
    while(1);
  }
  Serial.println("Allocate Tensors OK");

  input = interpreter->input(0);
}

void loop() {
  unsigned long t0 = millis();
  //busy = true;
  if (kTfLiteOk != GetImage(error_reporter, kNumCols, kNumRows, kNumChannels,
                            input->data.int8)) {
    TF_LITE_REPORT_ERROR(error_reporter, "Image capture failed.");
    //busy = false;
    return;
  }
  //busy = false;
  Serial.print("GetImage ms: "); Serial.println(millis() - t0);

  unsigned long t1 = millis();
  //busy = true;
  if (kTfLiteOk != interpreter->Invoke()) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed.");
    //busy = false;
    return;
  }
  //busy = false;
  Serial.print("Invoke ms: "); Serial.println(millis() - t1);

  TfLiteTensor* output = interpreter->output(0);
  float output_scale = output->params.scale;
  int output_zero_point = output->params.zero_point;

  float max_confidence = -1.0f;
  int predicted_class = -1;

  for (int i = 0; i < kCategoryCount; i++) {
    float prob = (output->data.int8[i] - output_zero_point) * output_scale;
    if (prob > max_confidence) {
      max_confidence = prob;
      predicted_class = i;
    }
  }

  RespondToDetection(error_reporter, kCategoryLabels[predicted_class], max_confidence);
}