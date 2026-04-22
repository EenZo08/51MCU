#ifndef __DISPLAY_H__
#define __DISPLAY_H__

extern unsigned char DigitalNum[];

void Digital(unsigned char x, unsigned char num);
void Digital_Set(unsigned char x, unsigned char num);
void Digital_Loop(void);
void Digital_SetNum(unsigned char num);
void Digital_SetTemp(unsigned int temp_x10);
void Digital_SetBlank(void);

#endif
