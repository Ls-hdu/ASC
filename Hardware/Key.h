#ifndef __KEY_H
#define __KEY_H

void Key_Init(void);
uint8_t Key_Check(uint8_t Flag);
void Key_Tick(void);
void Key_UpNum(int *n);
void Key_UpNum1(int *n);
void Key_DownNum(int *n);
void Key_DownNum1(int *n);
void Key_EnterNum(int *n);
void Key_EnterNum1(int *n);
void Key_BackNum(int *n);
#endif 
