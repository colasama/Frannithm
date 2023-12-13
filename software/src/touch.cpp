#include "touch.h"

uint8_t checkRelease[32];

////按键定义////
KeyboardKeycode KeyCode[32] = {//键值列表
   KEY_I, KEY_COMMA, KEY_8, KEY_K, KEY_U, KEY_M, KEY_7, KEY_J, KEY_Y, KEY_N,
   KEY_6, KEY_H, KEY_T, KEY_B, KEY_5, KEY_G, KEY_R, KEY_V, KEY_4, KEY_F, KEY_E, KEY_C,  
   KEY_3, KEY_D, KEY_W, KEY_X, KEY_2, KEY_S, KEY_Q, KEY_Z, KEY_1, KEY_A,
   }; 

uint8_t TOUCH_THRESHOLD[32] = {
  210, 45, 45, 45, 45, 45, 45, 45,
  45, 45, 45, 45, 45, 45, 45, 45,
  45, 45, 45, 45, 45, 45, 45, 45,
  45, 45, 45, 45, 45, 45, 45, 45
 };

// Init the touch part
void touchSetup()
{
  cyInit();
  Wire.setClock(800000); // I2C波特率
}

// 触摸检测
void touchLoop() {
  int16_t cyStatus[2];
  cyStatus[0] = cyGetRawButtonStatus(0x37);
  cyStatus[1] = cyGetRawButtonStatus(0x38);
  // Serial.print("[DEBUG] Status: ");
  // for (int i = 0; i < 16; i++)
  //   {
  //     Serial.print(i);
  //     Serial.print(": ");
  //     bool temp = cyStatus[0] & (1 << i);
  //     Serial.print(temp);
  //     Serial.print("\t");
  //   }
  //   for (int i = 0; i < 16; i++)
  //   {
  //     Serial.print(i);
  //     Serial.print(": ");
  //     bool temp = cyStatus[1] & (1 << i);
  //     Serial.print(temp);
  //     Serial.print("\t");
  //   }
  // Serial.println();
  for (uint8_t i = 0; i < 16; i++) {
    
    if ((bool)(cyStatus[0] & (1 << i))) {
      // Serial.print(i);
      NKROKeyboard.press(KeyCode[i]);
    }
    else {
      NKROKeyboard.release(KeyCode[i]);
    }
    if ((bool)(cyStatus[1] & (1 << i))) {
      // Serial.print(i + 15);
      NKROKeyboard.press(KeyCode[i + 16]);
    }
    else {
      NKROKeyboard.release(KeyCode[i + 16]);
    }
  }
}

void touchDebug() {
  return;
}