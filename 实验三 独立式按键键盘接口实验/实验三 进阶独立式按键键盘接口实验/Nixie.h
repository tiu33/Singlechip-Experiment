#ifndef __NIXIE_H__
#define __NIXIE_H__

void Nixie_Scan(unsigned char Location,Number);
void Nixie_Loop(void);
void Nixie_SetBuf(unsigned char Loacation,Number);
void Nixie_each_Loop(unsigned char Wei);
void Nixie_Change_Loop(unsigned char Wei);

#endif