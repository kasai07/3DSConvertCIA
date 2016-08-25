#pragma once

#include "common.h"

#define MENU_MAX_ENTRIES    12
#define MENU_MAX_DEPTH      4
#define MENU_EXIT_REBOOT    0
#define MENU_EXIT_POWEROFF  1



typedef struct {
    
	u32 InitSD;
	u32 Cart;
	u32 count;
	
} ListMenu;

ListMenu Cart;

typedef struct {
   
	u32 pos;
	u32 pos2;
} Menu;

Menu menupos;

u32 Menu3DS();


