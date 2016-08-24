#pragma once


#include "common.h"

#define HID_STATE (*(volatile u32*)0x10146000)

#define BUTTON_A     		(1 << 0) 	//0x00000001
#define BUTTON_B     		(1 << 1) 	//0x00000002
#define BUTTON_SELECT		(1 << 2) 	//0x00000004
#define BUTTON_START  		(1 << 3) 	//0x00000008
#define BUTTON_RIGHT  		(1 << 4) 	//0x00000010
#define BUTTON_LEFT			(1 << 5) 	//0x00000020
#define BUTTON_UP    		(1 << 6) 	//0x00000040
#define BUTTON_DOWN  		(1 << 7) 	//0x00000080
#define BUTTON_R1     		(1 << 8) 	//0x00000100
#define BUTTON_L1     		(1 << 9) 	//0x00000200
#define BUTTON_X      		(1 << 10) 	//0x00000400
#define BUTTON_Y      		(1 << 11) 	//0x00000800
#define BUTTON_ANY    		0x00000FFF
#define BUTTON_ARROW  (BUTTON_RIGHT|BUTTON_LEFT|BUTTON_UP|BUTTON_DOWN)

u32 InputWait();
