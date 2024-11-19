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
	//һ���ǵ��������if��while�ڵĶ�Ҫ�޸�
	if(P1_0==0){KeyNumber=1;}
	if(P1_1==0){KeyNumber=2;}
	if(P1_2==0){KeyNumber=3;}
	if(P1_3==0){KeyNumber=4;}
	return KeyNumber;
}

void Key_Loop(void)
{
		static unsigned char NowState,LastState;
		LastState=NowState;
		NowState=Key_GetState();
		if(LastState && NowState==0)
		{
				// LastState��ֵ
				Key_KeyNumber=LastState;
		}
}