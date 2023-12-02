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
    // 先等待5秒再开始校准
    for(int i=0; i<5; i++) {
        digitalWrite(LED_BUILTIN_RX, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN_RX, LOW);
    }
    NKROKeyboard.begin();
    touchSetup();
    airSetup();
    airCalibrate();
    // 尝试减少开局误触，但是无用，记得删除并修复这个问题
    delay(300);
    // // Debugging CY8CMBR31XX
    // Serial.begin(115200);
    // Serial.setTimeout(0);
    // cyInit();
}

void loop() {
	// airDebug();
    // airLoop(); 
    // touchDebug();
    touchLoop();
    // cyDebugAllButtonStatus();
    // // cyDebugAllButtonStatus(0x38);
    // delay(50);
}
