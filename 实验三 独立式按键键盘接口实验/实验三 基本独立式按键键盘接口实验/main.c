#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"
#include "Key.h"

unsigned char Wei, KeyNum;	//Wei:数位
//1是显示器显示数值或选择修改的数位
//2是增一，3是数值减一，4是移位
//char ShuweiTable[] = {1, 2, 3, 4};


void main()
{
		Timer0Init();
		P1 = 0xFF;		// 进入输入模式
		// 括号内不带参数
		// 注意函数名字对应
		// 在C51中，调用已经写好的函数前面也不需要加void或者int
		// 你像void Timer0Init();就无法调用函数了
		while(1)
		{
				KeyNum=Key();
				if (KeyNum)
				{
					P3 = 0x01<<KeyNum;
					Nixie_SetBuf(1, KeyNum);
				}
				
		}
}


void Timer0_Rountine() interrupt 1
{
		static unsigned char T0Count1=0,T0Count2=0;		
		TH0 = 0xFC;	
		TL0 = 0x66;			//设定定时初值
		T0Count1++;
		//中断一次就是一微秒，1000次就是1秒
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
}