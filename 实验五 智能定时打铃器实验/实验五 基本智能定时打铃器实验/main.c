#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"
#include "Key.h"

unsigned char RunFlag=1;			// RunFlag:ģʽѡ��,��ʾģʽ/�޸�ģʽ
unsigned char Time[3] = {0, 0, 0};
// Time[0]���룬Time[1]�Ƿ֣�Time[2]��ʱ

// ���ΰ����룬�֣�ʱ���ж�ʱ����
unsigned char SetTime[] = {10, 0, 0, 30, 0, 0, 0, 1, 0, 10, 1, 0, 20, 1, 0};
unsigned char RingFlag;		// �����жϴ���ı�־
unsigned char RingTime = 5;	// ��������


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
				if(RunFlag)
				{
					// �ǵ���ģʽ��������ģʽ
					// Time[0]�룬Time[1]����,Time[2]Сʱ
					Nixie_SetBuf(1,Time[2]/10);
					Nixie_SetBuf(2,Time[2]%10);
					Nixie_SetBuf(3,Time[1]/10);
					Nixie_SetBuf(4,Time[1]%10);
					Nixie_SetBuf(5,Time[0]/10);
					Nixie_SetBuf(6,Time[0]%10);
				}
		}
}

unsigned char check(unsigned char thePoint)
{
		if(thePoint >= (sizeof (SetTime) / sizeof (unsigned char)))
			// �������鳤��ֱ�ӱ�����
			return 0;
		
		if(Time[0] == SetTime[thePoint] && Time[1] == SetTime[thePoint+1] && Time[2] == SetTime[thePoint+2])
			return 1;
		else
			return 0;
}

void Sec_Loop(void)
{
		// ������ұȶԵľ�ָ̬��
		static unsigned char SetPoint;
		if(RunFlag)
		{
				// ����Ƿ��ڷǴ���״̬�£�Ȼ���ж��Ƿ���Ҫ����
				if(check(SetPoint) && !RingFlag)
				{
						RingFlag = RingTime;
						// ָ����ת����һ����Ҫ�����������
						SetPoint += 3;
				}
				
				if(RingFlag)
				{
						RingFlag--;
						P1_4 = 0;
				}
				else
				{
						P1_4 = 1;
				}

				Time[0]++;
				if(Time[0]>=60)
				{
						Time[0]=0;
						Time[1]++;
						if(Time[1]>=60)
						{
								Time[1]=0;
								Time[2]++;
								if(Time[2]>=24)
								{
										Time[2]=0;
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
		if(T0Count2>=2 && RunFlag)
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