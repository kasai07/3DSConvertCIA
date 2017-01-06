#include "menu.h"
#include "draw.h"
#include "hid.h"
#include "keys.h"
#include "fs.h"
#include "tga.h"
#include "fatfs/ff.h"
#include "pathmenu.h"
#include "platform.h"
#include "game.h"
#include "title.h"
#include "titre.h"
#include "menu3ds.h"
#include "menucia.h"
#include "menudumpcart.h"
#include "button.h"
#include "Barre_smdh.h"
#include "smdh.h"
#include "i2c.h"

char* menulist[] = {
"Dump Cart to CIA",
"Dump & Decrypt Cart (full)",
"Dump & Decrypt Cart (trim)",
"Dump NTR CART (full)",
"Dump NTR CART (trim)",
"Dump Cart (full)",
"Dump Cart (trim)",
"Dump Private Header"
};
u32 paramdump[] = {16,26,26,20,20,16,16,19};

void DrawMenu(u32 count, u32 index, bool DrawTop, u32 menudraw, ListMenu Cart);
u32 MenuCIA();
u32 Menu3DS();
u32 MenuDumpCart();

u32 MenuCIA()
{
	u32 index = 0;
	u32 count = PathMenu(1);
	Cart.Cart = 0;
	DrawMenu(count, index, true , 1, Cart);
	
	while (true) 
	{
		bool DrawTop = true;
        u32 pad_state = InputWait();
		if (pad_state & BUTTON_X) {
			if(count != 0)
			{
				DeleteFileGame(index, 1);
				menu.refresh = 1;
			}
		} else if (pad_state & BUTTON_SELECT) {
            Screenshot(NULL);
        } else if (pad_state & BUTTON_B) {
			menu.refresh = 1;
		}  else if (pad_state & BUTTON_R1) {
			MenuDumpCart();
		}  else if (pad_state & BUTTON_L1) {
			Menu3DS();
		}  else if (pad_state & BUTTON_DOWN) {
		   index = (index == count - 1) ? 0 : index + 1;
           DrawTop = false;
		} else if (pad_state & BUTTON_UP) {
			index = (index == 0) ? count - 1 : index - 1;
			DrawTop = false;
		} else if (pad_state & BUTTON_POWER) {
		   PowerOff();
		} else if (pad_state & BUTTON_HOME) {
		   Reboot();
        } else {
			DrawTop = false;
		}
		if(menu.refresh == 1)
		{
			DeinitFS();
			InitFS();
			count = PathMenu(1);
			DrawTop = true;
			menu.refresh = 0;
			index = 0;
		}
		
		DrawMenu(count, index, DrawTop, 1, Cart);
	}
}

u32 MenuDumpCart()
{
	u32 index = 0;
	u32 count = 8;
	Cart.Cart = 1;
	DrawMenu(count, index, true, 2, Cart);
	
	while (true) 
	{
		bool DrawTop = true;
		u32 pad_state = InputWait();
		
		if (pad_state & BUTTON_A) {
			Cart.count = 0;
			if(index == 0)DumpGameCart(CD_DECRYPT | CD_MAKECIA);
			if(index == 1)DumpGameCart(CD_DECRYPT);
			if(index == 2)DumpGameCart(CD_DECRYPT | CD_TRIM);
			if(index == 3)DumpGameCart(CD_NTRCART);
			if(index == 4)DumpGameCart(CD_NTRCART | CD_TRIM);
			if(index == 5)DumpGameCart(0);
			if(index == 6)DumpGameCart(CD_TRIM);
			if(index == 7)DumpPrivateHeader(0);
			menu.refresh = 1;
		} else if (pad_state & BUTTON_B) {
			menu.refresh = 1;
		} else if (pad_state & BUTTON_SELECT) {
            Screenshot(NULL);
        } else if (pad_state & BUTTON_R1) {
			Menu3DS();
		} else if (pad_state & BUTTON_L1) {
			MenuCIA();
		} else if (pad_state & BUTTON_DOWN) {
			index = (index == count - 1) ? 0 : index + 1;
			DrawTop = false;
		} else if (pad_state & BUTTON_UP) {
			index = (index == 0) ? count - 1 : index - 1;
			DrawTop = false;
		} else if (pad_state & BUTTON_POWER) {
			PowerOff();
		 }else if (pad_state & BUTTON_HOME) {
			Reboot();
		} else {
			DrawTop = false;
		}
		if(menu.refresh == 1)
		{
			Cart.Cart = 1;
			DeinitFS();
			InitFS();
			DrawTop = true;
			menu.refresh = 0;
			index = 0;
		}
		
		DrawMenu(count, index, DrawTop, 2, Cart);
	}
}

u32 Menu3DS()
{
	u32 index = 0;
	menu.refresh = 0;
	u32 count = PathMenu(0);
	Cart.Cart = 0;
	DrawMenu(count, index, true, 0, Cart);
	
	while (true) 
	{
        
		
		bool DrawTop = true;
        u32 pad_state = InputWait();
		
		if (pad_state & BUTTON_A) {
            Cart.count = 0;
			Convert3dstoCIA(index);
			menu.refresh = 1;
		} else if (pad_state & BUTTON_B) {
			menu.refresh = 1;
		} else if (pad_state & BUTTON_DOWN) {
		   index = (index == count - 1) ? 0 : index + 1;
           DrawTop = false;
		} else if (pad_state & BUTTON_UP) {
			index = (index == 0) ? count - 1 : index - 1;
			DrawTop = false;
		} else if (pad_state & BUTTON_R1) {
			MenuCIA();
		}  else if (pad_state & BUTTON_L1) {
			MenuDumpCart();
		} else if (pad_state & BUTTON_X) {
			if(count != 0)
			{
				DeleteFileGame(index, 0);
				menu.refresh = 1;
			}
		} else if (pad_state & BUTTON_POWER) {
           PowerOff();
        } else if (pad_state & BUTTON_HOME) {
           Reboot();
        } else {
			DrawTop = false;
		}
		if(menu.refresh == 1)
		{
			DeinitFS();
			InitFS();
			count = PathMenu(0);
			DrawTop = true;
			menu.refresh = 0;
			index = 0;
		}
		DrawMenu(count, index, DrawTop, 0, Cart);
	}
}

void DrawMenu(u32 count, u32 index, bool DrawTop, u32 menudraw, ListMenu Cart)
{
    if (DrawTop) {
        
		ClearScreenFull(true, true);
		
		loadtga(true,false,"Game3ds/menu/topbg.tga",0,0);
		loadtga(false,true,"Game3ds/menu/botbg.tga",0,0);
		
		if(count == 0)DrawStringFColor(RED  , TRANSPARENT, 200 - ((16 * 8) / 2), 120, true,"Game Not Found !");
		if(Cart.Cart == 1)titlescreen();
		
		drawimage(true, titre, 32, 107, 5, 185, 20);//titre
		DrawStringFColor(WHITE, TRANSPARENT, 20, 50, false, "Power:PowerOFF");
		DrawStringFColor(WHITE, TRANSPARENT, 20, 60, false, "Home:Reboot");
		
		if(menudraw == 0)
		{
			drawimage(true, menu3ds, 32, 162, 30, 75, 14);//menu 3ds
			DrawStringFColor(GREEN, TRANSPARENT, 20, 80, false, "A : Convert");
			DrawStringFColor(RED, TRANSPARENT, 20, 90, false, "B : Refresh");
			DrawStringFColor(PURPLE, TRANSPARENT, 20, 120, false, "X : Delete the Game");
			DrawStringFColor(WHITE, TRANSPARENT, 20, 150, false, "R1 : Menu CIA");
			DrawStringFColor(WHITE, TRANSPARENT, 20, 160, false, "L1 : Menu Dump Cart");
		}
		if(menudraw == 1)
		{
			drawimage(true, menucia, 32, 160, 30, 81, 14);//menu cia
			DrawStringFColor(RED, TRANSPARENT, 20, 90, false, "B : Refresh");
			DrawStringFColor(PURPLE, TRANSPARENT, 20, 120, false, "X : Delete the Game");
			DrawStringFColor(WHITE, TRANSPARENT, 20, 150, false, "R1 : Menu Dump Carte");
			DrawStringFColor(WHITE, TRANSPARENT, 20, 160, false, "L1 : Menu 3DS");
		}
		if(menudraw == 2)
		{
			drawimage(true, menudumpcart, 32, 132, 30, 135, 17);//menu dump cart
			DrawStringFColor(GREEN, TRANSPARENT, 20, 80, false, "A : Launch");
			DrawStringFColor(RED, TRANSPARENT, 20, 90, false, "B : refresh cartridge");
			DrawStringFColor(WHITE, TRANSPARENT, 20, 150, false, "R1 : Menu 3DS");
			DrawStringFColor(WHITE, TRANSPARENT, 20, 160, false, "L1 : Menu CIA");
		}
		if (CheckSD()) {
			DrawStringFColor(WHITE, TRANSPARENT, 5, 210, false, "SD Storage:   %lluMB", TotalStorageSpace() / (1024*1024));
			DrawStringFColor(WHITE, TRANSPARENT, 5, 220, false, "      Used:   %lluMB", TotalStorageSpace() / (1024*1024) - RemainingStorageSpace() / (1024*1024));
			DrawStringFColor(WHITE, TRANSPARENT, 5, 230, false, "      Free:   %lluMB", RemainingStorageSpace() / (1024*1024));
		} else {
			DrawStringFColor(RED  , TRANSPARENT, 5, SCREEN_HEIGHT - 20, false, "SD storage: unknown filesystem");
		}
	}
	//---------------------------------------
	if(menudraw == 2)
	{
		for (u32 i = 0; i < count; i++) 
		{
			char* name = menulist[i];
			
			if(i != index)
			DrawStringFColor(WHITE, TRANSPARENT, 200 - ((paramdump[i] * 8) / 2), 50 + (i*13 + 2), true, "%s", name);
			
			if(i == index)
			DrawStringFColor(SELECT, TRANSPARENT, 200 - ((paramdump[i] * 8) / 2), 50 + (i*13 + 2), true, "%s", name);
		}
		
	} else {
		
		if(menudraw == 0)
		{	
			for (u32 i = 0; i < count; i++) 
			{
				drawimage(true, button,32, 25, 50 + (i*13), 350, 11);//button
				if(i >= 12)break;
			}
			if(index == 0)menu.pos2 = 0;	
			if(count > 12)
			{
				if((menu.pos2 + 12) < index)menu.pos2++;
				if(menu.pos2 > index)menu.pos2--;
				if(index == count - 1)menu.pos2 = count - 13;
			}
			menu.pos = menu.pos2;
		}
		
		if(menudraw == 1)
		{	
			drawimage(true, Barre_smdh,24, 25, 163, 350, 52);
			if(count != 0)icone_Large(index, true, false, 27, 165);
			
			for (u32 i = 0; i < count; i++) 
			{
				drawimage(true, button,32, 25, 50 + (i*13), 350, 11);//button
				if(i >= 7)break;
			}
			
			if(index == 0)menu.pos2 = 0;	
			if(count > 7)
			{
				if((menu.pos2 + 7) < index)menu.pos2++;
				if(menu.pos2 > index)menu.pos2--;
				if(index == count - 1)menu.pos2 = count - 8;
			}
			menu.pos = menu.pos2;
		}
		
		for (u32 i = 0; i < count; i++) 
		{
			if(menu.pos != index)
			{
				if(compteur[menu.pos] >= 37)
				{
					char name[38];
					snprintf(name, 37, "%s",c[menu.pos]);
					DrawStringFColor(WHITE, TRANSPARENT, 200 - ((39 * 8) / 2), 50 + (i*13 + 2), true, "%s...", name);
				} else {
					DrawStringFColor(WHITE, TRANSPARENT, 200 - ((compteur[menu.pos] * 8) / 2), 50 + (i*13 + 2), true, "%s", c[menu.pos]);
				}
			}
			if(menu.pos == index)
			{
				if(compteur[menu.pos] >= 37)
				{
					char name[38];
					snprintf(name, 37, "%s",c[menu.pos]);
					DrawStringFColor(SELECT, TRANSPARENT, 200 - ((39 * 8) / 2), 50 + (i*13 + 2), true, "%s...", name);
				} else {
					DrawStringFColor(SELECT, TRANSPARENT, 200 - ((compteur[menu.pos] * 8) / 2), 50 + (i*13 + 2), true, "%s", c[menu.pos]);
				}
			}
			menu.pos++;
			if(menudraw == 0){if(i >= 12)break;}
			if(menudraw == 1){if(i >= 7)break;}
		}
	}
}

	