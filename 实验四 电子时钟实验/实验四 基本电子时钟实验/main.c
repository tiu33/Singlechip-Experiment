#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"
#include "Key.h"

unsigned char RunFlag=1;			//RunFlag:ģʽѡ��,��ʾģʽ/�޸�ģʽ
unsigned char KeyNum;					//KeyNum:���̶�Ӧģʽ���
//1��У׼
//2�ǵ�ʱ��3�ǵ��֣�4�ǵ���
unsigned char Sec, Min, Hour;


void main()
{
		Timer0Init();
		// �����ڲ�������
		// ע�⺯�����ֶ�Ӧ
		// ��C51�У������Ѿ�д�õĺ���ǰ��Ҳ����Ҫ��void����int
		// ����void Timer0Init();���޷����ú�����
		P1 = 0xFF;		// ��������ģʽ
		while(1)
		{
				KeyNum=Key();
				if(KeyNum==1)
				{
						//P3_0=0;
						RunFlag=!RunFlag;
				}
				if(!RunFlag&&KeyNum==2)
				{
						// ��֤Сʱ��Զ���ᳬ��24
						Hour = (Hour + 1)%24;
				}
				if(!RunFlag&&KeyNum==3)
				{
						// ��֤������Զ���ᳬ��60
						Min = (Min + 1)%60;
				}
				if(!RunFlag&&KeyNum==4)
				{
						// ��֤����Զ���ᳬ��60
						Sec = (Sec + 1)%60;
				}
				// ��ʱ��洢����ʾ������
				Nixie_SetBuf(6,Sec%10);
				Nixie_SetBuf(5,Sec/10);
				Nixie_SetBuf(4,Min%10);
				Nixie_SetBuf(3,Min/10);
				Nixie_SetBuf(2,Hour%10);
				Nixie_SetBuf(1,Hour/10);
		}
}

void Sec_Loop(void)
{
		if(RunFlag)
		{
				Sec++;
				if(Sec>=60)
				{
						Sec=0;
						Min++;
						if(Min>=60)
						{
								Min=0;
								Hour++;
								if(Hour>=24)
								{
										Hour=0;
								}
						}
				}
		}
}


void Timer0_Rountine() interrupt 1
{
		static unsigned char T0Count1=0,T0Count2=0;		
		static unsigned int T0Count3=0;
		TH0 = 0xFC;			// 1msװ���ֵ
		TL0 = 0x66;			// �趨��ʱ��ֵ
		T0Count1++;
		//�ж�һ�ξ���һ���룬1000�ξ���1��
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
		T0Count3++;
		if(T0Count3>=1000)
		{
				// 1ms * 1000 = 1s
				T0Count3=0;
				Sec_Loop();
		}
}