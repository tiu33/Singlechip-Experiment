#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"
#include "Key.h"

unsigned char RunFlag=1;			//RunFlag:模式选择,显示模式/修改模式
unsigned char KeyNum;					//KeyNum:键盘对应模式序号
//1是校准
//2是调时，3是调分，4是调秒
unsigned char Time[6];
//Time[0]对应小时的十位，Time[1]对应小时的个位
//Time[2]对应分钟的十位，Time[3]对应分钟的个位
//Time[4]对应秒的十位，Time[5]对应秒的个位
unsigned char SetTime[6];
unsigned char Wei, tmpNum, i, j;   // Wei表示选取时钟数位，tmp是暂存确定数据


void main()
{
		Timer0Init();
		// 括号内不带参数
		// 注意函数名字对应
		// 在C51中，调用已经写好的函数前面也不需要加void或者int
		// 你像void Timer0Init();就无法调用函数了
		while(1)
		{
				KeyNum=Key();
				if(KeyNum == 0x0A)
				{
						// A号键位进入暂停/启动，设置模式切换
						//P3_0=~P3_0;
						RunFlag=!RunFlag;
						for(j=0;j<=5;j++)
						{
								SetTime[j] = Time[j];
						}
				}
				
				if(!RunFlag && KeyNum == 0x0B)
				{
						// B号键位选取数位
						Wei = (Wei + 1)%6;
						//P3 = ~(0x01<<Wei);
				}
				
				if(!RunFlag && (KeyNum >= 0x01 && KeyNum <= 0x09 || KeyNum == 0x10))
				{
						// 数字键位设置数字
						// 对0特殊处理
						if(KeyNum == 0x10)KeyNum = 0;
						SetTime[Wei] = KeyNum;
						//P3_2=~P3_2;
				}
				
				if(!RunFlag && KeyNum == 0x0C )
				{
						// C号键位确认
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
						// D号键位取消，我们就当无事发生
						RunFlag=!RunFlag;
				}
				
				if(RunFlag)
				{
						// 非调试模式，即正常模式
						//Time[0]对应小时的十位，Time[1]对应小时的个位
						//Time[2]对应分钟的十位，Time[3]对应分钟的个位
						//Time[4]对应秒的十位，Time[5]对应秒的个位
						for(i=1;i<=6;i++)
						{
								Nixie_SetBuf(i,Time[i-1]);
						}
				}
				else
				{
						// 调试模式，单独显示
						/*
						for(i=1;i<=6;i++)
						{
								Nixie_SetBuf(i,SetTime[i-1]);
						}
						*/
							Nixie_SetBuf(Wei + 1,SetTime[Wei]);
						// tmpNum表示当前更改的数位对应更改的数字
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
		TH0 = 0xFC;			// 1ms装填初值
		TL0 = 0x66;			// 设定定时初值
		
		T0Count1++;
		//中断一次就是一毫秒，1000次就是1秒
		if(T0Count1>=20)
		{
				// 采用定时器每20ms扫描一次键盘
				T0Count1=0;
				Key_Loop();
		}
		
		T0Count2++;
		if(T0Count2>=2 && RunFlag)
		{
				// 定时器每2ms扫一次数码管
				T0Count2=0;
				Nixie_Loop();
		}
		if(T0Count2>=2 && !RunFlag)
		{
				// 定时器每2ms扫一次数码管
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