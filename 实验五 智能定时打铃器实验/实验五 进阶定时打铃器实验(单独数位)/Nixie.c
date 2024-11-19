#include <REGX52.H>
#include "Delay.h"

unsigned char Nixie_Buf[9];
unsigned char DP_Flag;	// 小数点标志

unsigned char NixieTable[]={0x3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F,0x00,0x40};
unsigned char DP_NixieTable[]={0xBF,0X86,0XDB,0XCF,0XE6,0XED,0XFD,0X87,0XFF,0XEF,0x80,0xC0};
void Nixie_SetBuf(unsigned char Loacation,Number)
{
		Nixie_Buf[Loacation]=Number;
}

void Nixie_Scan(unsigned char Location,Number)
{
		P0=0X00;
		DP_Flag = 0;
		switch(Location)
		{
			case 1:P2=0xFE;break;
			case 2:P2=0xFD;DP_Flag=1;break;
			case 3:P2=0xFB;break;
			case 4:P2=0xF7;DP_Flag=1;break;
			case 5:P2=0xEF;break;
			case 6:P2=0xDF;break;
		}
		if(DP_Flag)
			P0=DP_NixieTable[Number];
		else
			P0=NixieTable[Number];
		// 延时1ms，视觉暂留
		Delay(1);
		// 消隐清零
		P0=0x00;
}

void Nixie_Loop(void)
{
		static unsigned char i;
		Nixie_Scan(i,Nixie_Buf[i]);
		i++;
		if(i>=7){i=1;}
}

void Nixie_each_Loop(unsigned char Wei)
{
		Nixie_Scan(Wei,Nixie_Buf[Wei]);
}

void Nixie_Change_Loop(unsigned char Wei)
{
		static unsigned char i, flag;
		if(i==Wei)
		{
				// 闪烁位置
				if(flag < 100)Nixie_Scan(i,Nixie_Buf[i]);	
				flag = (flag + 1)%200;
		}
		else	Nixie_Scan(i,Nixie_Buf[i]);
		i++;
		if(i>=7){i=1;}
}