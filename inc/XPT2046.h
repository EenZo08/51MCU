#ifndef __XPT2046_H__
#define __XPT2046_H__

/*
XPT2046_XP	    0x9C	1001 1100	001	    测量 X+ 通道（触摸屏 X 轴位置）
XPT2046_YP	    0xDC	1101 1100	101	    测量 Y+ 通道（触摸屏 Y 轴位置）
XPT2046_VBAT	0xAC	1010 1100	010	    测量 电池电压
XPT2046_AUX	    0xEC	1110 1100	110	    测量 辅助输入通道
*/
#define XPT2046_VBAT 0xAC
#define XPT2046_AUX 0xEC
#define XPT2046_XP 0x9C // 0xBC
#define XPT2046_YP 0xDC

unsigned int XPT2046_ReadAD(unsigned char Commend);

#endif