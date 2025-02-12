# include "main.h"
# include "pid.h"

// dtの更新
void updateDeltaTime() {
    dt = (micros() - preTime) / 1000000.0; // 秒単位の処理時間
    preTime = micros(); // 現在の時間を記録
}

void PD_Pendulum() {
    M5.IMU.getAhrsData(&pitch, &roll, &yaw);
    updateDeltaTime(); //dtをwheelと共有する
    error = target - roll;
    // P control
    P = error / 90; // -90~90を取るので90で割って-1.0~1.0にする
    // D control
    D = (P - prepP) / dt;
		//power
    power = ((p_p * P) + (p_d * D));

    prepP = P;
}

float PID_Pendulum() {
    M5.IMU.getAhrsData(&pitch, &roll, &yaw);
    updateDeltaTime(); //dtをwheelと共有する
    // P control
    error = target - roll;
    P = error / 90; // -90~90を取るので90で割って-1.0~1.0にする
    // I control
    I += P * dt;
    // D control
    D = (P - prepP) / dt;

    power = ((p_p * P) + (p_i * I) + (p_d * D));

    clamp = constrain(power, -255, 255);

    // アンチワインドアップのためのバック計算
    I += (clamp - power) * antiWindupGain;

    prepP = P;
    return clamp;
}
