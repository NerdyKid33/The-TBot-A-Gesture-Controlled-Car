#include "image_provider.h"
#include <Wire.h>

#ifndef ARDUINO_EXCLUDE_CODE

#include "Arduino.h"
#include "Arduino_OV767X.h"

// Must be outside GetImage

TfLiteStatus GetImage(tflite::ErrorReporter* error_reporter, int image_width,
                      int image_height, int channels, int8_t* image_data) {

  byte data[176 * 144];
  
  Camera.readFrame(data);

  int min_x = (176 - 96) / 2;
  int min_y = (144 - 96) / 2;
  int index = 0;

  for (int y = min_y; y < min_y + 96; y++) {
    for (int x = min_x; x < min_x + 96; x++) {
      image_data[index++] = static_cast<int8_t>(data[(y * 176) + x] - 128);
    }
  }

  return kTfLiteOk;
}

#endif