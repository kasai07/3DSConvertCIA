#pragma once


#include "common.h"

#define TOUCH_SCREEN (*(volatile u32*)0x10147000)
#define BUTTON_T_SCREEN     		1 // 3 no touch  
#define BUTTON_WIFI_SLIDER (*(volatile u32*)0x10144000) 
#define BUTTON_WF     				10
 
#define HID_STATE (*(volatile u32*)0x10146000)

#define BUTTON_A     			(1 << 0) 	
#define BUTTON_B     			(1 << 1) 	
#define BUTTON_SELECT			(1 << 2) 	
#define BUTTON_START  			(1 << 3) 	
#define BUTTON_RIGHT  			(1 << 4) 	
#define BUTTON_LEFT				(1 << 5) 	
#define BUTTON_UP    			(1 << 6) 	
#define BUTTON_DOWN  			(1 << 7) 	
#define BUTTON_R1     			(1 << 8) 	
#define BUTTON_L1     			(1 << 9) 	
#define BUTTON_X      			(1 << 10) 	
#define BUTTON_Y      			(1 << 11) 	
#define BUTTON_POWER 			1
#define BUTTON_HOME				2
#define BUTTON_WIFI				3
#define BUTTON_ANY    			0x00000FFF
#define BUTTON_ARROW  (BUTTON_RIGHT|BUTTON_LEFT|BUTTON_UP|BUTTON_DOWN)


u32 padflag_state;

u32 InputWait();
