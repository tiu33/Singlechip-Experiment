#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"
#include "Key.h"

unsigned char RunFlag=1;			//RunFlag:ģʽѡ��,��ʾģʽ/�޸�ģʽ
unsigned char Wei, KeyNum;	//Wei:��λ
//1����ʾ����ʾ��ֵ��ѡ���޸ĵ���λ
//2����һ��3����ֵ��һ��4����λ
char ShuweiTable[] = {0, 0, 0};


void main()
{
		Timer0Init();
		// �����ڲ�������
		// ע�⺯�����ֶ�Ӧ
		// ��C51�У������Ѿ�д�õĺ���ǰ��Ҳ����Ҫ��void����int
		// ����void Timer0Init();���޷����ú�����
		while(1)
		{
				KeyNum=Key();
				P1 = 0xFF;		// ��������ģʽ
				if(KeyNum==1)
				{
						P3_0=0;
						RunFlag=!RunFlag;
				}
				if(KeyNum==2)
				{
						// ��ʱ������λ
						// ��֤��λ������Զ���ᳬ��3
						Wei= (Wei + 1)%3;
				}
				if(KeyNum==3)
				{
						// ��Ӧ��λ�ϵ����ּ�һ
						ShuweiTable[Wei] = (ShuweiTable[Wei] + 1) % 10;
						// ���û�����
						Nixie_SetBuf(Wei+1,ShuweiTable[Wei]);
						/*
						// ֻ��ʾ��Ӧλ
						Nixie_Scan(Wei+1,ShuweiTable[Wei]);
						*/	
				}
				if(KeyNum==4)
				{
						// ��Ӧ��λ�ϵ����ּ�һ
						// ��ֹ�������
						if (ShuweiTable[Wei] - 1 <0)
							ShuweiTable[Wei]=9;
						else
							ShuweiTable[Wei] = (ShuweiTable[Wei] - 1) % 10;
						// ���û�����
						Nixie_SetBuf(Wei+1,ShuweiTable[Wei]);
						/*
						// ֻ��ʾ��Ӧλ
						Nixie_Scan(Wei+1,ShuweiTable[Wei]);
						*/
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
		if(T0Count2>=2&&RunFlag)
		{
				// ��ʱ��ÿ2msɨһ�������
				T0Count2=0;
				Nixie_Loop();
		}
		if(T0Count2>=2&&!RunFlag)
		{
				// ��ʱ��ÿ2msɨһ�������
				T0Count2=0;
				Nixie_Change_Loop(Wei + 1);
				//Nixie_each_Loop(Wei+1);
		}
}