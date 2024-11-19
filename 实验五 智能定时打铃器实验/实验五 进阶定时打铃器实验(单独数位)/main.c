#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"
#include "Key.h"

unsigned char RunFlag=0;			// RunFlag:模式选择,显示模式/修改模式
unsigned char KeyNum;					// KeyNum:键盘对应模式序号
// 1是设置
// 2是调整设置数位，3是数位增一，4是数位减一
unsigned char Time[6];
//Time[0]对应小时的十位，Time[1]对应小时的个位
//Time[2]对应分钟的十位，Time[3]对应分钟的个位
//Time[4]对应秒的十位，Time[5]对应秒的个位
unsigned char Wei, tmpPoint, j;   // Wei表示选取时钟数位，tmpPoint是暂存静态指针
unsigned char tmpTime[6];

// 依次按照秒，分，时进行定时设置
unsigned char SetTime[18];		// 主程序缓存
unsigned char RingFlag;			// 用于判断打铃的标志
unsigned char RingTime = 5;	// 打铃秒数
unsigned char ZeroFlag;			// 设置反馈标志
// 能设置几次，即SetTime中元素个数除以6
unsigned char SetChance = (sizeof (SetTime) / (6 * sizeof (unsigned char)));


void main()
{
		Timer0Init();
		// 括号内不带参数
		// 注意函数名字对应
		// 在C51中，调用已经写好的函数前面也不需要加void或者int
		// 你像void Timer0Init();就无法调用函数了
		P1 = 0x0F;		// 进入输入模式
		while(1)
		{
				KeyNum=Key();
				//P3 = (0x01<<SetChance);
				if(KeyNum==1)
				{
						// A号键位进入暂停/启动，设置模式切换
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
						// B号键位选取数位
						Wei = (Wei + 1)%6;
						//P3 = ~(0x01<<Wei);
				}
				
				if(!RunFlag && KeyNum == 3)
				{
						if((Wei == 2 || Wei == 4) && tmpTime[Wei] < 5)
							// 分钟与秒的十位数
							tmpTime[Wei] += 1;
						else if((Wei == 3 || Wei == 5) && tmpTime[Wei] < 9)
							// 分钟与秒的个位数
							tmpTime[Wei] += 1;
						else if(Wei == 0 && tmpTime[Wei] < 2)
							// 小时的十位数
							tmpTime[Wei] += 1;
						else if(Wei == 1 && tmpTime[0] == 2 && tmpTime[Wei] < 3)
							// 2开头的小时的个位数
							tmpTime[Wei] += 1;
						else if(Wei == 1 && tmpTime[0] != 2 && tmpTime[Wei] < 9)
							// 非2开头的小时的个位数
							tmpTime[Wei] += 1;
				}
				
				if(!RunFlag && KeyNum == 4)
				{
						if((Wei == 2 || Wei == 4) && tmpTime[Wei] > 0)
							// 分钟与秒的十位数
							tmpTime[Wei] -= 1;
						else if((Wei == 3 || Wei == 5) && tmpTime[Wei] > 0)
							// 分钟与秒的个位数
							tmpTime[Wei] -= 1;
						else if(Wei == 0 && tmpTime[Wei] > 0)
							// 小时的十位数
							tmpTime[Wei] -= 1;
						else if(Wei == 1 && tmpTime[Wei] > 0)
							// 小时的个位数
							tmpTime[Wei] -= 1;
				}
				
				if(RunFlag)
				{
						// 非调试模式，即正常模式
						//Time[0]对应小时的十位，Time[1]对应小时的个位
						//Time[2]对应分钟的十位，Time[3]对应分钟的个位
						//Time[4]对应秒的十位，Time[5]对应秒的个位
						Nixie_SetBuf(1,Time[0]);
						Nixie_SetBuf(2,Time[1]);
						Nixie_SetBuf(3,Time[2]);
						Nixie_SetBuf(4,Time[3]);
						Nixie_SetBuf(5,Time[4]);
						Nixie_SetBuf(6,Time[5]);
				}
				else
				{
						// 调试模式，单独显示
						Nixie_SetBuf(Wei+1,tmpTime[Wei]);
				}

		}
}

unsigned char check(unsigned char thePoint)
{
		if(thePoint >= (sizeof (SetTime) / sizeof (unsigned char)))
			// 超出数组长度直接报错返回
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
		// 方便查找比对的静态指针
		static unsigned char SetPoint;
		if(RunFlag)
		{
				// 检查是否在非打铃状态下，然后判断是否需要打铃
				if(check(SetPoint) && !RingFlag)
				{
						RingFlag = RingTime;
						// 指针跳转到下一个需要打铃的秒数上
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
				if(ZeroFlag)
				{
					// 反馈闪烁
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