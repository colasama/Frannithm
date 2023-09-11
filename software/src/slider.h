#ifndef SLIDER_H
#define SLIDER_H

#include "config.h"
#include "mpr121-helper.h"
#include "mpr121-config.h"
#include "serial.h"
#include "FastLED.h"

// 串口定义
#define CLAMP(val, lo, hi) (val < lo ? lo : (val > hi ? hi : val))
#define BSD_HASH(checksum, ch) do {\
    checksum = (checksum >> 1) + ((checksum & 1) << 15);\
    checksum += ch;\
  } while(0);

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

void serialTouchSetup();
void serialTouchLoop();
void sliderSetLED();
void sliderScanStart();
void sliderScanStop();
void sliderReset();
void sliderGetBoardInfo();
void sliderGetBoardInfo();
void sliderScan();
void serialNotFound();
int calTouch(int bv, int fd);
#endif