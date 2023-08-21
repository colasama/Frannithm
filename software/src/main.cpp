#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "air.h"
#include "touch.h"
#include "USB.h"
#include "USBHIDVendor.h"

void setup() {
    // Start serial connection
    Serial.begin(115200);
    Serial.setTimeout(0);
    Serial.println("[INFO] Frannithm is starting up...");
    // Wire.setClock(3400000);
    touchSetup();
    airSetup();
    airCalibrate();
    USB.PID(0x8222);
    USB.productName("Frannithm Controller");
    USB.manufacturerName("Colanns");
    USB.serialNumber("0821");
    USB.begin();
}

void loop() {
	airDebug();
    // touchDebug();
    // airLoop();
    // touchLoop();
}
