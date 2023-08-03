#include "touch.h"

static MPR121 capA, capB, capC; // mpr121定义
uint8_t checkRelease[32];

////按键定义////
KeyboardKeycode KeyCode[32] = {//键值列表
   KEY_I, KEY_COMMA, KEY_8, KEY_K, KEY_U, KEY_M, KEY_7, KEY_J, KEY_Y, KEY_N,
   KEY_6, KEY_H, KEY_T, KEY_B, KEY_5, KEY_G, KEY_R, KEY_V, KEY_4, KEY_F, KEY_E, KEY_C,  
   KEY_3, KEY_D, KEY_W, KEY_X, KEY_2, KEY_S, KEY_Q, KEY_Z, KEY_1, KEY_A,
   }; 

// Init the touch part
void touchSetup()
{
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  capA.begin(CA_ADDR);
  capA.init();
  capA.run();

  capB.begin(CB_ADDR);
  capB.init();
  capB.run();

  capC.begin(CC_ADDR);
  capC.init();
  capC.run();

  Wire.setClock(800000); // I2C波特率
  Serial.println("[INFO] All MPR121 Connected!");
}

// 触摸数值计算
int calCheck(int bl, int fd) { 
  int cal = bl - fd;
  return cal > 4 ? 5 * cal : (cal > 0 ? cal : 0);
}

// 触摸检测
void touchLoop() {
  int16_t bl, fl, cal, calpress[32];

  // 注意：FRAN 台 v1 的键位是反的
  for (uint8_t i = 0; i < 12; i++)
  { // 计算数值
    bl = capA.baselineData(i);
    fl = capA.filteredData(i);
    cal = calCheck(bl, fl);
    calpress[i] = CLAMP(cal, 0, 255);

    bl = capB.baselineData(i);
    fl = capB.filteredData(i);
    cal = calCheck(bl, fl);
    calpress[i + 12] = CLAMP(cal, 0, 255);
    if (i >= 4)
    {
      bl = capC.baselineData(i);
      fl = capC.filteredData(i);
      cal = calCheck(bl, fl);
      calpress[i + 20] = CLAMP(cal, 0, 255);
    }
  }
  
  for (uint8_t i = 0; i < 32; i++)
  {
    uint8_t calkeypress = calpress[i];
    // Serial.println(calpress[i]);
    if (calkeypress >= TOUCH_THRESHOLD)
    {
      checkRelease[i] = SLIDER_CMD_AUTO_SCAN;
      NKROKeyboard.press(KeyCode[i]);
    }
    else
    {
      if (checkRelease[i])
      {
        checkRelease[i] = 0;
        // Serial.print("[DEBUG] Release Key: ");
        // Serial.println(i);
        NKROKeyboard.release(KeyCode[i]);
      }
      continue;
    }
  }
}

void touchDebugLoop() {
  int16_t debugBaseline[32], debugFiltered[32];
  int16_t cal, calpress[32];

  // 注意：FRAN 台 v1 的键位是反的
  for (uint8_t i = 0; i < 12; i++)
  { // 计算数值
    debugBaseline[i] = capA.baselineData(i);
    debugFiltered[i] = capA.filteredData(i);
    cal = calCheck(debugBaseline[i], debugFiltered[i]);
    calpress[i] = CLAMP(cal, 0, 255);

    debugBaseline[i + 12] = capB.baselineData(i);
    debugFiltered[i + 12] = capB.filteredData(i);
    cal = calCheck(debugBaseline[i], debugFiltered[i]);
    calpress[i + 12] = CLAMP(cal, 0, 255);
    if (i >= 4)
    {
      debugBaseline[i + 20] = capC.baselineData(i);
      debugFiltered[i + 20] = capC.filteredData(i);
      cal = calCheck(debugBaseline[i], debugFiltered[i]);
      calpress[i + 20] = CLAMP(cal, 0, 255);
    }
  }

  for (uint8_t i = 0; i < 32; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.print(calpress[i]);
    Serial.print("\t");
  }
  Serial.println();
  for (uint8_t i = 0; i < 12; i++) {
    Serial.print("A");
    Serial.print(i);
    Serial.print("\tB");
    Serial.print(debugBaseline[i]);
    Serial.print("F");
    Serial.print(debugFiltered[i]);
    Serial.print("\t");
  }
  Serial.println();
  for (uint8_t i = 0; i < 12; i++) {
    Serial.print("B");
    Serial.print(i);
    Serial.print("\tB");
    Serial.print(debugBaseline[i + 12]);
    Serial.print("F");
    Serial.print(debugFiltered[i + 12]);
    Serial.print("\t");
  }
  Serial.println();
  for (uint8_t i = 4; i < 12; i++) {
    Serial.print("C");
    Serial.print(i);
    Serial.print("\tB");
    Serial.print(debugBaseline[i + 20]);
    Serial.print("F");
    Serial.print(debugFiltered[i + 20]);
    Serial.print("\t");
  }
  Serial.println();
  Serial.println();
  delay(100);
}