#include <REGX52.H>

void Timer0Init(void)		//10����@12.000MHz
{
	TMOD |= 0x01;		//�趨��ʱ��ģʽ
	TH0 = 0xDC;			//���ö�ʱ��ֵ
	TL0 = 0x00;			//���ö�ʱ��ֵ
	TR0 = 1;				//��ʱ����0��ʼ��ʱ
	//�����ж�
	//Ҳ����IE=0X82,��T0�ж�
	ET0=1;
	EA=1;
}

/*
void Timer0_Rountine() interrupt 1
{
	static unsigned int T0Count=0;		
	TL0 = 0x18;			//���ö�ʱ��ֵ
	TH0 = 0xFC;	
	T0Count++;
	//1���ж�һ΢�룬1000�ξ���1��
	if(T0Count>=500)
	{
		T0Count=0;
		//ʹ��INTRINS���ÿ����ѭ�����ƻ�����
		if(LEDMode==0)
			P2=_crol_(P2,1);
		if(LEDMode==1)
			P2=_cror_(P2,1);
	}
}
*/

