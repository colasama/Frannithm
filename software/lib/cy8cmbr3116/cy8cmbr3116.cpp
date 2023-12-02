#include "cy8cmbr3116.h"

void cyInit()
{
  // 用于初始化I2C地址，只需要设置一次即可
  // cySetup(0x38, aConfigData);
  // cySetup(0x37, bConfigData);
  // 正常的写入设置
  cySetup(0x37, aConfigData);
  cySetup(0x38, bConfigData);
}

bool cySetup(uint8_t initaddr, const uint8_t configData[])
{
  int writeerr = 0;
  // Start wire
  Wire.begin();

  // Write dummy data
  Wire.beginTransmission(initaddr); // transmit to device #0x37
  Wire.write(0x00);                 // sends Offset byte
  Wire.write(0xFF);
  Wire.endTransmission(); // stop transmitting

  Wire.beginTransmission(initaddr); // transmit to device #0x37
  Wire.write(0x88);                 // sends Offset byte
  Wire.endTransmission();           // stop transmitting

  // Arduino function can send only 31 bytes of data
  // So whole frame is send in chunks
  // from [0] to [30]
  Wire.beginTransmission(initaddr);
  Wire.write(REGMAP_ORIGIN); // 0
  size_t sendedSize = Wire.write(&configData[0], 31);
  Wire.endTransmission();

  writeerr = Wire.getWriteError();

  if (writeerr == 0)
  {
    Serial.print("First packet sended\n");
  }

  // from [31] to [61]
  Wire.beginTransmission(initaddr);
  Wire.write(BUTTON_LBR); // 31
  sendedSize = Wire.write(&configData[31], 31);
  Wire.endTransmission();

  writeerr = Wire.getWriteError();

  if (writeerr == 0)
  {
    Serial.print("Second packet sended\n");
  }

  // from [62] to [92]
  Wire.beginTransmission(initaddr);
  Wire.write(BUZZER_CFG); // 62
  sendedSize = Wire.write(&configData[62], 31);
  Wire.endTransmission();

  writeerr = Wire.getWriteError();

  if (writeerr == 0)
  {
    Serial.print("Third packet sended\n");
  }

  // from [93] to [123]
  Wire.beginTransmission(initaddr);
  Wire.write(SLIDER_CFG); // 93
  sendedSize = Wire.write(&configData[93], 31);
  Wire.endTransmission();

  writeerr = Wire.getWriteError();

  if (writeerr == 0)
  {
    Serial.print("Fourth packet sended\n");
  }

  // from [124] to [127]
  Wire.beginTransmission(initaddr);
  Wire.write(0x7C); // 124
  sendedSize = Wire.write(&configData[124], 4);
  Wire.endTransmission();

  writeerr = Wire.getWriteError();

  if (writeerr == 0)
  {
    Serial.print("Fifth packet sended\n");
  }

  /*
  Write 0x02 to 0x86
  Info from datasheet
  The device calculates a CRC checksum over the configuration data in this register map and
  compares the result with the content of CONFIG_CRC. If the two values match, the device saves
  the configuration and the CRC checksum to nonvolatile memory.
  */
  Wire.beginTransmission(initaddr); // transmit to device #0x37
  Wire.write(CTRL_CMD);
  Wire.write(SAVE_CHECK_CRC);
  Wire.endTransmission(); // stop transmitting

  delay(300);

  // Reset
  Wire.beginTransmission(initaddr);
  Wire.write(CTRL_CMD);
  Wire.write(SW_RESET);
  Wire.endTransmission(); // stop transmitting
  Serial.print("[INFO] CY8 at 0x");
  Serial.print(initaddr, HEX);
  Serial.print(" configurated.\n");
  delay(200);
  return true;
}

void cyReset(uint8_t slaveaddr)
{
  // Reset
  Wire.beginTransmission(slaveaddr);
  Wire.write(CTRL_CMD);
  Wire.write(SW_RESET);
  Wire.endTransmission();

  Wire.beginTransmission(slaveaddr);
  Wire.write(0x88);
  Wire.endTransmission();
  Wire.requestFrom(slaveaddr, (uint8_t)1);
}

uint16_t cyGetRawButtonStatus(uint8_t slaveaddr)
{
  uint8_t rawData[2];
  Wire.beginTransmission(slaveaddr);
  Wire.write(BUTTON_STATUS);
  Wire.endTransmission();

  int i = 0;
  Wire.requestFrom(slaveaddr, (uint8_t)2); // 2 bytes for whole BUTTON_STATUS
  while (Wire.available())
  {
    rawData[i] = Wire.read();
    i++;
  }
  return (uint16_t)rawData[0] << 8 | rawData[1];
}

void cyDebugAllButtonStatus()
{
  uint16_t rawData = cyGetRawButtonStatus(0x37);
  Serial.print(0x37, HEX);
  Serial.print(" Status: ");
  for (int i = 0; i < 16; i++)
  {
    Serial.print(i);
    Serial.print(": ");
    bool temp = rawData & (1 << i);
    Serial.print(temp);
    Serial.print("\t");
  }

  rawData = cyGetRawButtonStatus(0x38);
  Serial.print(0x38, HEX);
  Serial.print(" Status: ");
  for (int i = 0; i < 16; i++)
  {
    Serial.print(i);
    Serial.print(": ");
    bool temp = rawData & (1 << i);
    Serial.print(temp);
    Serial.print("\t");
  }
  Serial.println();
}