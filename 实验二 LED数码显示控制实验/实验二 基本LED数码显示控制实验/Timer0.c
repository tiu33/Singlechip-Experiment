#include <REGX52.H>

void Timer0Init(void)		//10毫秒@12.000MHz
{
	TMOD |= 0x01;		//设定计时器模式
	TH0 = 0xDC;			//设置定时初值
	TL0 = 0x00;			//设置定时初值
	TR0 = 1;				//定时器从0开始计时
	//配置中断
	//也就是IE=0X82,开T0中断
	ET0=1;
	EA=1;
}

/*
void Timer0_Rountine() interrupt 1
{
	static unsigned int T0Count=0;		
	TL0 = 0x18;			//设置定时初值
	TH0 = 0xFC;	
	T0Count++;
	//1次中断一微秒，1000次就是1秒
	if(T0Count>=500)
	{
		T0Count=0;
		//使用INTRINS内置库进行循环左移或右移
		if(LEDMode==0)
			P2=_crol_(P2,1);
		if(LEDMode==1)
			P2=_cror_(P2,1);
	}
}
*/

