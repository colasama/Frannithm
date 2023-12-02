#include "config.h"
#include "cy8cmbr3116.h"
#include "HID-Project.h"
#include "serial.h"

// 串口定义
#define CLAMP(val, lo, hi) (val < lo ? lo : (val > hi ? hi : val))


void touchSetup();
void touchLoop();
void touchDebug();