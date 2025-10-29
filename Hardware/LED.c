#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"

void LED_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Write(GPIOB, 0xFFFF);
}

uint32_t LED_GetDelayTime(int *SPEED) {
    switch(*SPEED) {
        case 0:   return 500;
        case 1:   return 1000;
        case 2:   return 200;
        default:  return 500;
    }
}

uint16_t LED_GetPattern(int *ORDER, uint8_t step) {
    static const uint16_t forward_pattern[4] = {0x1000, 0x2000, 0x4000, 0x8000};
    static const uint16_t backward_pattern[4] = {0x8000, 0x4000, 0x2000, 0x1000};
    
    if (*ORDER == 0) {
        return forward_pattern[step % 4];
    } else {
        return backward_pattern[step % 4];
    }
}
