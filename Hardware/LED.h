#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

void LED_Init(void);
uint32_t LED_GetDelayTime(int *SPEED);
uint16_t LED_GetPattern(int *ORDER, uint8_t step);


#endif
