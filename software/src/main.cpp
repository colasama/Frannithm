#include <Arduino.h>
#include <Wire.h>
#include "air.h"
#include "touch.h"
#include "config.h"

void setup() {
    // Start serial connection
    Serial.begin(115200);
    Serial.setTimeout(0);
    while (!Serial) { // needed to keep leonardo/micro from starting too fast!
        delay(10);
    }
    Serial.println("[INFO] Frannithm is starting up...");
    
    touchSetup();
    airSetup();
    airCalibrate();
}

void loop() {
	// airDebug();
    airLoop();
    touchLoop();
}
