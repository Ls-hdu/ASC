#ifndef __OLED_SHOW_H
#define __OLED_SHOW_H

void OLED_InitShow(void);
void OLED_InitShowLED(void);
void OLED_InitShowPID(void);
void OLED_InitShowImage(void);
void OLED_InitShowAngle(void);
void OLED_FinalShow(int i);
void OLED_InitShowCursurMainMenu(int i);
void OLED_InitNshowCursurMainMenu(int i);
void OLED_InitNshowCursurSubMenu(int i);
void OLED_InitShowCursurSubMenu(int i);
void OLED_InitShowOneDec(int i,int line,int col);
void OLED_InitShowSure(int *flag);
#endif
