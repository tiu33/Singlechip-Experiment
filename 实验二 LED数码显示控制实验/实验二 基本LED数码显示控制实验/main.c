#include <REGX52.H>
#include <INTRINS.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"

unsigned int num,T0Count;	// 中断次数

/*
void Init_Timer0(void)
{
		TMOD |=  0x01;   //????1,16????,??"|"?????????????????
		TL0 = 0x18;                     //reload timer0 low byte
    TH0 = 0xFC;                //reload timer0 high byte
    TR0 = 1;                        //timer0 start running
    ET0 = 1;                        //enable timer0 interrupt
    EA = 1;                         //open global interrupt switch
}
*/

void main()
{
		//Init_Timer0();
		Timer0Init();
		// 括号内不带参数
		// 注意函数名字对应
		// 在C51中，调用已经写好的函数前面也不需要加void或者int
		// 你像void Timer0Init();就无法调用函数了
		while(1)
		{
				Nixie(num);
		}
}

void Timer0_Rountine() interrupt 1
{
	TH0 = 0xDC;			//设置定时初值		10ms
	TL0 = 0x00;			//设置定时初值
	num += 1;
	/*
	// 直接让其溢出等于0即可
	if(num>=10000)
	{
			num=0;
	}
	*/
}