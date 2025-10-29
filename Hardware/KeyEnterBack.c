#include "stm32f10x.h"



uint8_t Key_Flag[2];

void Key_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure1;
    GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure1);
}

uint8_t Key_Check(uint8_t n,uint8_t Flag){
	if(Key_Flag[n] & Flag){
		
		if(Flag != 0x01) Key_Flag[n] &= ~Flag;
		return 1;
	}
	return 0;
}


uint8_t Key_GetState(uint8_t n){
	if(n == 0){
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0){
		    return 1;
	    }
	}
	if(n == 0){
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==0){
		    return 1;
	    }
	}

	return 0;
}

void Key_Tick(void){
	static uint8_t Count;
	static uint8_t CurrState,PrevState;
	static uint8_t S;
	static uint16_t Time;
	
	if(Time > 0) Time-- ;
	
	Count ++;
	if(Count >=20){
		
		
		Count = 0;
		PrevState = CurrState;
		CurrState = Key_GetState();
		
		if(CurrState == 1){
			Key_Flag |=0x01;//hold=1
		}
		else{
			Key_Flag |=~0x01;//hold=0
		}
		if(CurrState == 1 && PrevState==0){
			Key_Flag |=0x02;//down=1
		}
		if(CurrState == 0 && PrevState==1){
			Key_Flag |=0x04;//up=1
		}
		if(S ==0){
			if(CurrState == 1){
				Time =1000;
				S=1;
			}
		}
		else if(S == 1){
			if(CurrState == 0){
				Time =200;
				S=2;
			}
			else if(Time == 0){
				Time =100;
				S=4;
			}
		}
		else if(S == 2){
			if(CurrState == 1){
				S=3;
			}
			else if(Time ==0){
				S=0;
			}
		}
		else if(S == 3){
			if(CurrState == 0){
				S=0;
			}
		}
		else if(S == 4){
			if(CurrState == 0){
				S=0;
			}
			else if(Time ==0){
				Time = 100;
				S=4;
			}
				
		}
	}
		
	
}