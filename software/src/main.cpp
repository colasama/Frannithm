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
    NKROKeyboard.begin();

    #ifdef ENABLE_SERIAL_MODE
    serialTouchSetup();
    #else
    touchSetup();
    #endif
    airSetup();
    airCalibrate();
}

void loop() {
	// airDebug();
    airLoop();
    // touchDebug();；‘
    #ifdef ENABLE_SERIAL_MODE
    serialTouchLoop();
    #else
    // touchLoop();
    touchDebug();
    #endif
}