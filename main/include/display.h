#ifndef DISPLAY_H
#define DISPLAY_H

#include <M5Core2.h>

// ボタンの定義
#define btn_WID 120
#define btn_HET 50
#define btn_SPC 25
#define btn_ySET 170

// ボタンクラス
extern Button B_on;
extern Button B_off;

// プロトタイプ宣言
void initDisplay();
void updateDisplay();
void buttonInit();
void printPram();
void param_change();
void logState();
void displayMessage(const char* message);

#endif // DISPLAY_H
