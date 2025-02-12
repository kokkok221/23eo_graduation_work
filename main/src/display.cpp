#include "WiFi.h"
#include "main.h"
#include "display.h"
#include "sensor.h"

// ボタンの定義
Button B_on(btn_HET-btn_SPC, btn_ySET, btn_WID, btn_HET, "ON");
Button B_off(btn_WID+btn_SPC, btn_ySET, btn_WID, btn_HET, "OFF");

// ボタンの初期化
void buttonInit() {
    M5.Lcd.drawRect(B_on.x, B_on.y, B_on.w, B_on.h, TFT_WHITE);
    M5.Lcd.drawString("ON", B_on.x + 34, B_on.y + 5, 2);
    M5.Lcd.drawRect(B_off.x + 20, B_off.y, B_off.w, B_off.h, TFT_WHITE);
    M5.Lcd.drawString("OFF", B_off.x + 48, B_off.y + 5, 2);
}

// ディスプレイの初期化
void initDisplay() {
    M5.Lcd.begin();
    M5.Lcd.clear();
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(GREEN,BLACK);
    M5.Lcd.setTextSize(2);
    buttonInit();
}

// ディスプレイの更新
void updateDisplay() {
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.printf("roll: %06.2f", roll);
    printPram();
    param_change();
    logState();
}

// 選択中のパラメータを表示
void printPram(){
    M5.Lcd.setCursor(0, 55);
    M5.Lcd.printf("Selected Param: %s",
        button_Param == 0 ? "P_p" :
        button_Param == 1 ? "I_p" :
        button_Param == 2 ? "D_p" :
        button_Param == 3 ? "AWP" :
        button_Param == 4 ? "Trg" :
        "OFF"
        );
    M5.Lcd.setCursor(0, 75);
    M5.Lcd.printf("Pp:%4.0f, Pi:%4.1f, Pd:%4.1f", p_p, p_i, p_d);
    M5.Lcd.setCursor(0, 95);
    M5.Lcd.printf("AP:%4.1f, Tg:%4.1f, oF:%4d", antiWindupGain, target, Motor_offset);
}

// ログ状態表示
void logState() {
    M5.Lcd.setCursor(0, 140);
    switch (log_state) {
        case 0:
            M5.Lcd.printf("log Init Failed!");
            break;
        case 1:
            M5.Lcd.printf("Connecting to Network...");
            break;
        case 2:
            M5.Lcd.printf("SD Ready. Press Log Button");
            break;
        case 3:
            M5.Lcd.printf("Logging in Progress.......");
            break;
        case 4:
            M5.Lcd.printf("File Write Failed!");
            break;
        default:
            M5.Lcd.printf("IP Address:%s", WiFi.localIP().toString().c_str());
            break;
    }
}

// パラメータ変更
void param_change() {
    // ボタンA: 次のパラメータを選択
    if (M5.BtnA.isPressed()) {
        button_Param = (button_Param + 1) % 7; // 全7パラメータの切り替え
    }

    // ボタンB: パラメータを増加
    if (M5.BtnB.isPressed()) {
        switch (button_Param) {
            case 0: p_p += 10; break;
            case 1: p_i += 1; break;
            case 2: p_d += 1; break;
            case 3: antiWindupGain += 0.1; break;
            case 4: target += 0.1; break;
            case 5: Motor_offset += 1; break;
        }
    }

    // ボタンC: パラメータを減少
    if (M5.BtnC.isPressed()) {
        switch (button_Param) {
            case 0: p_p -= 10; break;
            case 1: p_i -= 1; break;
            case 2: p_d -= 1; break;
            case 3: antiWindupGain -= 0.1; break;
            case 4: target -= 0.1; break;
            case 5: Motor_offset -= 1; break;
        }
    }
}

// メッセージ表示(デバッグ用、必要に応じて追加)
void displayMessage(const char* message) {
    M5.Lcd.clear();
    buttonInit();
    M5.Lcd.setCursor(0, 100);
    M5.Lcd.printf("%s", message);
}
