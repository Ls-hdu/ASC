#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "OLED_SHOW.h"
#include "Encoder.h"
#include "Timer.h"

#define KEY_PRESSED				1
#define KEY_UNPRESSED			0

#define KEY_1					0
#define KEY_2					1
#define KEY_3					2
#define KEY_4					3

#define KEY_HOLD				0x01
#define KEY_DOWN				0x02
#define KEY_UP					0x04
#define KEY_SINGLE				0x08
#define KEY_DOUBLE				0x10
#define KEY_LONG				0x20
#define KEY_REPEAT				0x40

// 菜单行映射函数
int line(int i){
    if(i == 0) return 1;  // LED Control
    if(i == 1) return 4;  // PID
    if(i == 2) return 3;  // Image
    if(i == 3) return 2;  // Angle
    return 1;
}

int line1(int i){
    if(i == 0) return 2;  // LED_speed
    if(i == 1) return 3;  // LED_dir
    return 2;
}

int line2(int i){
    if(i == 0) return 2;  // kp
    if(i == 1) return 4;  // ki
    if(i == 2) return 3;  // kd
    return 2;
}

int main(void) {
    LED_Init();
    Key_Init();
    OLED_Init();
    OLED_InitShow();
	Encoder_Init();
	Timer_Init();
    
    int LED[2] = {0, 0};      // [速度, 方向]
    int PID[4] = {0, 0,0,0};   // [kp, ki, kd]
    int i = 0, j = 0;         // 主菜单索引，子菜单索引
    int i1 = 0, j1 = 0;       // 临时索引
    int k = 1;                // 菜单标志：1-主菜单，0-子菜单
	int l=0;
    int up = 4, up1 = 2;      // 上限值
    int Flag = 0;             // 进入子菜单标志
    int flag = 0;             // 编辑模式标志
    uint8_t current_step = 0;

    while(1) {
		
		
        // 根据当前模式设置上限
        if(line(i) == 1) {  // LED Control
            up = 1;         // LED子菜单项数-1
            if(j == 0) up1 = 2;  // 速度范围0-2
            else if(j == 1) up1 = 1;  // 方向范围0-1
        }
        if(line(i) == 2) {  // PID
            up = 2;         // PID子菜单项数-1
        }
        
        // 按键处理
        if(Flag == 0) flag = 0;  // 主菜单时退出编辑模式
        
        if(flag == 0) {
            // 导航模式
			l=3;
			PID[l]+=Encoder_GetCount();
			
            if(k) {
                // 主菜单导航
                Key_UpNum(&i1);
                Key_DownNum(&i1);
				if(i1 > 3) i1 = 0;
				if(i1 < 0) i1 = 3;
            } 
			else {
                // 子菜单导航
                Key_UpNum(&j1);
                Key_DownNum(&j1);
				if(j1 > up) j1 = 0;
				if(j1 < 0) j1 = up;
            }
        } 
		else {
            // 编辑模式
            if(line(i) == 1) {
                // LED参数编辑
                Key_UpNum(&LED[j]);
                Key_DownNum(&LED[j]);
				if(LED[j] > up1) LED[j] = 0;
				if(LED[j] < 0) LED[j] = up1;
            } 
			else if(line(i) == 2) {
                // PID参数编辑
				l=line2(j1)-2;
                Key_UpNum1(&PID[line2(j1)-2]);
                Key_DownNum1(&PID[line2(j1)-2]);
				PID[l]+=Encoder_GetCount();
				if(PID[j]>100)PID[j]=-100;
				if(PID[j]<-100)PID[j]=100;
            }
        }

        // 确认键处理
        (Flag == 0) ? Key_EnterNum(&Flag) : Key_EnterNum1(&flag);
        
        // 光标显示
        if(k) {
            // 主菜单光标
            if(i != i1) {
                OLED_InitNshowCursurMainMenu(line(i));
                OLED_InitShowCursurMainMenu(line(i1));
                i = i1;
            }
            OLED_InitShowCursurMainMenu(line(i));
        } else {
            // 子菜单光标
            if(j != j1) {
                if(line(i) == 1) {
                    OLED_InitNshowCursurSubMenu(line1(j));
                    OLED_InitShowCursurSubMenu(line1(j1));
                } else if(line(i) == 2) {
                    OLED_InitNshowCursurSubMenu(line2(j));
                    OLED_InitShowCursurSubMenu(line2(j1));
                }
                j = j1;
            }
            
            // 根据当前菜单显示光标
            if(line(i) == 1) {
                OLED_InitShowCursurSubMenu(line1(j));
            } 
			else if(line(i) == 2) {
                OLED_InitShowCursurSubMenu(line2(j));
            }
			else if(line(i) == 3) {
                OLED_InitShowCursurSubMenu(2);
            }
			else if(line(i) == 4) {
                OLED_InitShowCursurSubMenu(2);
            }
        }
        
        // 返回键处理
        Key_BackNum(&Flag);
        
        // 菜单切换处理
        if(Flag) {
            if(k) {
                OLED_Clear();
                OLED_FinalShow(line(i));
                k = 0;
                j1 = 0;  // 重置子菜单索引
                j = 0;
            }
            
            // 显示参数值
            if(line(i) == 1) {  // LED Control
                OLED_ShowNum(2, 14, LED[0], 1);  // 显示速度
                OLED_ShowNum(3, 14, LED[1], 1);  // 显示方向
                OLED_InitShowSure(&flag);
            } 
			else if(line(i) == 2) {  // PID
                OLED_InitShowOneDec(PID[0], 2, 14);  // kp
                OLED_InitShowOneDec(PID[1], 3, 14);  // ki
                OLED_InitShowOneDec(PID[2], 4, 14);  // kd
                OLED_InitShowSure(&flag);
            }

        } 
		else {
            if(k == 0) {
                OLED_Clear();
                OLED_InitShow();
                k = 1;
                i1 = 0;  // 重置主菜单索引
                i = 0;
            }
        }
        
        // LED控制逻辑
          static uint32_t accumulated_time = 0;
          accumulated_time += 10;
         uint32_t required_time = LED_GetDelayTime(&LED[0]);
          if (accumulated_time >= required_time) {
              uint16_t led_pattern = LED_GetPattern(&LED[1], current_step);
              GPIO_Write(GPIOB, (uint16_t)~led_pattern);
              current_step = (current_step + 1) % 4;
              accumulated_time = 0;
          }
        
        Delay_ms(10);
    }
}
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
