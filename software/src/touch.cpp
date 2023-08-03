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
  


// void getTouched() {
//   // Get the currently touched pads
//   currtouched = cap.touched();

//   for (uint8_t i=0; i<12; i++) {
//     // it if *is* touched and *wasnt* touched before, alert!
//     if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
//       Serial.print(i); Serial.println(" touched");
//     }
//     // if it *was* touched and now *isnt*, alert!
//     if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
//       Serial.print(i); Serial.println(" released");
//     }
//   }

//   // reset our state
//   lasttouched = currtouched;

//   // comment out this line for detailed data from the sensor!
//   return;

//   // debugging info, what
//   Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
//   Serial.print("Filt: ");
//   for (uint8_t i=0; i<12; i++) {
//     Serial.print(cap.filteredData(i)); Serial.print("\t");
//   }
//   Serial.println();
//   Serial.print("Base: ");
//   for (uint8_t i=0; i<12; i++) {
//     Serial.print(cap.baselineData(i)); Serial.print("\t");
//   }
//   Serial.println();

//   // put a delay so it isn't overwhelming
//   delay(100);
// }

// 直连触摸函数部分//
int calCheck(int bl, int fd) { // 触摸数值计算
  int cal = bl - fd;
  return cal > 4 ? 5 * cal : (cal > 0 ? cal : 0);
}

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
    if (calkeypress >= SLIDER_THRESHOLDS)
    { // Check
      checkRelease[i] = SLIDER_CMD_AUTO_SCAN;
      // Serial.print("[DEBUG] Press Key: ");
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
  int16_t testA[12], testB[12], testC[12];
  int16_t testAF[12], testBF[12], testCF[12];

  int16_t cal, calpress[32];

  // 注意：FRAN 台 v1 的键位是反的
  for (uint8_t i = 0; i < 12; i++)
  { // 计算数值
    testA[i] = capA.baselineData(i);
    testAF[i] = capA.filteredData(i);
    cal = calCheck(testA[i], testAF[i]);
    calpress[i] = CLAMP(cal, 0, 255);

    testB[i] = capB.baselineData(i);
    testBF[i] = capB.filteredData(i);
    cal = calCheck(testB[i], testBF[i]);
    calpress[i + 12] = CLAMP(cal, 0, 255);
    if (i >= 4)
    {
      testC[i] = capC.baselineData(i);
      testCF[i] = capC.filteredData(i);
      cal = calCheck(testC[i], testCF[i]);
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
    Serial.print(testA[i]);
    Serial.print("F");
    Serial.print(testAF[i]);
    Serial.print("\t");
  }
  Serial.println();
  for (uint8_t i = 0; i < 12; i++) {
    Serial.print("B");
    Serial.print(i);
    Serial.print("\tB");
    Serial.print(testB[i]);
    Serial.print("F");
    Serial.print(testBF[i]);
    Serial.print("\t");
  }
  Serial.println();
  for (uint8_t i = 4; i < 12; i++) {
    Serial.print("C");
    Serial.print(i);
    Serial.print("\tB");
    Serial.print(testC[i]);
    Serial.print("F");
    Serial.print(testCF[i]);
    Serial.print("\t");
  }
  Serial.println();
  Serial.println();
  delay(100);
}