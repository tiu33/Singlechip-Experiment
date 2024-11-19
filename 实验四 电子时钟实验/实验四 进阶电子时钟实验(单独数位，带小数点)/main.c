#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"
#include "Key.h"

unsigned char RunFlag=1;			//RunFlag:ģʽѡ��,��ʾģʽ/�޸�ģʽ
unsigned char KeyNum;					//KeyNum:���̶�Ӧģʽ���
//1��У׼
//2�ǵ�ʱ��3�ǵ��֣�4�ǵ���
unsigned char Time[6];
//Time[0]��ӦСʱ��ʮλ��Time[1]��ӦСʱ�ĸ�λ
//Time[2]��Ӧ���ӵ�ʮλ��Time[3]��Ӧ���ӵĸ�λ
//Time[4]��Ӧ���ʮλ��Time[5]��Ӧ��ĸ�λ
unsigned char SetTime[6];
unsigned char Wei, tmpNum, i, j;   // Wei��ʾѡȡʱ����λ��tmp���ݴ�ȷ������


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
				if(KeyNum == 0x0A)
				{
						// A�ż�λ������ͣ/����������ģʽ�л�
						//P3_0=~P3_0;
						RunFlag=!RunFlag;
						for(j=0;j<=5;j++)
						{
								SetTime[j] = Time[j];
						}
				}
				
				if(!RunFlag && KeyNum == 0x0B)
				{
						// B�ż�λѡȡ��λ
						Wei = (Wei + 1)%6;
						//P3 = ~(0x01<<Wei);
				}
				
				if(!RunFlag && (KeyNum >= 0x01 && KeyNum <= 0x09 || KeyNum == 0x10))
				{
						// ���ּ�λ��������
						// ��0���⴦��
						if(KeyNum == 0x10)KeyNum = 0;
						SetTime[Wei] = KeyNum;
						//P3_2=~P3_2;
				}
				
				if(!RunFlag && KeyNum == 0x0C )
				{
						// C�ż�λȷ��
						for(i=0;i<=5;i++)
						{
								if(i==0)
								{
										if(SetTime[i]>=3)
											Time[i]=0;
										else
											Time[i]=SetTime[i];
								}
								else if(i==1)
								{
										if(SetTime[0]==2 && SetTime[i]>=4)
											Time[i]=0;
										else
											Time[i]=SetTime[i];
								}
								else if(i==2 || i == 4)
								{
										if(SetTime[i]>=6)
											Time[i]=0;
										else
											Time[i]=SetTime[i];
								}
								else if(i==3 || i== 5)
								{
										Time[i]=SetTime[i];
								}
						}
						/*
						switch(Wei)
						{
							case 5:
								Time[Wei] = tmpNum;break;
							case 4:
								if(tmpNum>=6)break;
								Time[Wei] = tmpNum;break;
							case 3:
								Time[Wei] = tmpNum;break;
							case 2:
								if(tmpNum>=6)break;
								Time[Wei] = tmpNum;break;
							case 1:
								if(Time[0] == 2 && tmpNum>=4)break;
								Time[Wei] = tmpNum;break;
							case 0:
								if(tmpNum>=3)break;
								Time[Wei] = tmpNum;break;
						}
						tmpNum = 0;
						*/
						RunFlag=!RunFlag;
				}
				
				if(!RunFlag && KeyNum == 0x0D )
				{
						// D�ż�λȡ�������Ǿ͵����·���
						RunFlag=!RunFlag;
				}
				
				if(RunFlag)
				{
						// �ǵ���ģʽ��������ģʽ
						//Time[0]��ӦСʱ��ʮλ��Time[1]��ӦСʱ�ĸ�λ
						//Time[2]��Ӧ���ӵ�ʮλ��Time[3]��Ӧ���ӵĸ�λ
						//Time[4]��Ӧ���ʮλ��Time[5]��Ӧ��ĸ�λ
						for(i=1;i<=6;i++)
						{
								Nixie_SetBuf(i,Time[i-1]);
						}
				}
				else
				{
						// ����ģʽ��������ʾ
						/*
						for(i=1;i<=6;i++)
						{
								Nixie_SetBuf(i,SetTime[i-1]);
						}
						*/
							Nixie_SetBuf(Wei + 1,SetTime[Wei]);
						// tmpNum��ʾ��ǰ���ĵ���λ��Ӧ���ĵ�����
						//Nixie_SetBuf(Wei+1,tmpNum);
				}

		}
}

void Sec_Loop(void)
{
		if(RunFlag)
		{
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