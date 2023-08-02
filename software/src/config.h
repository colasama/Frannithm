#ifndef CONFIG_H
#define CONFIG_H

// Air Keys
#define AIR_KEY_0 KEYPAD_0
#define AIR_KEY_1 KEYPAD_1
#define AIR_KEY_2 KEYPAD_2
#define AIR_KEY_3 KEYPAD_3
#define AIR_KEY_4 KEYPAD_4
#define AIR_KEY_5 KEYPAD_5

// Air PIN in Pro Micro
#define AIR_TX_PIN_0 5
#define AIR_TX_PIN_1 A7
#define AIR_TX_PIN_2 7
// #define AIR_TX_PIN_3 16
// TODO: 由于PIN3暂时无法使用，先以PIN2代替
#define AIR_TX_PIN_3 7
#define AIR_TX_PIN_4 14
#define AIR_TX_PIN_5 15

#define AIR_RX_PIN_0 A3
#define AIR_RX_PIN_1 A2
#define AIR_RX_PIN_2 A1
#define AIR_RX_PIN_3 A0
#define AIR_RX_PIN_4 A10
#define AIR_RX_PIN_5 A9

// Air Config
#define AIR_LED_DELAY 96            // 轮询延迟
#define AIR_INPUT_SENSITIVITY 0.3   // 红外敏感度
#define CALIBRATION_SAMPLES 256     // 校准采样数
#define SKIP_SAMPLES 256            // 校准跳过采样数4

#define IR_ACTIVATION 300           // 默认红外触发阈值(0-8191)

// Touch Config
#define CA_ADDR 0x5A // MPR121 ADDR
#define CB_ADDR 0x5B
#define CC_ADDR 0x5C

#define SLIDER_THRESHOLDS 45        // 默认触摸触发阈值
#endif