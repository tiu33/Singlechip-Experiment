#include <REGX52.H>
#include "Delay.h"

unsigned char NixieTable[]={0x3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F};

void Nixie(unsigned int theCount)       // �������ʾ���Ӻ�������ʾ�жϴ���
{
		unsigned char wei = 0xfe;
		unsigned char num;
		// 0xFF~0xBF��ֻ��ǰ��������ܽ��в���
		while(wei != 0xBF)
		{
				// ʹ��ȡ�������ȡ��ǰ���λ����
				num = theCount % 10;
				// Ȼ���ó���ȥ����ǰ���λ����
				theCount = theCount / 10;
				P2 = wei;
				P0 = NixieTable[num];
				// �γ����ֺ�����ʱ�������Ƚϰ�
				// ��ʱ1ms���Ӿ�����
				Delay(1);
				// ��������
				P0=0x00;
				wei = (wei<<1)|0x01;
		}
}

