#include "main.h"
#include "sensor.h"
#include "pid.h"
#include "motor.h"
#include "logging.h"
#include "display.h"
#include "network.h"
#include "constants.h"

//mpu 6886 values
float accX = 0.0F, accY = 0.0F, accZ = 0.0F;
float gyroX = 0.0F, gyroY = 0.0F, gyroZ = 0.0F;
float temp = 0.0F;
float pitch = 0.0F, roll = 0.0F, yaw = 0.0F;

// display values
int countS = 0, i = 0;
int button_Param = 0; // 0: P_p, 1: I_p, 2: D_p, 3: P_w, 4: I_w 5:target
int log_state = 0; //0:SDfail, 1:connecting, 2:success, 3:logging, 4:sending
bool updateParamFlag = false; // touchPram更新用フラグ
bool updateLogStateFlag = false; // logState更新用フラグ

// PID
float p_p = P_P;
float p_i = P_I;
float p_d = P_D;
float target = TARGET;
float P = 0, I = 0, D = 0, prepP = 0;
float error = 0, power = 0, clamp = 0;
uint8_t configValue = 0;

// PWM
float antiWindupGain = ANTI_WINDUP_GAIN;
int Motor_offset = MOTOR_OFFSET;
float dt = 0.0;
unsigned long preTime = 0;
int Duty = 0;
bool motorOutputEnabled = false;

// log variables
float cycleTimeCore0 = 0.0f;       // サイクル時間（秒）
unsigned long lastCycleTimeCore0 = 0; // 前回のサイクル時間記録（ミリ秒）
float cycleTimeCore1_1 = 0.0f;       // サイクル時間（秒）
unsigned long lastCycleTimeCore1_1 = 0; // 前回のサイクル時間記録（ミリ秒）
float cycleTimeCore1_2 = 0.0f;       // サイクル時間（秒）
unsigned long lastCycleTimeCore1_2 = 0; // 前回のサイクル時間記録（ミリ秒）

float core0TaskLoadBuffer[BUFFER_SIZE];
float core0CycleTimeBuffer[BUFFER_SIZE];
float core1_1TaskLoadBuffer[BUFFER_SIZE];
float core1_1CycleTimeBuffer[BUFFER_SIZE];
float core1_2TaskLoadBuffer[BUFFER_SIZE];
float core1_2CycleTimeBuffer[BUFFER_SIZE];
int core0BufferIndex = 0;
int core1_1BufferIndex = 0;
int core1_2BufferIndex = 0;

// タスクのハンドル
TaskHandle_t SensorControlTaskHandle = NULL;
TaskHandle_t LogControlTaskHandle = NULL;
TaskHandle_t DisplayTaskHandle = NULL;



//////////////////////////////////////////////
// 各Coreの処理内容はlogging.cpp内の定義を参照//
/////////////////////////////////////////////

// コア0: センサー取得と制御タスク
void SensorControlTask(void *pvParameters) {
    for (;;) {
        Core0_PLS();
        vTaskDelay(pdMS_TO_TICKS(10)); // 10ms周期
    }
}


// コア1: ログ記録タスク
void LogControlTask(void *pvParameters) {
    for (;;) {
        Core1_logTask();
        vTaskDelay(pdMS_TO_TICKS(10)); // 10ms
    }
}

// コア1: ディスプレイタスク
void updateDisplayTask(void *pvParameters) {
    for (;;) {
        Core1_displayTask();
        vTaskDelay(pdMS_TO_TICKS(100)); // 100ms周期で更新
    }
}


void setup() {
    Serial.begin(115200);
    // M5Stack初期化
    M5.begin();
    // mpu6886  init & DLPF設定
    M5.IMU.Init();
    initMPU6886(dlpfSetting);
    debug_MPU6886Filter();
    // ディスプレイ初期化
    initDisplay();
    // モータピン初期化
    initMotor();
    // ログ(SDCard)初期化
    initLogging();
    // WiFi
    connectWiFi("YourSSID", "YourPassward");
    // HTTP server
    setupServer();

    // タスクの作成
    xTaskCreatePinnedToCore(SensorControlTask, "Sensor & Control", 4096, NULL, 3, &SensorControlTaskHandle, 0);
    xTaskCreatePinnedToCore(LogControlTask, "Display & Logging", 4096, NULL, 2, &LogControlTaskHandle, 1);
    xTaskCreatePinnedToCore(updateDisplayTask, "Display Update", 4096, NULL, 1, &DisplayTaskHandle, 1);
}

// メインループ
void loop() {
    // メインループは空でOK
    // タスクで処理する
}
