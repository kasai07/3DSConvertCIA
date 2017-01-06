#pragma once


#include "common.h"

#define EMMC_STATUS (*(volatile u8*)0x1000601C) 
#define SD_CARD_INSERT 	 	(1 << 5)

#define TOUCH_SCREEN (*(volatile u32*)0x10147000)
// 0x03 no touch   // 0x01 touch

#define FLAG_CHARGE (*(volatile u32*)0x10144000) 
#define CHARGE     				0xFA // no charge = 0xE2


#define HID_STATE (*(volatile u32*)0x10146000)

#define BUTTON_A     			(1 << 0)	//0FFF->0FFE 	0x00000001
#define BUTTON_B     			(1 << 1)	//0FFF->0FFD 	0x00000002
#define BUTTON_SELECT			(1 << 2)	//0FFF->0FFB	0x00000004
#define BUTTON_START  			(1 << 3)	//0FFF->0FE7 	0x00000008
#define BUTTON_RIGHT  			(1 << 4)	//0FFF->0FEF 	0x00000010
#define BUTTON_LEFT				(1 << 5)	//0FFF->0FDF	0x00000020
#define BUTTON_UP    			(1 << 6)	//0FFF->0FBF	0x00000040
#define BUTTON_DOWN  			(1 << 7)	//0FFF->0F7F 	0x00000080
#define BUTTON_R1     			(1 << 8)	//0FFF->0EFF 	0x00000100
#define BUTTON_L1     			(1 << 9)	//0FFF->0DFF 	0x00000200
#define BUTTON_X      			(1 << 10)	//0FFF->0BFF 	0x00000400
#define BUTTON_Y      			(1 << 11)	//0FFF->07FF 	0x00000800

#define BUTTON_POWER 			(1 << 12)  //0x00001000
#define BUTTON_HOME				(1 << 13)  //0x00002000
#define BUTTON_WIFI				(1 << 14)  //0x00004000
#define BUTTON_SCREEN			(1 << 15)  //0x00008000

#define BUTTON_ANY    			0x00000FFF
#define BUTTON_ARROW  (BUTTON_RIGHT|BUTTON_LEFT|BUTTON_UP|BUTTON_DOWN)

#define BIT_0	(1 << 0) //0x01
#define BIT_1	(1 << 1) //0x02
#define BIT_2	(1 << 2) //0x04
#define BIT_3	(1 << 3) //0x08
#define BIT_4	(1 << 4) //0x10
#define BIT_5	(1 << 5) //0x20
#define BIT_6	(1 << 6) //0x40
#define BIT_7	(1 << 7) //0x80


u32 InputWait();
