#include "stm32f10x.h"

#define KEY_PRESSED				1
#define KEY_UNPRESSED			0

#define KEY_1Up					0
#define KEY_2Down				1
#define KEY_3Enter				2
#define KEY_4Back				3

#define KEY_HOLD				0x01
#define KEY_DOWN				0x02
#define KEY_UP					0x04
#define KEY_SINGLE				0x08
#define KEY_DOUBLE				0x10
#define KEY_LONG				0x20
#define KEY_REPEAT				0x40

uint8_t Key_Flag[4]={0,0,0,0};



void Key_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure1;
    GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_4 ;
    GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure1); 

	
	
	GPIO_InitTypeDef GPIO_InitStructure2;
    GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure2.GPIO_Pin =  GPIO_Pin_2 ;
    GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure2); 



	GPIO_InitTypeDef GPIO_InitStructure3;
	GPIO_InitStructure3.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure3.GPIO_Pin =  GPIO_Pin_0 /*enter*/;
    GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure3);



	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure4;
    GPIO_InitStructure4.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure4.GPIO_Pin = GPIO_Pin_14/*back*/;
    GPIO_InitStructure4.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure4);
}

uint8_t Key_GetState(uint8_t n)
{
	if (n ==  KEY_1Up)
	{
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)
		{
			return KEY_PRESSED;
		}
	}
	else if (n == KEY_2Down)
	{
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
		{
			return KEY_PRESSED;
		}
	}
	else if (n == KEY_3Enter)
	{
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
			return KEY_PRESSED;
		}
	}
	else if (n == KEY_4Back)
	{
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 0)
		{
			return KEY_PRESSED;
		}
	}
	return KEY_UNPRESSED;
}



uint8_t Key_Check(uint8_t n, uint8_t Flag)
{
	if (Key_Flag[n] & Flag)
	{
		if (Flag != KEY_HOLD)
		{
			Key_Flag[n] &= ~Flag;
		}
		return 1;
	}
	return 0;
}

void Key_Tick(void)
{
	static uint8_t Count, i;
	static uint8_t CurrState[4], PrevState[4];
	static uint8_t S[4];
	static uint16_t Time[4];
	
	for (i = 0; i < 4; i ++)
	{
		if (Time[i] > 0)
		{
			Time[i] --;
		}
	}
	
	Count ++;
	if (Count >= 20)
	{
		Count = 0;
		
		for (i = 0; i < 4; i ++)
		{
			PrevState[i] = CurrState[i];
			CurrState[i] = Key_GetState(i);
			
			if (CurrState[i] == KEY_PRESSED)
			{
				Key_Flag[i] |= KEY_HOLD;
			}
			else
			{
				Key_Flag[i] &= ~KEY_HOLD;
			}
			
			if (CurrState[i] == KEY_PRESSED && PrevState[i] == KEY_UNPRESSED)
			{
				Key_Flag[i] |= KEY_DOWN;
			}
			
			if (CurrState[i] == KEY_UNPRESSED && PrevState[i] == KEY_PRESSED)
			{
				Key_Flag[i] |= KEY_UP;
			}
			
			if (S[i] == 0)
			{
				if (CurrState[i] == KEY_PRESSED)
				{
					Time[i] = 1000;
					S[i] = 1;
				}
			}
			else if (S[i] == 1)
			{
				if (CurrState[i] == KEY_UNPRESSED)
				{
					Time[i] = 200;
					S[i] = 2;
				}
				else if (Time[i] == 0)
				{
					Time[i] = 100;
					Key_Flag[i] |= KEY_LONG;
					S[i] = 4;
				}
			}
			else if (S[i] == 2)
			{
				if (CurrState[i] == KEY_PRESSED)
				{
					Key_Flag[i] |= KEY_DOUBLE;
					S[i] = 3;
				}
				else if (Time[i] == 0)
				{
					Key_Flag[i] |= KEY_SINGLE;
					S[i] = 0;
				}
			}
			else if (S[i] == 3)
			{
				if (CurrState[i] == KEY_UNPRESSED)
				{
					S[i] = 0;
				}
			}
			else if (S[i] == 4)
			{
				if (CurrState[i] == KEY_UNPRESSED)
				{
					S[i] = 0;
				}
				else if (Time[i] == 0)
				{
					Time[i] = 100;
					Key_Flag[i] |= KEY_REPEAT;
					S[i] = 4;
				}
			}
		}
	}
}

void Key_UpNum(int *n){
    if(Key_Check(0,KEY_SINGLE)) {
        (*n)++;  
    }
}

void Key_DownNum(int *n){
    if(Key_Check(1,KEY_SINGLE)) {
        (*n)--;  
    }
}

void Key_EnterNum(int *n){
    if(Key_Check(2,KEY_SINGLE)) {
        (*n)++;  
    }
}

void Key_BackNum(int *n){
    if(Key_Check(3,KEY_SINGLE)) {
        (*n) = 0;  
    }
	else if (Key_Check(3,KEY_LONG)){
		(*n) = 0;
	}
}

void Key_UpNum1(int *n){
    if(Key_Check(0,KEY_SINGLE) || Key_Check(0,KEY_REPEAT)) {
        (*n)++;  
    }
}

void Key_DownNum1(int *n){
    if(Key_Check(1,KEY_SINGLE) || Key_Check(1,KEY_REPEAT)) {
        (*n)--;  
    }
}

void Key_EnterNum1(int *n){
    if(Key_Check(2,KEY_SINGLE)) {
        if(*n == 0){
            *n = 1;
        }
        else{
            *n = 0;
        }
    }
}
