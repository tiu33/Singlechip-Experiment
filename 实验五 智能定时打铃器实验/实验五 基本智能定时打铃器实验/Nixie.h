#ifndef __NIXIE_H__
#define __NIXIE_H__

void Nixie_Scan(unsigned char Location,Number);
void Nixie_Loop(void);
void Nixie_SetBuf(unsigned char Loacation,Number);
void Nixie_each_Loop(unsigned char Wei);	//仅单独显示数位
void Nixie_Change_Loop(unsigned char Wei);			// 闪烁单独数位

#endif