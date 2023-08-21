#include "config.h"
// 调试用库
// #include "Adafruit_MPR121.h"
#include "mpr121-helper.h"
#include "mpr121-config.h"
// #include "serial.h"
#include "keyboardMultiple.h"
#include "USB.h"
#include <USBHID.h>
#include <map>
// 串口定义
#define CLAMP(val, lo, hi) (val < lo ? lo : (val > hi ? hi : val))


void touchSetup();
void touchLoop();
void touchDebug();