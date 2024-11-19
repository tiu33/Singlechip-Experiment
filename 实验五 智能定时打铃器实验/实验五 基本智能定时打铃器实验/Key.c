#include <REGX52.H>
#include "Delay.h"

unsigned char Key_KeyNumber;

unsigned char Key(void)
{
		unsigned char Temp=0;
		Temp=Key_KeyNumber;
		Key_KeyNumber=0;
		return Temp;
}

unsigned char Key_GetState()
{
	unsigned char KeyNumber=0;
	//一定记得软件消抖if和while内的都要修改
	
	P2 = 0xFF;
	P2_0=0;
	//先给按键赋值，使得变量彼此独立
	if(P1_0==0){Delay(20);while(P1_0==0);Delay(20);KeyNumber=0x07;}
	if(P1_1==0){Delay(20);while(P1_1==0);Delay(20);KeyNumber=0x04;}
	if(P1_2==0){Delay(20);while(P1_2==0);Delay(20);KeyNumber=0X01;}
	if(P1_3==0){Delay(20);while(P1_3==0);Delay(20);KeyNumber=0X10;}
	
	
	P2 = 0xFF;
	P2_1=0;
	//先给按键赋值，使得变量彼此独立
	if(P1_0==0){Delay(20);while(P1_0==0);Delay(20);KeyNumber=0x08;}
	if(P1_1==0){Delay(20);while(P1_1==0);Delay(20);KeyNumber=0x05;}
	if(P1_2==0){Delay(20);while(P1_2==0);Delay(20);KeyNumber=0X02;}
	if(P1_3==0){Delay(20);while(P1_3==0);Delay(20);KeyNumber=0X0F;}
	
	P2 = 0xFF;
	P2_2=0;
	//先给按键赋值，使得变量彼此独立
	if(P1_0==0){Delay(20);while(P1_0==0);Delay(20);KeyNumber=0x09;}
	if(P1_1==0){Delay(20);while(P1_1==0);Delay(20);KeyNumber=0x06;}
	if(P1_2==0){Delay(20);while(P1_2==0);Delay(20);KeyNumber=0X03;}
	if(P1_3==0){Delay(20);while(P1_3==0);Delay(20);KeyNumber=0X0E;}
	
	P2 = 0xFF;
	P2_3=0;
	//先给按键赋值，使得变量彼此独立
	if(P1_0==0){Delay(20);while(P1_0==0);Delay(20);KeyNumber=0x0A;}
	if(P1_1==0){Delay(20);while(P1_1==0);Delay(20);KeyNumber=0x0B;}
	if(P1_2==0){Delay(20);while(P1_2==0);Delay(20);KeyNumber=0X0C;}
	if(P1_3==0){Delay(20);while(P1_3==0);Delay(20);KeyNumber=0X0D;}
	
	return KeyNumber;
}

void Key_Loop(void)
{
		static unsigned char NowState,LastState;
		LastState=NowState;
		NowState=Key_GetState();
		if(LastState && NowState==0)
		{
				// LastState有值
				Key_KeyNumber=LastState;
		}
}