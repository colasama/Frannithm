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
    // 先等待3秒再开始校准
    for(int i=0; i<3; i++) {
        digitalWrite(LED_BUILTIN_RX, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN_RX, LOW);
    }
    NKROKeyboard.begin();
    touchSetup();
    airSetup();
    airCalibrate();
}

void loop() {
	// airDebug();
    airLoop(); 
    // touchDebug();
    // touchLoop();
    touchLoopNew();
}
