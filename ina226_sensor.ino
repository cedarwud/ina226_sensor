#include <Wire.h>             // 引入 Wire 庫，用於 I2C 通訊
#include <INA226_WE.h>        // 引入 INA226 驅動庫

#define I2C_ADDRESS_1 0x40    // 定義第一個 INA226 感測器的 I2C 地址
#define I2C_ADDRESS_2 0x41    // 定義第二個 INA226 感測器的 I2C 地址

// 創建 INA226 感測器物件
INA226_WE ina226_1 = INA226_WE(I2C_ADDRESS_1); 
INA226_WE ina226_2 = INA226_WE(I2C_ADDRESS_2);

void setup() {
  Serial.begin(9600);         // 初始化序列通訊，波特率設置為 9600
  Wire.begin();               // 初始化 I2C 通訊

  // 初始化第一個 INA226 感測器
  if (!ina226_1.init()) {
    Serial.println("Failed to init INA226 #1. Check your wiring.");
    while (1) {}              // 如果初始化失敗，停止程式執行
  }

  // 初始化第二個 INA226 感測器
  if (!ina226_2.init()) {
    Serial.println("Failed to init INA226 #2. Check your wiring.");
    while (1) {}              // 如果初始化失敗，停止程式執行
  }

  Serial.println("INA226 Current Sensor - Data Push to Local Server");
}

void loop() {
  // 讀取第一個感測器的數據
  float voltage_V1 = ina226_1.getBusVoltage_V(); // 獲取匯流排電壓（伏特）
  float current_A1 = ina226_1.getCurrent_mA();   // 獲取電流（毫安培）
  float power_W1 = ina226_1.getBusPower();       // 獲取功率（瓦特）

  // 讀取第二個感測器的數據
  float voltage_V2 = ina226_2.getBusVoltage_V(); // 獲取匯流排電壓（伏特）
  float current_A2 = ina226_2.getCurrent_mA();   // 獲取電流（毫安培）
  float power_W2 = ina226_2.getBusPower();       // 獲取功率（瓦特）

  // 組裝 JSON 格式數據
  String jsonData = "{\"sensor_1\": {\"voltage\":";
  jsonData += voltage_V1;                       // 添加第一個感測器電壓
  jsonData += ", \"current\":";
  jsonData += current_A1;                       // 添加第一個感測器電流
  jsonData += ", \"power\":";
  jsonData += power_W1;                         // 添加第一個感測器功率
  jsonData += ", \"channel\":";
  jsonData += 0;                                // 第一個感測器的通道標識
  jsonData += "}, \"sensor_2\": {\"voltage\":";
  jsonData += voltage_V2;                       // 添加第二個感測器電壓
  jsonData += ", \"current\":";
  jsonData += current_A2;                       // 添加第二個感測器電流
  jsonData += ", \"power\":";
  jsonData += power_W2;                         // 添加第二個感測器功率
  jsonData += ", \"channel\":";
  jsonData += 1;                                // 第二個感測器的通道標識
  jsonData += "}}";

  // 將 JSON 數據輸出到序列監視器
  Serial.println(jsonData);

  delay(1000); // 每秒更新一次數據
}