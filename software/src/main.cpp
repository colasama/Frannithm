#include <Arduino.h>
#include <Wire.h>
#include "air.h"
#include "touch.h"
#include "config.h"
#include "HID-Project.h"

void setup() {
    // Start serial connection
    Serial.begin(115200);
    Serial.setTimeout(0);//******** /**/
    Serial.println("[INFO] Frannithm is starting up...");
    NKROKeyboard.begin();
    touchSetup();
    airSetup();
    airCalibrate();
}

void loop() {
	// airDebug();
    // airLoop(); 
    touchDebug();
    // touchLoop();
}
