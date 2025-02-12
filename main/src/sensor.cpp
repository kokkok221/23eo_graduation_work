#include "main.h"
#include "sensor.h"
#include "constants.h"

//mpu6886 初期化 & DLPF設定
void initMPU6886(uint8_t dlpfSetting, int maxRetries) {
    if (dlpfSetting > 7) dlpfSetting = 7;
    for (int attempt = 1; attempt <= maxRetries; ++attempt) {
        Wire.beginTransmission(MPU6886_ADDR);
        Wire.write(MPU6886_REG_CONF);
        Wire.write(dlpfSetting & 0x07);
        if (Wire.endTransmission() == 0) {
            // 書き込み後の確認
            Wire.beginTransmission(MPU6886_ADDR);
            Wire.write(MPU6886_REG_CONF);
            Wire.endTransmission();
            Wire.requestFrom(MPU6886_ADDR, 1);
            if (Wire.available()) {
                uint8_t readValue = Wire.read();
                if ((readValue & 0x07) == dlpfSetting) {
                    Serial.printf("MPU6886 DLPF configuration successful on attempt %d.\n", attempt);
                    return; // 成功した場合は終了
                } else {
                    Serial.printf("Mismatch after setting DLPF: Written 0x%02X, Read 0x%02X\n", dlpfSetting, readValue);
                }
            } else {
                Serial.println("Failed to verify MPU6886 DLPF configuration.");
            }
        } else {
            Serial.printf("DLPF configuration attempt %d failed. Retrying...\n", attempt);
        }
        delay(100); // 次の試行までの待機
    }
    Serial.println("MPU6886 DLPF configuration failed after maximum retries.");
}

// mpu6886からデータを取得
void get_mpu6886() {
    M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
    M5.IMU.getAhrsData(&pitch, &roll, &yaw);
}

//mpu6886 デバッグ
void debug_MPU6886Filter() {
    Wire.beginTransmission(MPU6886_ADDR);
    Wire.write(MPU6886_REG_CONF);
    Wire.endTransmission();
    Wire.requestFrom(MPU6886_ADDR, 1);
    if (Wire.available()) {
        uint8_t dlpfSetting = Wire.read() & 0b111;
        Serial.printf("MPU6886 DLPF Setting: %u\n", dlpfSetting);
    } else {
        Serial.println("Failed to read MPU6886 filter settings.");
    }
}
