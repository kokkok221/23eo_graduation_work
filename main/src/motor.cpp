#include "motor.h"
#include "main.h"
#include "display.h"
#include "constants.h"
#include "sensor.h"


void PWM_pd() {
    // Motor PWM 255に制限
    Duty = constrain((int)power + Motor_offset, -255, 255);

    // モータフェールセーフ（前後30度以内で倒立動作）
    if (-30 < error && error < 30) {
      if (motorOutputEnabled){
        // 出力は正負でCWとCCWを切り替える
        ledcWrite(CCW_CHANNEL, ((power) < 0 ? 0 : abs(Duty)));
        ledcWrite(CW_CHANNEL, ((power) < 0 ? abs(Duty) : 0));
      } else {
        ledcWrite(CW_CHANNEL, 0);
        ledcWrite(CCW_CHANNEL, 0);
      }
    // 各変数をリセット（転倒と判断）
    } else {
      ledcWrite(CW_CHANNEL, 0);
      ledcWrite(CCW_CHANNEL, 0);
      Duty = 0;
      power = 0;
      motorOutputEnabled = false;
      ledcWrite(PLT_LAMP_CHANNEL, 0);
    }
}

void PWM_pid() {
    // Motor PWM 255に制限
    Duty = constrain((int)clamp + Motor_offset, -255, 255);
    // モータフェールセーフ（前後30度以内で倒立動作）
    if (-30 < error && error < 30) {
      if (motorOutputEnabled){
        // 出力は正負でCWとCCWを切り替える
        ledcWrite(CCW_CHANNEL, ((clamp) < 0 ? 0 : abs(Duty)));
        ledcWrite(CW_CHANNEL, ((clamp) < 0 ? abs(Duty) : 0));
      } else {
        ledcWrite(CW_CHANNEL, 0);
        ledcWrite(CCW_CHANNEL, 0);
      }
    // 各変数をリセット（転倒と判断）
    } else {
      ledcWrite(CW_CHANNEL, 0);
      ledcWrite(CCW_CHANNEL, 0);
      motorOutputEnabled = false;
      ledcWrite(PLT_LAMP_CHANNEL, 0);
      I = 0;
      clamp = 0;
      power = 0;
      Duty = 0;
    }
}

// モータ初期化（ピン番号はconstants.cppを参照）
void initMotor() {
  pinMode(MOTOR_CW, OUTPUT);
  pinMode(MOTOR_CCW, OUTPUT);
  pinMode(MOTOR_PLT_LAMP, OUTPUT);

  ledcSetup(CW_CHANNEL, 490, 8);
  ledcSetup(CCW_CHANNEL, 490, 8);
  ledcSetup(PLT_LAMP_CHANNEL, 490, 8);
  ledcAttachPin(MOTOR_CW, CW_CHANNEL);
  ledcAttachPin(MOTOR_CCW, CCW_CHANNEL);
  ledcAttachPin(MOTOR_PLT_LAMP, PLT_LAMP_CHANNEL);

  // M5 Stack起動時にモータ出力をOFF
  ledcWrite(CW_CHANNEL, 0);
  ledcWrite(CCW_CHANNEL, 0);
  ledcWrite(PLT_LAMP_CHANNEL, 0);
}

// デバッグ & テスト用
// PWM出力80でCWとCCWを5秒間隔で切り替える
void sensor_disturbance2() {
  if (motorOutputEnabled){
    static unsigned long lastSwitchTime = 0;
    static bool direction = true; // true: CW, false: CCW

    unsigned long currentTime = millis();

    // 5秒間隔で切り替え
    if (currentTime - lastSwitchTime >= 3000) {
        lastSwitchTime = currentTime;
        direction = !direction; // 方向を切り替え
    }

    // モーター出力
    if (direction) {
        ledcWrite(CW_CHANNEL, 50);
        ledcWrite(CCW_CHANNEL, 0);
    } else {
        ledcWrite(CW_CHANNEL, 0);
        ledcWrite(CCW_CHANNEL, 50);
    }
  } else {
    ledcWrite(CW_CHANNEL, 0);
    ledcWrite(CCW_CHANNEL, 0);
  }
}
