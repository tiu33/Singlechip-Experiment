#include <REGX52.H>
#include <INTRINS.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"

unsigned int num,T0Count;	// �жϴ���

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
		// �����ڲ�������
		// ע�⺯�����ֶ�Ӧ
		// ��C51�У������Ѿ�д�õĺ���ǰ��Ҳ����Ҫ��void����int
		// ����void Timer0Init();���޷����ú�����
		while(1)
		{
				Nixie(num);
		}
}

void Timer0_Rountine() interrupt 1
{
	TH0 = 0xDC;			//���ö�ʱ��ֵ		10ms
	TL0 = 0x00;			//���ö�ʱ��ֵ
	num += 1;
	/*
	// ֱ�������������0����
	if(num>=10000)
	{
			num=0;
	}
	*/
}