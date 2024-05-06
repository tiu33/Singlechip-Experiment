#include<REGX52.H>
#include<INTRINS.H>
#include"Delay.h"

unsigned char DATA;
void main()
{
		DATA = 0xFE;
		while(1)
		{
				P2 = DATA;
				DATA = _crol_(DATA, 1);
				Delay(500);
		}
}