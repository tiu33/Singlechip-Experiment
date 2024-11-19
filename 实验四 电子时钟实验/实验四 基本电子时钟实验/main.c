#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"
#include "Key.h"

unsigned char RunFlag=1;			//RunFlag:模式选择,显示模式/修改模式
unsigned char KeyNum;					//KeyNum:键盘对应模式序号
//1是校准
//2是调时，3是调分，4是调秒
unsigned char Sec, Min, Hour;


void main()
{
		Timer0Init();
		// 括号内不带参数
		// 注意函数名字对应
		// 在C51中，调用已经写好的函数前面也不需要加void或者int
		// 你像void Timer0Init();就无法调用函数了
		P1 = 0xFF;		// 进入输入模式
		while(1)
		{
				KeyNum=Key();
				if(KeyNum==1)
				{
						//P3_0=0;
						RunFlag=!RunFlag;
				}
				if(!RunFlag&&KeyNum==2)
				{
						// 保证小时永远不会超过24
						Hour = (Hour + 1)%24;
				}
				if(!RunFlag&&KeyNum==3)
				{
						// 保证分钟永远不会超过60
						Min = (Min + 1)%60;
				}
				if(!RunFlag&&KeyNum==4)
				{
						// 保证秒永远不会超过60
						Sec = (Sec + 1)%60;
				}
				// 将时间存储在显示缓存区
				Nixie_SetBuf(6,Sec%10);
				Nixie_SetBuf(5,Sec/10);
				Nixie_SetBuf(4,Min%10);
				Nixie_SetBuf(3,Min/10);
				Nixie_SetBuf(2,Hour%10);
				Nixie_SetBuf(1,Hour/10);
		}
}

void Sec_Loop(void)
{
		if(RunFlag)
		{
				Sec++;
				if(Sec>=60)
				{
						Sec=0;
						Min++;
						if(Min>=60)
						{
								Min=0;
								Hour++;
								if(Hour>=24)
								{
										Hour=0;
								}
						}
				}
		}
}


void Timer0_Rountine() interrupt 1
{
		static unsigned char T0Count1=0,T0Count2=0;		
		static unsigned int T0Count3=0;
		TH0 = 0xFC;			// 1ms装填初值
		TL0 = 0x66;			// 设定定时初值
		T0Count1++;
		//中断一次就是一毫秒，1000次就是1秒
		if(T0Count1>=20)
		{
				// 采用定时器每20ms扫描一次键盘
				T0Count1=0;
				Key_Loop();
		}
		T0Count2++;
		if(T0Count2>=2)
		{
				// 定时器每2ms扫一次数码管
				T0Count2=0;
				Nixie_Loop();
		}
		T0Count3++;
		if(T0Count3>=1000)
		{
				// 1ms * 1000 = 1s
				T0Count3=0;
				Sec_Loop();
		}
}