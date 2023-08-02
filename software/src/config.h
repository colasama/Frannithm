#ifndef CONFIG_H
#define CONFIG_H

#include "Adafruit_MPR121.h"
// Air Key
#define AIR_KEY_0 '4'
#define AIR_KEY_1 '5'
#define AIR_KEY_2 '6'
#define AIR_KEY_3 '7'
#define AIR_KEY_4 '8'
#define AIR_KEY_5 '9'

// Air PIN in Pro Micro
#define AIR_TX_PIN_0 5
#define AIR_TX_PIN_1 A7
#define AIR_TX_PIN_2 7
#define AIR_TX_PIN_3 16
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

#define IR_ACTIVATION 300                // 默认红外触发阈值(0-8191)
//#define IR_UPDOWN 200                    // 默认红外上升下降阈值(0-8191)
#define IRDEBUG 0                        // 红外调试

// Touch Config
#define CA_ADDR 0x5A // MPR121 ADDR
#define CB_ADDR 0x5B
#define CC_ADDR 0x5C



#define SLIDER_THRESHOLDS 45             // 默认触摸触发阈值

// 开机时按下任意触摸键进入手套模式,反之进入空手模式
// 配置参考AN3890文档
#define SLIDERDEBUG 0                    // 滑条调试
// 空手模式
#define PressThresholds 0                // 按下触发阈值
#define ReleaseThresholds 0              // 释放触发阈值
#define MPR121_CHARGE_CURRENT 0x30       // 极板充电电流,设置越高灵敏度越高
#define MPR121_ENCODING_PERIOD 0x20      // 极板编码时间,具体设置见MPR121手册

// 手套模式
#define GlovesPressThresholds 0              // 手套模式按下触发阈值
#define GlovesReleaseThresholds 0            // 手套模式释放触发阈值
#define GlovesMPR121_CHARGE_CURRENT 0x3F     // 手套模式极板充电电流,设置越高灵敏度越高
#define GlovesMPR121_ENCODING_PERIOD 0x20    // 手套模式极板编码时间,具体设置见MPR121手册

#define AUTOCONFIG 1                         // 是否开启MPR121自动参数设置,1开启,0关闭

#endif