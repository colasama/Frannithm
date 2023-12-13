#include <Arduino.h>
#include <Wire.h>
#include "air.h"
// 开启原生串口模式，注释掉则为键盘模式
#define ENABLE_SERIAL_MODE 1 

#ifdef ENABLE_SERIAL_MODE
#include "slider.h"
#else
#include "touch.h"
#endif
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
    #ifdef ENABLE_SERIAL_MODE
    serialTouchSetup();
    #else
    touchSetup();
    // airSetup();
    // airCalibrate();
}

void loop() {
	// airDebug();
    // airLoop(); 
    #ifdef ENABLE_SERIAL_MODE
    serialTouchLoop();
    // slider/*Debug();
    #else
    touchLoopNew();
    // touchDebug();
    #endif
}
