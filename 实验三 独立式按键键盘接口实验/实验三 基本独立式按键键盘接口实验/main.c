#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"
#include "Key.h"

unsigned char Wei, KeyNum;	//Wei:��λ
//1����ʾ����ʾ��ֵ��ѡ���޸ĵ���λ
//2����һ��3����ֵ��һ��4����λ
//char ShuweiTable[] = {1, 2, 3, 4};


void main()
{
		Timer0Init();
		P1 = 0xFF;		// ��������ģʽ
		// �����ڲ�������
		// ע�⺯�����ֶ�Ӧ
		// ��C51�У������Ѿ�д�õĺ���ǰ��Ҳ����Ҫ��void����int
		// ����void Timer0Init();���޷����ú�����
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
		TL0 = 0x66;			//�趨��ʱ��ֵ
		T0Count1++;
		//�ж�һ�ξ���һ΢�룬1000�ξ���1��
		if(T0Count1>=20)
		{
				// ���ö�ʱ��ÿ20msɨ��һ�μ���
				T0Count1=0;
				Key_Loop();
		}
		T0Count2++;
		if(T0Count2>=2)
		{
				// ��ʱ��ÿ2msɨһ�������
				T0Count2=0;
				Nixie_Loop();
		}
}