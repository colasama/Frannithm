#ifndef CONFIG_H
#define CONFIG_H

// 天键键值
#define AIR_KEY_0 '0'
#define AIR_KEY_1 '1'
#define AIR_KEY_2 '2'
#define AIR_KEY_3 '3'
#define AIR_KEY_4 '4'
#define AIR_KEY_5 '5'

// S2 中的发射引脚
#define AIR_TX_PIN_0 33
#define AIR_TX_PIN_1 34
#define AIR_TX_PIN_2 18
#define AIR_TX_PIN_3 21
#define AIR_TX_PIN_4 16
#define AIR_TX_PIN_5 17

// S2 中的接收引脚
#define AIR_RX_PIN_0 2
#define AIR_RX_PIN_1 3
#define AIR_RX_PIN_2 4
#define AIR_RX_PIN_3 5
#define AIR_RX_PIN_4 6
#define AIR_RX_PIN_5 7

// 灯光控制引脚
#define LIGHT_PIN 9
// Air Config
#define AIR_LED_DELAY 96            // 轮询延迟
#define AIR_INPUT_SENSITIVITY 0.3   // 红外敏感度
#define CALIBRATION_SAMPLES 256     // 校准采样数
#define SKIP_SAMPLES 256            // 校准跳过采样数

#define IR_ACTIVATION 300           // 默认红外触发阈值(0-8191)

// Touch Config
#define CA_ADDR 0x5A // MPR121 ADDR
#define CB_ADDR 0x5B
#define CC_ADDR 0x5C

#define TOUCH_THRESHOLD 45        // 默认触摸触发阈值，越低越灵敏

#endif