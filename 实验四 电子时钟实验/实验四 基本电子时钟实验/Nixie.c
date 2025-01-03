#include <REGX52.H>
#include "Delay.h"

unsigned char Nixie_Buf[9];

unsigned char NixieTable[]={0x3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F,0x00,0x40};

void Nixie_SetBuf(unsigned char Loacation,Number)
{
		Nixie_Buf[Loacation]=Number;
}

void Nixie_Scan(unsigned char Location,Number)
{
		P0=0X00;
		switch(Location)
		{
			case 1:P2=0xFE;break;
			case 2:P2=0xFD;break;
			case 3:P2=0xFB;break;
			case 4:P2=0xF7;break;
			case 5:P2=0xEF;break;
			case 6:P2=0xDF;break;
		}
		P0=NixieTable[Number];
		// ��ʱ1ms���Ӿ�����
		Delay(1);
		// ��������
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