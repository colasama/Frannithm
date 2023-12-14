#include "config.h"
// 调试用库
#include "Adafruit_MPR121.h"
// #include "mpr121-config.h"
#include "HID-Project.h"
#include "serial.h"
#include "FastLED.h"

// 串口定义
#define CLAMP(val, lo, hi) (val < lo ? lo : (val > hi ? hi : val))

//初始化RGB灯的颜色
struct {
  uint8_t r = 0;
  uint8_t g = 255;
  uint8_t b = 255;
} initializationRGB;

//键盘模式下，当被触摸时的颜色
struct {
  uint8_t r = 255;
  uint8_t g = 0;
  uint8_t b = 255;
} keyboardPressRGB;

//键盘模式下，当触摸后释放时的颜色
struct {
  uint8_t r = 0;
  uint8_t g = 255;
  uint8_t b = 255;
} keyboardReleaseRGB;

void touchSetup();
void touchLoop();
void touchLoopNew();
void touchDebug();
