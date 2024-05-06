#include<REGX52.H>
#include<INTRINS.H>
#include"Delay.h"

void shiftLeft(unsigned char num) 
{
    unsigned char i;
		unsigned char Data = 0xFF;
		Data = Data << num;								// 规定初始状态的Data
    for (i = 0; i < 8; i++) 
		{
				P2 = Data;
        Data = (Data << 1) | 0x01;		// 每次左移同时移入的是1
				Delay(500);
    }
}

void main()
{
		unsigned char count = 1;  // 记录当前LED灯数量
    while(1) 
		{  
        shiftLeft(count);  		// 左移count个LED灯
        count *= 2;  					// 增加LED灯的数量
        if (count > 4) 
				{  	
						// 如果超过4个灯，重置回一个灯
            count = 1;
        }
    }
}