#include "logging.h"
#include "display.h"
#include "pid.h"
#include "motor.h"
#include "sensor.h"
#include "main.h"
#include "network.h"
#include "constants.h"

// ログファイル名
File dataFile;

///////////////////////////////////////////////////////////////////
////////////////////// Core0タスクのメイン処理 /////////////////////
//////////////////////////////////////////////////////////////////
void Core0_PLS() { //PLS : Phase Lock Loop Servo
    unsigned long taskStart = micros(); // タスク処理開始

        //Calculate PID
        PID_Pendulum();

        // PWM信号生成
        PWM_pid();

    unsigned long taskEnd = micros(); // タスク処理終了

    // タスクの実行時間を計算（マイクロ秒単位 -> ミリ秒単位）
    float taskExecutionTime = (taskEnd - taskStart) / 1000.0;

    // タスク周期を計算（秒単位）
    float taskCycleTime = calculateTaskCycleTime(lastCycleTimeCore0);

    // CPU負荷率（％）と処理時間（ｓ）をバッファへ格納
    core0TaskLoadBuffer[core0BufferIndex] = calculateTaskCPULoad(taskExecutionTime, taskCycleTime * 1000.0); // ミリ秒単位に変換
    core0CycleTimeBuffer[core0BufferIndex] = taskCycleTime; // そのまま秒単位で保存

    // バッファインデックスを更新＆管理
    core0BufferIndex = (core0BufferIndex + 1) % BUFFER_SIZE;
}

///////////////////////////////////////////////////////////////////
////////////////////// Core1_1タスク（ログ処理） ///////////////////
//////////////////////////////////////////////////////////////////
void Core1_logTask() {
        unsigned long taskStart = micros();

        // ログ記録
        sensor_logToSDCard();
        //logToSDCard();

        unsigned long taskEnd = micros();

    // タスクの実行時間を計算（マイクロ秒単位 -> ミリ秒単位）
    float taskExecutionTime = (taskEnd - taskStart) / 1000.0;

    // タスク周期を計算（秒単位）
    float taskCycleTime = calculateTaskCycleTime(lastCycleTimeCore1_1);

    // CPU負荷率（％）と処理時間（ｓ）をバッファへ格納
    core1_1TaskLoadBuffer[core1_1BufferIndex] = calculateTaskCPULoad(taskExecutionTime, taskCycleTime * 1000.0); // ミリ秒単位に変換
    core1_1CycleTimeBuffer[core1_1BufferIndex] = taskCycleTime; // そのまま秒単位で保存

    // バッファインデックスを更新＆管理
    core1_1BufferIndex = (core1_1BufferIndex + 1) % BUFFER_SIZE;
}

///////////////////////////////////////////////////////////////////
////////////////////// Core1_2タスク（ディスプレイ更新） ////////////
//////////////////////////////////////////////////////////////////
void Core1_displayTask() {
        unsigned long taskStart = micros();

        // ボタン状態更新
        M5.update();
        // ディスプレイ更新
        updateDisplay();
        // ログ開始
        push_B_on();
        // ログ終了
        push_B_off();
        // ネットワーク処理
        server.handleClient();

        unsigned long taskEnd = micros();

    // タスクの実行時間を計算（マイクロ秒単位 -> ミリ秒単位）
    float taskExecutionTime = (taskEnd - taskStart) / 1000.0;

    // タスク周期を計算（秒単位）
    float taskCycleTime = calculateTaskCycleTime(lastCycleTimeCore1_2);

    // CPU負荷率（％）と処理時間（ｓ）をバッファへ格納
    core1_2TaskLoadBuffer[core1_2BufferIndex] = calculateTaskCPULoad(taskExecutionTime, taskCycleTime * 1000.0); // ミリ秒単位に変換
    core1_2CycleTimeBuffer[core1_2BufferIndex] = taskCycleTime; // そのまま秒単位で保存

    // バッファインデックスを更新＆管理
    core1_2BufferIndex = (core1_2BufferIndex + 1) % BUFFER_SIZE;
}

// SDカードの初期化
void initLogging() {
    if (!SD.begin()) {
        log_state = 0;
        Serial.println("Failed to initialize SD card!");
    }
    log_state = 1;
}

// センサ用ログ記録関数
void sensor_logToSDCard() {
    // 固定パラメータとヘッダーを記録するためのフラグ
    static bool headerWritten = false;

    if (dataFile) {
        // 初回記録時に固定パラメータとヘッダーを記録
        if (!headerWritten) {
            // 固定パラメータを記録
            dataFile.println("Parameter, Value");
            dataFile.printf("P_p, %.2f\n", p_p);
            dataFile.printf("I_p, %.2f\n", p_i);
            dataFile.printf("D_p, %.2f\n", p_d);
            dataFile.printf("AWP, %.1f\n", ANTI_WINDUP_GAIN);
            dataFile.printf("Target, %.2f\n", target);
            dataFile.printf("Motor_OFFSET, %d\n", Motor_offset);
            dataFile.printf("mpu6886_DLPF, %u\n", dlpfSetting);
            // 空行で区切り、ヘッダーを記録
            dataFile.println();
            dataFile.println("Time(ms), roll,error, Duty, Power, P, I, D, Core0_Load(%), Core0_Cycle(s), Core1_1_Load(%), Core1_1_Cycle(s), Core1_2_Load(%), Core1_2_Cycle(s)");
            // ヘッダー記録済み
            headerWritten = true;
        } else {
            // ログエントリを格納する一時変数
            String logEntry;

            // 現在時刻を記録
            logEntry += String(millis()) + ",";

            // センサデータを取得してログエントリに追加
            logEntry += String(roll, 2) + ",";   // Roll角
            logEntry += String(error, 2) + ",";   // error
            logEntry += String(Duty) + ",";   // motor Duty
            logEntry += String(clamp, 2) + ",";  // clamp
            logEntry += String(P, 2) + ",";  // pid P
            logEntry += String(I, 2) + ",";  // pi
            logEntry += String(D, 2) + ",";  // pid D

////////////////////////////////////////////////////////////////////
//各タスクの処理時間及び負荷率を記録する場合は以下のコメントアウトを解除//
////////////////////////////////////////////////////////////////////
/*
            // Core 0のタスク負荷データを追加
            if (core0BufferIndex > 0) {
                logEntry += String(core0TaskLoadBuffer[(core0BufferIndex - 1) % BUFFER_SIZE], 2) + ",";
                logEntry += String(core0CycleTimeBuffer[(core0BufferIndex - 1) % BUFFER_SIZE], 3) + ",";
            } else {
                logEntry += ",,";
            }

            // Core 1_1のタスク負荷データを追加
            if (core1_1BufferIndex > 0) {
                logEntry += String(core1_1TaskLoadBuffer[(core1_1BufferIndex - 1) % BUFFER_SIZE], 2) + ",";
                logEntry += String(core1_1CycleTimeBuffer[(core1_1BufferIndex - 1) % BUFFER_SIZE], 3);
            } else {
                logEntry += ",,";
            }

            // Core 1_2のタスク負荷データを追加
            if (core1_2BufferIndex > 0) {
                logEntry += String(core1_2TaskLoadBuffer[(core1_2BufferIndex - 1) % BUFFER_SIZE], 2) + ",";
                logEntry += String(core1_2CycleTimeBuffer[(core1_2BufferIndex - 1) % BUFFER_SIZE], 3);
            } else {
                logEntry += ",,";
            }

*/

            // 改行を追加
            logEntry += "\n";
            // バッファリングされた書き込み
            logToSDCardBuffered(logEntry);
        }
    }
}

// バッファリングされたログデータをSDカードに書き込む
void logToSDCardBuffered(const String& data) {
    static String logBuffer = "";        // バッファとして使用する文字列
    static unsigned long lastLogTime = 0; // 最後に書き込んだ時刻

    // データをバッファに追加
    logBuffer += data;

    // バッファが一定サイズ以上または一定時間経過した場合に書き込む
    if (logBuffer.length() >= 512 || millis() - lastLogTime >= 500) {
        if (dataFile) {
            dataFile.print(logBuffer); // バッファ全体をSDカードに書き込み
            dataFile.flush();         // 書き込みを確定
            logBuffer = "";           // バッファをクリア
            lastLogTime = millis();   // 書き込み時刻を更新
        }
    }
}


// ログ記録を開始する
void push_B_on() {
    if (B_on.wasPressed()) {
            // モーター出力をenable
            motorOutputEnabled = true;
            // パイロットランプを点灯
            ledcWrite(PLT_LAMP_CHANNEL, 150);
            // ログ遷移状態を更新
            log_state = 3;
            // ファイルを開く
            dataFile = SD.open(LOG_FILENAME, FILE_WRITE);
            // ファイルが開けなかった場合の処理
            if (!dataFile) {
                log_state = 4;
                Serial.println("[push_B_log] Failed to open log file!");
                 return;
            }
     }
}

// ログ記録を終了
void push_B_off() {
    // ボタンBが押された場合
    if (B_off.wasPressed() && dataFile) {
        // ファイルを閉じる
        dataFile.close();
        // ログ遷移状態を更新
        log_state = 5;
        // モータ出力をdisable
        motorOutputEnabled = false;
        // パイロットランプを消灯
        ledcWrite(PLT_LAMP_CHANNEL, 0);
    }
}

// CPU負荷を計算する関数
float calculateTaskCPULoad(unsigned long executionTime, unsigned long taskPeriod) {
    return (executionTime / (float)taskPeriod) * 100.0; // 負荷率を計算
}

// タスク周期を計算する関数
float calculateTaskCycleTime(unsigned long &lastTime) {
    unsigned long currentTime = micros();
    float cycleTime = (currentTime - lastTime) / 1000000.0; // マイクロ秒 -> 秒
    lastTime = currentTime;
    return cycleTime;
}
