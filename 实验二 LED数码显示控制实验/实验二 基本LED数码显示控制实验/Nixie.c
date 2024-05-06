#include <REGX52.H>
#include "Delay.h"

unsigned char NixieTable[]={0x3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F};

void Nixie(unsigned int theCount)       // 数码管显示的子函数，显示中断次数
{
		unsigned char wei = 0xfe;
		unsigned char num;
		// 0xFF~0xBF是只对前六个数码管进行操作
		while(wei != 0xBF)
		{
				// 使用取余操作获取当前最低位数字
				num = theCount % 10;
				// 然后用除法去掉当前最低位数字
				theCount = theCount / 10;
				P2 = wei;
				P0 = NixieTable[num];
				// 形成数字后先延时，否则会比较暗
				// 延时1ms，视觉暂留
				Delay(1);
				// 清零消隐
				P0=0x00;
				wei = (wei<<1)|0x01;
		}
}

