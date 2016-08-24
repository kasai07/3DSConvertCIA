#include "menu.h"
#include "draw.h"
#include "hid.h"
#include "keys.h"
#include "fs.h"
#include "tga.h"
#include "fatfs/ff.h"
#include "pathmenu.h"
#include "game.h"
#include "title.h"
#include "titre.h"
#include "menu3ds.h"
#include "menucia.h"
#include "menudumpcart.h"
#include "button.h"


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
	u32 refresh = 0;
	u32 count = PathMenucia();
	
	Cart.Cart = 0;
	Cart.InitSD = 0;
	DrawMenu(count, index, true , 1, Cart);
	
	
	while (true) 
	{
		
		bool DrawTop = true;
        u32 pad_state = InputWait();
		if (pad_state & BUTTON_X) {
            
			if(count != 0)
			{
				DeleteFileGame(index, 1);
				refresh = 1;
			}
			
		} else if (pad_state & BUTTON_B) {
            
			refresh = 1;
			Cart.InitSD = 1;
			
		} else if (pad_state & BUTTON_R1) {
            
			MenuDumpCart();
			
		} else if (pad_state & BUTTON_L1) {
            
			Menu3DS();
			
		} else if (pad_state & BUTTON_DOWN) {
           
		   index = (index == count - 1) ? 0 : index + 1;
           DrawTop = false;
			
		} else if (pad_state & BUTTON_UP) {
            
			index = (index == 0) ? count - 1 : index - 1;
			DrawTop = false;
		} else if (pad_state & BUTTON_START) {
           
		   DeinitFS();
		   PowerOff();
		   
        } else if (pad_state & BUTTON_SELECT) {
           
		   DeinitFS();
		   Reboot();
        } else {
			
			DrawTop = false;
			
		}
		
		if(refresh == 1)
		{
			count = PathMenucia();
			DrawTop = true;
			refresh = 0;
			index = 0;
			
		}
		DrawMenu(count, index, DrawTop, 1, Cart);
	}
	

}
u32 MenuDumpCart()
{
	
	u32 index = 0;
	u32 refresh = 0;
	u32 count = 8;
	Cart.Cart = 1;
	Cart.InitSD = 0;
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
			refresh = 1;
			
		} else if (pad_state & BUTTON_B) {
           
			refresh = 1;
			Cart.Cart = 1;
			Cart.InitSD = 1;
			
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
		} else if (pad_state & BUTTON_START) {
           
		   DeinitFS();
		   PowerOff();
		  
        }else if (pad_state & BUTTON_SELECT) {
           
		   DeinitFS();
		   Reboot();
		   
        } else {
			
			DrawTop = false;
			
		}
		
		if(refresh == 1)
		{
			
			DrawTop = true;
			refresh = 0;
			index = 0;
			
		}
		DrawMenu(count, index, DrawTop, 2, Cart);
	}
	
}


u32 Menu3DS()
{
    
	//Mise en memoire des noms des payload dans le "char c"
	//et retourne "count" le nombre de payload present
	
	
    
	u32 index = 0;
	
	u32 refresh = 0;
	u32 count = PathMenu3ds();
	Cart.Cart = 0;
	Cart.InitSD = 0;
	
	DrawMenu(count, index, true, 0, Cart);
   
 // main processing loop
    while (true) 
	{
        
		
		
		bool DrawTop = true;
        u32 pad_state = InputWait();
        
		if (pad_state & BUTTON_A) {
            Cart.count = 0;
			Convert3dstoCIA(index);
			refresh = 1;
		} else if (pad_state & BUTTON_B) {
            
			refresh = 1;
			Cart.InitSD = 1;
			
		} else if (pad_state & BUTTON_DOWN) {
           
		   index = (index == count - 1) ? 0 : index + 1;
           DrawTop = false;
			
		} else if (pad_state & BUTTON_UP) {
            
			index = (index == 0) ? count - 1 : index - 1;
			DrawTop = false;
			
		} else if (pad_state & BUTTON_R1) {
            
			MenuCIA();
			refresh = 1;
			
		} else if (pad_state & BUTTON_L1) {
            
			MenuDumpCart();
			refresh = 1;
			
		} else if (pad_state & BUTTON_X) {
            
			if(count != 0)
			{
				DeleteFileGame(index, 0);
				refresh = 1;
			}
		} else if (pad_state & BUTTON_START) {
           DeinitFS();
		   PowerOff();
        } else if (pad_state & BUTTON_SELECT) {
           DeinitFS();
		   Reboot();
        }else {
			
			DrawTop = false;
			
		}
		
		if(refresh == 1)
		{
			count = PathMenu3ds();
			DrawTop = true;
			refresh = 0;
			index = 0;
			
		}
		
		DrawMenu(count, index, DrawTop, 0, Cart);
        
    }
    
 
}
void DrawMenu(u32 count, u32 index, bool DrawTop, u32 menudraw, ListMenu Cart)
{
    
	char pathtga[60];
	
	if (DrawTop) { // draw full menu top
        
		ClearScreenFull(true, false);
		
		snprintf(pathtga, 60, "Game3ds/menu/topbg.tga");
		loadtga(true,false,pathtga,0,0);
		if(menudraw == 2)
		{
			
			for (u32 i = 0; i < count; i++) 
			{
				drawimage(button, 25, 50 + (i*13), 350, 11);//button
			}
		}
		drawimage(titre, 107, 5, 185, 20);//titre
		if(menudraw == 0)drawimage(menu3ds, 162, 30, 75, 14);//menu 3ds
		if(menudraw == 1)drawimage(menucia, 165, 30, 81, 14);//menu cia
		if(menudraw == 2)drawimage(menudumpcart, 132, 30, 135, 17);//menu dump crat
		if(count == 0)DrawStringFColor(RED  , TRANSPARENT, 200 - ((16 * 8) / 2), 120, true,"Game Not Found !");
		
		
	//--------------------bottom-------------
	
	ClearScreenFull(false, true);
	snprintf(pathtga, 60, "Game3ds/menu/botbg.tga");
	
	loadtga(false,true,pathtga,0,0);
    
	if(Cart.Cart == 1)titlescreen();
	if(Cart.InitSD == 1)InitFS();
	DrawStringFColor(WHITE, TRANSPARENT, 20, 50, false, "START:Poweroff");
	DrawStringFColor(WHITE, TRANSPARENT, 20, 60, false, "SELECT:Reboot");
	if(menudraw == 0)
    {
		DrawStringFColor(GREEN, TRANSPARENT, 20, 80, false, "A : Convert");
		DrawStringFColor(RED, TRANSPARENT, 20, 90, false, "B : Refresh");
		DrawStringFColor(PURPLE, TRANSPARENT, 20, 110, false, "X : Delete Game");
		DrawStringFColor(WHITE, TRANSPARENT, 20, 140, false, "R1 : Menu CIA");
		DrawStringFColor(WHITE, TRANSPARENT, 20, 150, false, "L1 : Menu Dump Cart");
	}
	if(menudraw == 1)
    {
		DrawStringFColor(RED, TRANSPARENT, 20, 90, false, "B : Refresh");
		DrawStringFColor(PURPLE, TRANSPARENT, 20, 110, false, "X : Delete Game");
		DrawStringFColor(WHITE, TRANSPARENT, 20, 140, false, "R1 : Menu Dump Cart");
		DrawStringFColor(WHITE, TRANSPARENT, 20, 150, false, "L1 : Menu 3DS");
	}
	if(menudraw == 2)
    {
		DrawStringFColor(GREEN, TRANSPARENT, 20, 80, false, "A : Launch");
		DrawStringFColor(RED, TRANSPARENT, 20, 90, false, "B : Refresh cartridge");
		DrawStringFColor(WHITE, TRANSPARENT, 20, 140, false, "R1 : Menu 3DS");
		DrawStringFColor(WHITE, TRANSPARENT, 20, 150, false, "L1 : Menu CIA");
	}
	if (CheckSD()) {
			DrawStringFColor(WHITE, TRANSPARENT, 5, SCREEN_HEIGHT - 30, false, "SD storage:   %lluMB", TotalStorageSpace() / (1024*1024));
			DrawStringFColor(WHITE, TRANSPARENT, 5, SCREEN_HEIGHT - 20, false, "      Used:   %lluMB", TotalStorageSpace() / (1024*1024) - RemainingStorageSpace() / (1024*1024));
			DrawStringFColor(WHITE, TRANSPARENT, 5, SCREEN_HEIGHT - 10, false, "      Free:   %lluMB", RemainingStorageSpace() / (1024*1024));
		
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
		
		menupos.pos = 0;
		
		for (u32 i = 0; i < count; i++) 
		{
			
			drawimage(button, 25, 50 + (i*13), 350, 11);//button
			if(i >= 12)break;
		}
		
		if(index >= 12)menupos.pos = (index - 12);
		
		for (u32 i = 0; i < count; i++) 
		{
			if(menupos.pos != index)
			{
				if(compteur[menupos.pos] >= 37)
				{
					char name[38];
					snprintf(name, 37, "%s",c[menupos.pos]);
					DrawStringFColor(WHITE, TRANSPARENT, 200 - ((39 * 8) / 2), 50 + (i*13 + 2), true, "%s...", name);
				} else {
					DrawStringFColor(WHITE, TRANSPARENT, 200 - ((compteur[menupos.pos] * 8) / 2), 50 + (i*13 + 2), true, "%s", c[menupos.pos]);
				}
			}
			if(menupos.pos == index)
			{
				if(compteur[menupos.pos] >= 37)
				{
					char name[38];
					snprintf(name, 37, "%s",c[menupos.pos]);
					DrawStringFColor(SELECT, TRANSPARENT, 200 - ((39 * 8) / 2), 50 + (i*13 + 2), true, "%s...", name);
				} else {
					DrawStringFColor(SELECT, TRANSPARENT, 200 - ((compteur[menupos.pos] * 8) / 2), 50 + (i*13 + 2), true, "%s", c[menupos.pos]);
				}
			}
			menupos.pos++;
			if(i >= 12)break;
		}
	}
}

	