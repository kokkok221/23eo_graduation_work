#include "constants.h"


// mpu6886の設定値
const uint8_t dlpfSetting = 7;
/*
 * MPU6886 デジタル・ローパス・フィルタ（DLPF）設定
 * CONFIG レジスタの DLPF_CFG[2:0] ビットの設定値と対応するフィルタの特性。
 *
 * DLPF_CFG | 加速度センサの帯域幅 (Hz) | ジャイロセンサの帯域幅 (Hz) | 出力データレート (kHz)
 * -------------------------------------------------------------------------------------
 * 0        | 218.1                    | 256.0                     | 8.000
 * 1        | 218.1                    | 188.0                     | 1.000
 * 2        | 99.0                     | 98.0                      | 1.000
 * 3        | 44.8                     | 42.0                      | 1.000
 * 4        | 21.2                     | 20.0                      | 1.000
 * 5        | 10.2                     | 10.0                      | 1.000
 * 6        | 5.1                      | 5.0                       | 1.000
 * 7        | フィルタ適用なし (RAWデータ出力)
 */


// ファイル名の定義
const char* LOG_FILENAME = "/yourFavorate_LOG.csv";

// ピン番号の定義
const int MOTOR_CW = 27;
const int MOTOR_CCW = 19;
const int CW_CHANNEL = 0;
const int CCW_CHANNEL = 1;
const int MOTOR_PLT_LAMP = 13;
const int PLT_LAMP_CHANNEL = 2;

// PID制御の初期パラメータの定義
const float P_P = 850.0;
const float P_I = 10.0;
const float P_D = 18.0;
const float TARGET = 82.0;
const float W_P = 1.0;
const float W_I = 0.0;
const float W_D = 0.0;
const float ANTI_WINDUP_GAIN = 0.1;
const int MOTOR_OFFSET = 30;

// その他の定数の定義
//BUFFER_SIZEはconstexprを使用しconstants.hで定義
