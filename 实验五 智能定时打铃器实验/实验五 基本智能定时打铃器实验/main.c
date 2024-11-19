#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"
#include "Key.h"

unsigned char RunFlag=1;			// RunFlag:模式选择,显示模式/修改模式
unsigned char Time[3] = {0, 0, 0};
// Time[0]是秒，Time[1]是分，Time[2]是时

// 依次按照秒，分，时进行定时设置
unsigned char SetTime[] = {10, 0, 0, 30, 0, 0, 0, 1, 0, 10, 1, 0, 20, 1, 0};
unsigned char RingFlag;		// 用于判断打铃的标志
unsigned char RingTime = 5;	// 打铃秒数


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
				if(RunFlag)
				{
					// 非调试模式，即正常模式
					// Time[0]秒，Time[1]分钟,Time[2]小时
					Nixie_SetBuf(1,Time[2]/10);
					Nixie_SetBuf(2,Time[2]%10);
					Nixie_SetBuf(3,Time[1]/10);
					Nixie_SetBuf(4,Time[1]%10);
					Nixie_SetBuf(5,Time[0]/10);
					Nixie_SetBuf(6,Time[0]%10);
				}
		}
}

unsigned char check(unsigned char thePoint)
{
		if(thePoint >= (sizeof (SetTime) / sizeof (unsigned char)))
			// 超出数组长度直接报错返回
			return 0;
		
		if(Time[0] == SetTime[thePoint] && Time[1] == SetTime[thePoint+1] && Time[2] == SetTime[thePoint+2])
			return 1;
		else
			return 0;
}

void Sec_Loop(void)
{
		// 方便查找比对的静态指针
		static unsigned char SetPoint;
		if(RunFlag)
		{
				// 检查是否在非打铃状态下，然后判断是否需要打铃
				if(check(SetPoint) && !RingFlag)
				{
						RingFlag = RingTime;
						// 指针跳转到下一个需要打铃的秒数上
						SetPoint += 3;
				}
				
				if(RingFlag)
				{
						RingFlag--;
						P1_4 = 0;
				}
				else
				{
						P1_4 = 1;
				}

				Time[0]++;
				if(Time[0]>=60)
				{
						Time[0]=0;
						Time[1]++;
						if(Time[1]>=60)
						{
								Time[1]=0;
								Time[2]++;
								if(Time[2]>=24)
								{
										Time[2]=0;
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
		if(T0Count2>=2 && RunFlag)
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