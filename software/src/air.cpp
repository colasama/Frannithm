#include "air.h"

USBHIDKeyboard airKeyboard;
bool calibrated;
int thresholds[6];
int maxReadings[6];

int AirState[6];

char AIR_KEYS[6] = { // 键值列表
    AIR_KEY_0, AIR_KEY_1, AIR_KEY_2, AIR_KEY_3, AIR_KEY_4, AIR_KEY_5};

// 初始化天键
void airSetup() {
  calibrated = false;
  for (int i = 0; i < 6; i++) {
    maxReadings[i] = 0;
    thresholds[i] = 0;
    pinMode(AIR_RX_PINS[i], INPUT);
    pinMode(AIR_TX_PINS[i], OUTPUT);
    AirState[i] = 0;
  }
  airKeyboard.begin();
}

void turnOffLight() { // 关闭发射函数
  for (int i = 0; i < 6; i++) {
    digitalWrite(AIR_TX_PINS[i], LOW);
  }
}

// 红外发射轮询
void changeLight(int light) {
  if(light >= 0 && light <= 5) {
    digitalWrite(AIR_TX_PINS[light], HIGH);
  } else {
    turnOffLight();
  }
}

// 获取开灯数值函数
int getValue(int sensor) {
  changeLight(sensor);                    // 开灯
  delayMicroseconds(AIR_LED_DELAY / 2);   // 加上延迟保证完全开启
  return analogRead(AIR_RX_PINS[sensor]); // 读数值
}

// 返回Air状态
bool getSensorState(int sensor) {
  turnOffLight();                                   // 关灯
  delayMicroseconds(AIR_LED_DELAY / 2);             // 加上延迟保证完全关闭
  int enviroment = analogRead(AIR_RX_PINS[sensor]); // 读取环境数值
  int value = getValue(sensor);                     // 读取开灯数值

  // 把读数减去环境数值和校准数值比较后返回布尔值，+200为防止负数的比较
  return (enviroment - value + 200) < (thresholds[sensor] + 200);
}

void airCalibrate() { // 校准接收端
  // 跳过开头采样
  Serial.println("[INFO] Air keys are calibrating...");
  for (int i = 0; i < SKIP_SAMPLES; i++) {
    for (int sensor = 0; sensor < 6; sensor++) {
      getValue(sensor);
      turnOffLight();
    }
  }
  // 开始校准
  for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
    for (int sensor = 0; sensor < 6; sensor++) {
      turnOffLight();                                   // 关灯
      delayMicroseconds(AIR_LED_DELAY / 2);             // 加上延迟保证完全关闭
      int enviroment = analogRead(AIR_RX_PINS[sensor]); // 读取环境数值
      int value = enviroment - getValue(sensor);
      // 记录最高读数
      if (value > maxReadings[sensor]) {
        maxReadings[sensor] = value;
      }
    }
  }

  Serial.print("[INFO] Calibrated MaxReadings: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(maxReadings[i]);
    Serial.print(" ");
  }
  Serial.println();

  for (int i = 0; i < 6; i++) { // 通过敏感度生成阈值
    thresholds[i] = (AIR_INPUT_SENSITIVITY * maxReadings[i]);
  }
  calibrated = true; // 完成校准
  Serial.println("[INFO] Air keys calibrated!");
}

// 天键检测
void airLoop() {
  if (calibrated) {
    for (int i = 0; i < 6; i++) {
      if (getSensorState(i)) {
        // NKROKeyboard.press(AIR_KEYS[i]);
        airKeyboard.addKey(AIR_KEYS[i]);
        airKeyboard.sendKey();
        AirState[i] = 1;
      }
      else if (!getSensorState(i)) {
        // NKROKeyboard.release(AIR_KEYS[i]);
        airKeyboard.delKey(AIR_KEYS[i]);
        airKeyboard.sendKey();
        AirState[i] = 0;
      }
    }
  }
}

void airDebug() {
  Serial.println("--------------------------------");
  for (int i = 0; i < 6; i++) {
    // turnOffLight();//关灯
    // delayMicroseconds(AIR_LED_DELAY/2);
    digitalWrite(AIR_TX_PINS[i], HIGH);
    // changeLight(i);// 开灯
    // delayMicroseconds(AIR_LED_DELAY/2);// 加上延迟保证完全开启
    Serial.print("ID: ");
    Serial.print(i);
    Serial.print("\t");
    int value = analogRead(AIR_RX_PINS[i]);
    Serial.print("Value: ");
    Serial.print(value);
    Serial.print("\t");
    Serial.print("Thresholds: ");
    Serial.print(thresholds[i]);
    Serial.println();
    digitalWrite(AIR_TX_PINS[i], LOW);
  }
  delay(400);
}