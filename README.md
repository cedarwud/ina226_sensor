# INA226 Current Sensor - Arduino Project

## 概要
此 Arduino 專案使用 **INA226 電流/功率感測器** 來監控兩個設備的實時電壓、電流和功率數據，並以 JSON 格式輸出到序列監視器。INA226 是一款高精度的 I2C 感測器，適合測量電壓、電流和功率。

## 功能描述
1. 初始化兩個 INA226 感測器，分別通過不同的 I2C 地址與 Arduino 通訊。
2. 從每個感測器讀取：
   - **電壓 (Voltage)**：單位為伏特 (V)。
   - **電流 (Current)**：單位為毫安培 (mA)。
   - **功率 (Power)**：單位為瓦特 (W)。
3. 將讀取到的數據組成 JSON 格式，方便進一步處理。
4. 每秒更新並輸出數據到序列監視器。

## 目標硬體
- **Arduino 主控板**（支援 I2C，例：Uno、Mega、Leonardo）
- **INA226 感測器模組** x 2
- 連接用杜邦線

## 程式結構
### 初始化
- `INA226_WE` 類別創建兩個感測器對象，分別綁定 I2C 地址。
- 初始化每個感測器，若初始化失敗，會在序列監視器顯示錯誤並停止程式執行。

### 數據讀取與處理
- 讀取每個感測器的匯流排電壓 (Bus Voltage)、電流 (Current)、以及功率 (Power)。
- 數據整合為 JSON 格式：
  ```json
  {
    "sensor_1": {
      "voltage": 3.3,
      "current": 120.5,
      "power": 0.398,
      "channel": 0
    },
    "sensor_2": {
      "voltage": 5.0,
      "current": 100.0,
      "power": 0.5,
      "channel": 1
    }
  }
  ```

### 輸出與更新
- 每秒輸出一次更新數據，JSON 格式便於整合至本地伺服器或其他應用。

## 程式碼說明
以下為核心功能：

1. **初始化感測器與通訊**
   ```cpp
   INA226_WE ina226_1 = INA226_WE(I2C_ADDRESS_1);
   INA226_WE ina226_2 = INA226_WE(I2C_ADDRESS_2);

   if (!ina226_1.init()) {
       Serial.println("Failed to init INA226 #1. Check your wiring.");
       while (1) {}
   }

   if (!ina226_2.init()) {
       Serial.println("Failed to init INA226 #2. Check your wiring.");
       while (1) {}
   }
   ```

2. **讀取數據**
   ```cpp
   float voltage_V1 = ina226_1.getBusVoltage_V();
   float current_A1 = ina226_1.getCurrent_mA();
   float power_W1 = ina226_1.getBusPower();

   float voltage_V2 = ina226_2.getBusVoltage_V();
   float current_A2 = ina226_2.getCurrent_mA();
   float power_W2 = ina226_2.getBusPower();
   ```

3. **生成 JSON 並輸出**
   ```cpp
   String jsonData = "{\"sensor_1\": {\"voltage\":";
   jsonData += voltage_V1;
   jsonData += ", \"current\":";
   jsonData += current_A1;
   jsonData += ", \"power\":";
   jsonData += power_W1;
   jsonData += ", \"channel\":0}, \"sensor_2\": {\"voltage\":";
   jsonData += voltage_V2;
   jsonData += ", \"current\":";
   jsonData += current_A2;
   jsonData += ", \"power\":";
   jsonData += power_W2;
   jsonData += ", \"channel\":1}}";

   Serial.println(jsonData);
   ```

4. **每秒更新數據**
   ```cpp
   delay(1000);  // 延遲 1 秒後重新讀取
   ```

## 使用方式
1. **硬體接線**:
   - 將 INA226 感測器的 SDA 和 SCL 分別連接到 Arduino 的對應 I2C 腳位：
     - Uno/Nano: SDA=A4, SCL=A5
     - Mega: SDA=20, SCL=21
     - Leonardo: SDA=2, SCL=3
   - VCC 和 GND 連接到 Arduino。
   - 確保兩個感測器的 I2C 地址不同，否則會發生衝突。

2. **軟體準備**:
   - 安裝 [INA226_WE 庫](https://github.com/SV-Zanshin/INA226_WE)（可通過 Arduino 庫管理器安裝）。
   - 將此程式碼載入 Arduino IDE，並上傳至 Arduino 主控板。

3. **查看數據**:
   - 打開 Arduino IDE 的序列監視器，設定波特率為 `9600`。
   - 每秒查看最新的電壓、電流和功率數據。

## 注意事項
- 確保 INA226 感測器的硬體接線正確，尤其是 SDA 和 SCL 的連接。
- 如果無法初始化感測器，可能是 I2C 地址設定有誤，或接線問題。
- 根據感測器的配置，可能需要額外校準。

## 範例輸出
以下為可能的序列監視器輸出範例：
```
{"sensor_1": {"voltage":3.3, "current":120.5, "power":0.398, "channel":0}, "sensor_2": {"voltage":5.0, "current":100.0, "power":0.5, "channel":1}}
```

## 延伸功能建議
1. **數據上傳**: 將 JSON 數據發送至本地伺服器或雲端進行存儲與分析。
2. **更多通道支持**: 增加其他 INA226 感測器以同時監測更多設備。
3. **數據可視化**: 使用圖表或儀表板展示實時數據。