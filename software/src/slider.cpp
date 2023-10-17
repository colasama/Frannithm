
#include "slider.h"

static Adafruit_MPR121 capA, capB, capC; // mpr121定义
uint8_t checkLed = 0;
bool autoScan = false;
int keyMapForFranV1[32] = {10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1, 22, 23, 20, 21, 18, 19, 16, 17, 14, 15, 12, 13, 30, 31, 28, 29, 26, 27, 24, 25};
// Init the touch part
void serialTouchSetup()
{
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!capA.begin(CA_ADDR)) {
    Serial.println("MPR121 A not found, check wiring?");
    while (1);
  }
  if (!capB.begin(CB_ADDR)) {
    Serial.println("MPR121 B not found, check wiring?");
    while (1);
  }
  if (!capC.begin(CC_ADDR)) {
    Serial.println("MPR121 C not found, check wiring?");
    while (1);
  }
  Wire.setClock(400000); // I2C波特率
  Serial.println("[INFO] Serial Mode, All MPR121 Connected!");
}

// 串口模式循环
void serialTouchLoop() {
  switch (sliderserial_readreq()) {
    case SLIDER_CMD_SET_LED:
      sliderSetLED();
      break;
    case SLIDER_CMD_AUTO_SCAN_START:
      sliderScanStart();
      break;
    case SLIDER_CMD_AUTO_SCAN_STOP:
      sliderScanStop();
      break;
    case SLIDER_CMD_RESET:
      sliderReset();
      break;
    case SLIDER_CMD_GET_BOARD_INFO:
      sliderGetBoardInfo();
      break;
    default:
      sliderScan();
  }
  sliderserial_writeresp();
}

void sliderSetLED() {
      uint16_t prev_hash = -1;
  if (slider_tx_pending) {
    return;
  }
  // uint8_t   r, g, b;
  // uint16_t hash = 0;
  // TODO: LED 相关，暂时不启用
  // for (int i = 0; i < 31; i++) {
  //   b = slider_req.leds[i * 3 + 0];
  //   r = slider_req.leds[i * 3 + 1];
  //   g = slider_req.leds[i * 3 + 2];
  //   uint8_t l = i;
  //   leds[l].r = g;
  //   BSD_HASH(hash, g);
  //   leds[l].g = r;
  //   BSD_HASH(hash, r);
  //   leds[l].b = b;
  //   BSD_HASH(hash, b);
  // }
  // if (hash != prev_hash) {
  //   prev_hash = hash;
  //   FastLED[0].showLeds(255);
  // }
  slider_req.cmd = 0;
}

void sliderScanStart() {
  autoScan = true;
  sliderScan();
}

void sliderScanStop() {
  autoScan = false;
  slider_resp.syn = 0xff;
  slider_resp.cmd = SLIDER_CMD_AUTO_SCAN_STOP;
  slider_resp.size = 0;
  slider_req.cmd = 0;
}

void sliderReset() {
  if (!capA.begin(CA_ADDR)) {
    Serial.println("MPR121 A not found, check wiring?");
    while (1);
  }
  if (!capB.begin(CB_ADDR)) {
    Serial.println("MPR121 B not found, check wiring?");
    while (1);
  }
  if (!capC.begin(CC_ADDR)) {
    Serial.println("MPR121 C not found, check wiring?");
    while (1);
  }
  slider_resp.syn = 0xff;
  slider_resp.cmd = SLIDER_CMD_RESET;
  slider_resp.size = 0;
  slider_req.cmd = 0;
}

void sliderGetBoardInfo() {
  slider_resp.syn = 0xff;
  slider_resp.cmd = SLIDER_CMD_GET_BOARD_INFO;
  slider_resp.size = sizeof(slider_resp.version);
  strcpy(slider_resp.version, "15330   \xA0" "06712\xFF" "\x90");
  slider_req.cmd = 0;
}

void sliderScan() {
  if (!autoScan || slider_tx_pending) {
    serialNotFound(); // serial尚未接到包头0xff时运行
    return;
  }
  slider_resp.syn = 0xff;
  slider_resp.cmd = SLIDER_CMD_AUTO_SCAN;
  slider_resp.size = sizeof(slider_resp.pressure);

  uint16_t curTouched[] = {
    capA.touched(),
    capB.touched(),
    capC.touched()
  };

  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((curTouched[0] & _BV(i)) && !(lastTouched[0] & _BV(i)) ) {
      slider_resp.pressure[keyMapForFranV1[31 - i]] = 255;
    }
    if ((curTouched[1] & _BV(i)) && !(lastTouched[1] & _BV(i)) ) {
      slider_resp.pressure[keyMapForFranV1[19 - i]] = 255;
    }
    if ((curTouched[2] & _BV(i)) && !(lastTouched[2] & _BV(i))) {
      if (i >= 4) {
        slider_resp.pressure[keyMapForFranV1[11 - i]] = 255;
      }
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(curTouched[0] & _BV(i)) && (lastTouched[0] & _BV(i)) ) {
      slider_resp.pressure[keyMapForFranV1[31 - i]] = 0;
    }
    if (!(curTouched[1] & _BV(i)) && (lastTouched[1] & _BV(i)) ) {
      slider_resp.pressure[keyMapForFranV1[19 - i]] = 0;
    }
    if (!(curTouched[2] & _BV(i)) && (lastTouched[2] & _BV(i))) {
      if (i >= 4) {
        slider_resp.pressure[keyMapForFranV1[11 - i]] = 0;
      }
    }
  }

  lastTouched[0] = curTouched[0];
  lastTouched[1] = curTouched[1];
  lastTouched[2] = curTouched[2];
}

// void sliderScan() {
//   if (!autoScan || slider_tx_pending) {
//     serialNotFound(); // serial尚未接到包头0xff时运行
//     return;
//   }
//   slider_resp.syn = 0xff;
//   slider_resp.cmd = SLIDER_CMD_AUTO_SCAN;
//   slider_resp.size = sizeof(slider_resp.pressure);
//   int16_t bv, fd, pressure;
//   for (uint8_t i = 0; i < 12; i++)
//   { // 计算数值
//     bv = capA.baselineData(i);
//     fd = capA.filteredData(i);
//     pressure = calTouch(bv, fd);
//     slider_resp.pressure[keyMapForFranV1[31 - i]] = CLAMP(pressure, 0, 255);

//     bv = capB.baselineData(i);
//     fd = capB.filteredData(i);
//     pressure = calTouch(bv, fd);
//     slider_resp.pressure[keyMapForFranV1[19 - i]] = CLAMP(pressure, 0, 255);
//     if (i >= 4)
//     {
//       bv = capC.baselineData(i);
//       fd = capC.filteredData(i);
//       pressure = calTouch(bv, fd);
//       slider_resp.pressure[keyMapForFranV1[11 - i]] = CLAMP(pressure, 0, 255);
//     }
//   }
//   // 键值重映射
//   // for(uint8_t i = 0; i < 32; i++) {
//   //   slider_resp.pressure[i] = pressures[keyMapForFranV1[31 - i]];
//   // }
//   // aircheck();//air检测
//   // air_rgb_Serial();
// }

void serialNotFound() {
  if (SerialUSB.available()) {
    checkserial = 1;
    return;
  }
  if (checkserial) {
    // TODO: LED 相关，暂时不启用
    // for (uint8_t i = 0; i < 32; i++) {
    //   leds[i] = CRGB(0, 255, 0);
    // }
    // FastLED.show();
    checkserial = 0;
    checkLed = 0;
    if (!capA.begin(CA_ADDR)) {
      Serial.println("MPR121 A not found, check wiring?");
      while (1);
    }
    if (!capB.begin(CB_ADDR)) {
      Serial.println("MPR121 B not found, check wiring?");
      while (1);
    }
    if (!capC.begin(CC_ADDR)) {
      Serial.println("MPR121 C not found, check wiring?");
      while (1);
    }
    delay(100);
    return;
  }
  // keyboardmod();
}

// 触摸数值计算
int calTouch(int bv, int fd) { 
  int cal = bv - fd;
  return cal > 4 ? 5 * cal : (cal > 0 ? cal : 0);
}