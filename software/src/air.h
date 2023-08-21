#include "config.h"
#include <Arduino.h>
#include <SimpleKalmanFilter.h>
// #include "HID-Project.h"
#include "keyboardMultiple.h"
#include "USB.h"
#include <USBHID.h>
#include <map>

// const int AIR_KEYS[] = {AIR_KEY_0, AIR_KEY_1, AIR_KEY_2, AIR_KEY_3, AIR_KEY_4, AIR_KEY_5};
const int AIR_TX_PINS[] = {AIR_TX_PIN_0, AIR_TX_PIN_1, AIR_TX_PIN_2, AIR_TX_PIN_3, AIR_TX_PIN_4, AIR_TX_PIN_5};
const int AIR_RX_PINS[] = {AIR_RX_PIN_0, AIR_RX_PIN_1, AIR_RX_PIN_2, AIR_RX_PIN_3, AIR_RX_PIN_4, AIR_RX_PIN_5};

void airSetup();
void airCalibrate();
void airLoop();
void airDebug();