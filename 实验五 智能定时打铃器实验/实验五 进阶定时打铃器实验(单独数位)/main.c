#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"
#include "Key.h"

unsigned char RunFlag=0;			// RunFlag:ģʽѡ��,��ʾģʽ/�޸�ģʽ
unsigned char KeyNum;					// KeyNum:���̶�Ӧģʽ���
// 1������
// 2�ǵ���������λ��3����λ��һ��4����λ��һ
unsigned char Time[6];
//Time[0]��ӦСʱ��ʮλ��Time[1]��ӦСʱ�ĸ�λ
//Time[2]��Ӧ���ӵ�ʮλ��Time[3]��Ӧ���ӵĸ�λ
//Time[4]��Ӧ���ʮλ��Time[5]��Ӧ��ĸ�λ
unsigned char Wei, tmpPoint, j;   // Wei��ʾѡȡʱ����λ��tmpPoint���ݴ澲ָ̬��
unsigned char tmpTime[6];

// ���ΰ����룬�֣�ʱ���ж�ʱ����
unsigned char SetTime[18];		// �����򻺴�
unsigned char RingFlag;			// �����жϴ���ı�־
unsigned char RingTime = 5;	// ��������
unsigned char ZeroFlag;			// ���÷�����־
// �����ü��Σ���SetTime��Ԫ�ظ�������6
unsigned char SetChance = (sizeof (SetTime) / (6 * sizeof (unsigned char)));


void main()
{
		Timer0Init();
		// �����ڲ�������
		// ע�⺯�����ֶ�Ӧ
		// ��C51�У������Ѿ�д�õĺ���ǰ��Ҳ����Ҫ��void����int
		// ����void Timer0Init();���޷����ú�����
		P1 = 0x0F;		// ��������ģʽ
		while(1)
		{
				KeyNum=Key();
				//P3 = (0x01<<SetChance);
				if(KeyNum==1)
				{
						// A�ż�λ������ͣ/����������ģʽ�л�
						//P3_0=~P3_0;
						if(!SetChance)
								RunFlag=!RunFlag;
						else
						{
								for(j=0;j<=5;j++)
								{
										SetTime[tmpPoint+j] = tmpTime[j];
								}
								/*
								SetTime[tmpPoint] = tmpTime[0];
								SetTime[tmpPoint+1] = tmpTime[2]*10 + tmpTime[3];
								SetTime[tmpPoint+2] = tmpTime[0]*10 + tmpTime[1];
								*/
								tmpPoint += 6;
								ZeroFlag = 200;
								SetChance--;
						}
				}
				
				if(!RunFlag && KeyNum==2)
				{
						// B�ż�λѡȡ��λ
						Wei = (Wei + 1)%6;
						//P3 = ~(0x01<<Wei);
				}
				
				if(!RunFlag && KeyNum == 3)
				{
						if((Wei == 2 || Wei == 4) && tmpTime[Wei] < 5)
							// ���������ʮλ��
							tmpTime[Wei] += 1;
						else if((Wei == 3 || Wei == 5) && tmpTime[Wei] < 9)
							// ��������ĸ�λ��
							tmpTime[Wei] += 1;
						else if(Wei == 0 && tmpTime[Wei] < 2)
							// Сʱ��ʮλ��
							tmpTime[Wei] += 1;
						else if(Wei == 1 && tmpTime[0] == 2 && tmpTime[Wei] < 3)
							// 2��ͷ��Сʱ�ĸ�λ��
							tmpTime[Wei] += 1;
						else if(Wei == 1 && tmpTime[0] != 2 && tmpTime[Wei] < 9)
							// ��2��ͷ��Сʱ�ĸ�λ��
							tmpTime[Wei] += 1;
				}
				
				if(!RunFlag && KeyNum == 4)
				{
						if((Wei == 2 || Wei == 4) && tmpTime[Wei] > 0)
							// ���������ʮλ��
							tmpTime[Wei] -= 1;
						else if((Wei == 3 || Wei == 5) && tmpTime[Wei] > 0)
							// ��������ĸ�λ��
							tmpTime[Wei] -= 1;
						else if(Wei == 0 && tmpTime[Wei] > 0)
							// Сʱ��ʮλ��
							tmpTime[Wei] -= 1;
						else if(Wei == 1 && tmpTime[Wei] > 0)
							// Сʱ�ĸ�λ��
							tmpTime[Wei] -= 1;
				}
				
				if(RunFlag)
				{
						// �ǵ���ģʽ��������ģʽ
						//Time[0]��ӦСʱ��ʮλ��Time[1]��ӦСʱ�ĸ�λ
						//Time[2]��Ӧ���ӵ�ʮλ��Time[3]��Ӧ���ӵĸ�λ
						//Time[4]��Ӧ���ʮλ��Time[5]��Ӧ��ĸ�λ
						Nixie_SetBuf(1,Time[0]);
						Nixie_SetBuf(2,Time[1]);
						Nixie_SetBuf(3,Time[2]);
						Nixie_SetBuf(4,Time[3]);
						Nixie_SetBuf(5,Time[4]);
						Nixie_SetBuf(6,Time[5]);
				}
				else
				{
						// ����ģʽ��������ʾ
						Nixie_SetBuf(Wei+1,tmpTime[Wei]);
				}

		}
}

unsigned char check(unsigned char thePoint)
{
		if(thePoint >= (sizeof (SetTime) / sizeof (unsigned char)))
			// �������鳤��ֱ�ӱ�����
			return 0;
		
		for(j=0;j<=5;j++)
		{
				if(Time[j] !=  SetTime[thePoint + j])
						return 0;
		}
		
		return 1;
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
						SetPoint += 6;
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
				
				Time[5]++;
				if(Time[5]>=10)
				{
						Time[4]++;
						Time[5]=0;
						if(Time[4]>=6)
						{
								Time[3]++;
								Time[4]=0;
								if(Time[3]>=10)
								{
										Time[2]++;
										Time[3]=0;
										if(Time[2]>=6)
										{
												Time[1]++;
												Time[2]=0;
												if(Time[0]<2&&Time[1]>=10)
												{
														Time[0]++;
														Time[1]=0;
												}
												else if(Time[0]==2&&Time[1]>=4)
												{
														Time[0]=0;
														Time[1]=0;
												}
										}
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
		if(T0Count2>=2 && !RunFlag)
		{
				// ��ʱ��ÿ2msɨһ�������
				T0Count2=0;
				if(ZeroFlag)
				{
					// ������˸
					//P3_0 = 1;
					ZeroFlag--;
				}
				else
					Nixie_Change_Loop(Wei + 1);	
				//Nixie_each_Loop(Wei + 1);
		}
		
		T0Count3++;
		if(T0Count3>=1000)
		{
				// 1ms * 1000 = 1s
				T0Count3=0;
				Sec_Loop();
		}
		
}