#include<REGX52.H>
#include<INTRINS.H>
#include"Delay.h"

void shiftLeft(unsigned char num) 
{
    unsigned char i;
		unsigned char Data = 0xFF;
		Data = Data << num;								// �涨��ʼ״̬��Data
    for (i = 0; i < 8; i++) 
		{
				P2 = Data;
        Data = (Data << 1) | 0x01;		// ÿ������ͬʱ�������1
				Delay(500);
    }
}

void main()
{
		unsigned char count = 1;  // ��¼��ǰLED������
    while(1) 
		{  
        shiftLeft(count);  		// ����count��LED��
        count *= 2;  					// ����LED�Ƶ�����
        if (count > 4) 
				{  	
						// �������4���ƣ����û�һ����
            count = 1;
        }
    }
}