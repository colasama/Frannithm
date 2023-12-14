
#include "slider.h"

static Adafruit_MPR121 capA, capB, capC, capD; // mpr121定义
uint8_t checkLed = 0;

uint16_t curTouchedSlider[4];
uint16_t lastTouchedSlider[4] = {0, 0, 0, 0};

bool autoScan = false;
int keyMap[8] = {0, 1, 3, 2, 5, 4, 7, 6};
CRGB leds[NUM_LEDS];

// Init the touch part
void serialTouchSetup()
{
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, NUM_LEDS);
  Wire.setClock(800000); // I2C波特率
  capA.begin(CA_ADDR);
  capB.begin(CB_ADDR);
  capC.begin(CC_ADDR);
  capD.begin(CD_ADDR);

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
  uint8_t r, g, b;
  uint16_t hash = 0;
  // TODO: LED 相关
  for (int i = 0; i < 31; i++) {
    b = slider_req.leds[i * 3 + 0];
    r = slider_req.leds[i * 3 + 1];
    g = slider_req.leds[i * 3 + 2];
    uint8_t l = i;
    leds[l].r = g;
    BSD_HASH(hash, g);
    leds[l].g = r;
    BSD_HASH(hash, r);
    leds[l].b = b;
    BSD_HASH(hash, b);
  }
  if (hash != prev_hash) {
    prev_hash = hash;
    FastLED[0].showLeds(255);
  }
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
  capA.begin(CA_ADDR);
  capB.begin(CB_ADDR);
  capC.begin(CC_ADDR);
  capD.begin(CD_ADDR);
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

  uint16_t curTouchedSlider[] = {
    capA.touched(),
    capB.touched(),
    capC.touched(),
    capD.touched()
  };

  for (uint8_t i = 0; i < 8; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((curTouchedSlider[0] & _BV(i)) && !(lastTouchedSlider[0] & _BV(i)) ) {
      slider_resp.pressure[keyMap[i] + 24] = 255;
    }
    if ((curTouchedSlider[1] & _BV(i)) && !(lastTouchedSlider[1] & _BV(i)) ) {
      slider_resp.pressure[keyMap[i] + 16] = 255;
    }
    if ((curTouchedSlider[2] & _BV(i)) && !(lastTouchedSlider[2] & _BV(i))) {
      slider_resp.pressure[keyMap[i] + 8] = 255;
    }
    if ((curTouchedSlider[3] & _BV(i)) && !(lastTouchedSlider[3] & _BV(i))) {
      slider_resp.pressure[keyMap[i]] = 255;
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(curTouchedSlider[0] & _BV(i)) && (lastTouchedSlider[0] & _BV(i)) ) {
      slider_resp.pressure[keyMap[i] + 24] = 0;
    }
    if (!(curTouchedSlider[1] & _BV(i)) && (lastTouchedSlider[1] & _BV(i)) ) {
      slider_resp.pressure[keyMap[i] + 16] = 0;
    }
    if (!(curTouchedSlider[2] & _BV(i)) && (lastTouchedSlider[2] & _BV(i))) {
      slider_resp.pressure[keyMap[i] + 8] = 0;
    }
    if (!(curTouchedSlider[3] & _BV(i)) && (lastTouchedSlider[3] & _BV(i))) {
      slider_resp.pressure[keyMap[i]] = 0;
    }
  }
  lastTouchedSlider[0] = curTouchedSlider[0];
  lastTouchedSlider[1] = curTouchedSlider[1];
  lastTouchedSlider[2] = curTouchedSlider[2];
  lastTouchedSlider[3] = curTouchedSlider[3];
}

void sliderDebug() {
  uint16_t curTouchedSlider[] = {
    capA.touched(),
    capB.touched(),
    capC.touched(),
    capD.touched()
  };

  for (uint8_t i = 0; i < 8; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((curTouchedSlider[0] & _BV(i)) && !(lastTouchedSlider[0] & _BV(i)) ) {
      Serial.print("A按下："); Serial.println(keyMap[i] + 24);
    }
    if ((curTouchedSlider[1] & _BV(i)) && !(lastTouchedSlider[1] & _BV(i)) ) {
      Serial.print("B按下："); Serial.println(keyMap[i] + 16);
    }
    if ((curTouchedSlider[2] & _BV(i)) && !(lastTouchedSlider[2] & _BV(i))) {
      Serial.print("C按下："); Serial.println(keyMap[i] + 8);
    }
    if ((curTouchedSlider[3] & _BV(i)) && !(lastTouchedSlider[3] & _BV(i))) {
      Serial.print("D按下："); Serial.println(keyMap[i]);
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(curTouchedSlider[0] & _BV(i)) && (lastTouchedSlider[0] & _BV(i)) ) {
      Serial.print("A松开："); Serial.println(keyMap[i] + 24);
    }
    if (!(curTouchedSlider[1] & _BV(i)) && (lastTouchedSlider[1] & _BV(i)) ) {
      Serial.print("B松开："); Serial.println(keyMap[i] + 16);
    }
    if (!(curTouchedSlider[2] & _BV(i)) && (lastTouchedSlider[2] & _BV(i))) {
      Serial.print("C松开："); Serial.println(keyMap[i] + 8);
    }
    if (!(curTouchedSlider[3] & _BV(i)) && (lastTouchedSlider[3] & _BV(i))) {
      Serial.print("D松开："); Serial.println(keyMap[i] + 0);
    }
  }

  lastTouchedSlider[0] = curTouchedSlider[0];
  lastTouchedSlider[1] = curTouchedSlider[1];
  lastTouchedSlider[2] = curTouchedSlider[2];
  lastTouchedSlider[3] = curTouchedSlider[3];
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
//     slider_resp.pressure[keyMap[31 - i]] = CLAMP(pressure, 0, 255);

//     bv = capB.baselineData(i);
//     fd = capB.filteredData(i);
//     pressure = calTouch(bv, fd);
//     slider_resp.pressure[keyMap[19 - i]] = CLAMP(pressure, 0, 255);
//     if (i >= 4)
//     {
//       bv = capC.baselineData(i);
//       fd = capC.filteredData(i);
//       pressure = calTouch(bv, fd);
//       slider_resp.pressure[keyMap[11 - i]] = CLAMP(pressure, 0, 255);
//     }
//   }
//   // 键值重映射
//   // for(uint8_t i = 0; i < 32; i++) {
//   //   slider_resp.pressure[i] = pressures[keyMap[31 - i]];
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
    // LED 相关
    for (uint8_t i = 0; i < 31; i++) {
      leds[i] = CRGB(0, 255, 0);
    }
    FastLED.show();
    checkserial = 0;
    checkLed = 0;
    capA.begin(CA_ADDR);
    capB.begin(CB_ADDR);
    capC.begin(CC_ADDR);
    capD.begin(CD_ADDR);
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