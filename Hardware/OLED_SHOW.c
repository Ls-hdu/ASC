#include "stm32f10x.h"
#include "OLED.h"
#include "OLED_SHOW.h"

void OLED_InitShow(void){
    OLED_ShowString(1, 2, "LED Control");
    OLED_ShowString(2, 2, "PID");
    OLED_ShowString(3, 2, "Image");
    OLED_ShowString(4, 2, "Angle");
}

void OLED_InitShowLED(void){
    OLED_ShowString(1, 1, "LED Control");
    OLED_ShowString(2, 2, "LED_speed");
    OLED_ShowString(3, 2, "LED_dir");
}

void OLED_InitShowPID(void){
    OLED_ShowString(1, 1, "PID");
    OLED_ShowString(2, 2, "kp");
    OLED_ShowString(3, 2, "ki");
    OLED_ShowString(4, 2, "kd");
}

void OLED_InitShowImage(void){
    OLED_ShowString(1, 1, "Image");
    OLED_ShowString(2, 2, "Image");
}

void OLED_InitShowAngle(void){
    OLED_ShowString(1, 1, "Angle");
    OLED_ShowString(2, 2, "Angle");
}

void OLED_InitShowSure(int *flag){
    if(*flag) {
        OLED_ShowChar(1, 15, 'E');
    } else {
        OLED_ShowChar(1, 15, ' ');
    }
}

void OLED_InitShowCursurMainMenu(int i){
    OLED_ShowString(i, 1, ">");
}

void OLED_InitNshowCursurMainMenu(int i){
    OLED_ShowString(i, 1, " ");
}

void OLED_InitShowCursurSubMenu(int i){
    OLED_ShowString(i, 1, ">");
}

void OLED_InitNshowCursurSubMenu(int i){
    OLED_ShowString(i, 1, " ");
}

void OLED_InitShowOneDec(int i, int line, int col){
    int j, k,l;
	if(i<0) l=-i;
	else l=i;
    j = l / 10;  // 整数部分
    k = l % 10;  // 小数部分
	if(i<0) OLED_ShowString(line, col -1, "-");
	else OLED_ShowString(line, col -1, " ");
    OLED_ShowNum(line, col, j, 1);
    OLED_ShowString(line, col + 1, ".");
    OLED_ShowNum(line, col + 2, k, 1);
}

void OLED_FinalShow(int i){
    switch(i) {
        case 1: OLED_InitShowLED(); break;
        case 2: OLED_InitShowPID(); break;
        case 3: OLED_InitShowImage(); break;
        case 4: OLED_InitShowAngle(); break;
        default: break;
    }
}
