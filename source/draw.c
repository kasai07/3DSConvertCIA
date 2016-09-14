// Copyright 2013 Normmatt
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "font.h"
#include "draw.h"
#include "fs.h"

#include "fatfs/ff.h"

#define STD_COLOR_BG   COLOR_BLACK
#define STD_COLOR_FONT COLOR_WHITE

#define DBG_COLOR_BG   COLOR_BLACK
#define DBG_COLOR_FONT COLOR_WHITE

#define DBG_START_Y 10
#define DBG_END_Y   (SCREEN_HEIGHT - 10)
#define DBG_START_X 10
#define DBG_END_X   (SCREEN_WIDTH_TOP - 10)
#define DBG_STEP_Y  10

#define DBG_N_CHARS_Y   ((DBG_END_Y - DBG_START_Y) / DBG_STEP_Y)
#define DBG_N_CHARS_X   (((DBG_END_X - DBG_START_X) / 8) + 1)

static char debugstr[DBG_N_CHARS_X * DBG_N_CHARS_Y] = { 0 };

void ClearScreen(u8* screen, int width, int color)
{
    if (color == COLOR_TRANSPARENT) color = COLOR_BLACK;
    for (int i = 0; i < (width * SCREEN_HEIGHT); i++) {
        *(screen++) = color >> 16;  // B
        *(screen++) = color >> 8;   // G
        *(screen++) = color & 0xFF; // R
    }
}

void ClearScreenFull(bool clear_top, bool clear_bottom)
{
    if (clear_top) {
        ClearScreen(TOP_SCREEN0, SCREEN_WIDTH_TOP, STD_COLOR_BG);
        ClearScreen(TOP_SCREEN1, SCREEN_WIDTH_TOP, STD_COLOR_BG);
    }
    if (clear_bottom) {
        ClearScreen(BOT_SCREEN0, SCREEN_WIDTH_BOT, STD_COLOR_BG);
        ClearScreen(BOT_SCREEN1, SCREEN_WIDTH_BOT, STD_COLOR_BG);
    }
}

void DrawCharacter(u8* screen, int character, int x, int y, int color, int bgcolor)
{
    for (int yy = 0; yy < 8; yy++) {
        int xDisplacement = (x * BYTES_PER_PIXEL * SCREEN_HEIGHT);
        int yDisplacement = ((SCREEN_HEIGHT - (y + yy) - 1) * BYTES_PER_PIXEL);
        u8* screenPos = screen + xDisplacement + yDisplacement;

        u8 charPos = font[character * 8 + yy];
        for (int xx = 7; xx >= 0; xx--) {
            if ((charPos >> xx) & 1) {
                *(screenPos + 0) = color >> 16;  // B
                *(screenPos + 1) = color >> 8;   // G
                *(screenPos + 2) = color & 0xFF; // R
            } else if (bgcolor != COLOR_TRANSPARENT) {
                *(screenPos + 0) = bgcolor >> 16;  // B
                *(screenPos + 1) = bgcolor >> 8;   // G
                *(screenPos + 2) = bgcolor & 0xFF; // R
            }
            screenPos += BYTES_PER_PIXEL * SCREEN_HEIGHT;
        }
    }
}

void DrawString(u8* screen, const char *str, int x, int y, int color, int bgcolor)
{
    for (size_t i = 0; i < strlen(str); i++)
        DrawCharacter(screen, str[i], x + i * 8, y, color, bgcolor);
}

void DrawStringF(int x, int y, bool use_top, const char *format, ...)
{
    char str[512] = { 0 }; // 512 should be more than enough
    va_list va;

    va_start(va, format);
    vsnprintf(str, 512, format, va);
    va_end(va);

    for (char* text = strtok(str, "\n"); text != NULL; text = strtok(NULL, "\n"), y += 10) {
        if (use_top) {
            DrawString(TOP_SCREEN0, text, x, y, STD_COLOR_FONT, STD_COLOR_BG);
            DrawString(TOP_SCREEN1, text, x, y, STD_COLOR_FONT, STD_COLOR_BG);
        } else {
            DrawString(BOT_SCREEN0, text, x, y, STD_COLOR_FONT, STD_COLOR_BG);
            DrawString(BOT_SCREEN1, text, x, y, STD_COLOR_FONT, STD_COLOR_BG);
        }
    }
}
void DrawStringFColor(int colorfont, int colorbg, int x, int y, bool use_top, const char *format, ...)
{
    char str[512] = { 0 }; // 512 should be more than enough
    va_list va;

    va_start(va, format);
    vsnprintf(str, 512, format, va);
    va_end(va);

    for (char* text = strtok(str, "\n"); text != NULL; text = strtok(NULL, "\n"), y += 10) {
        if (use_top) {
            DrawString(TOP_SCREEN0, text, x, y, colorfont, colorbg);
            DrawString(TOP_SCREEN1, text, x, y, colorfont, colorbg);
        } else {
            DrawString(BOT_SCREEN0, text, x, y, colorfont, colorbg);
            DrawString(BOT_SCREEN1, text, x, y, colorfont, colorbg);
        }
    }
}
void Screenshot(const char* path)
{
	//pour le splash.bin
	/*loadSplash();*/
    u8* buffer = (u8*) 0x21000000; 
    u8* buffer_t = buffer + (400 * 240 * 3);
    u8 bmp_header[54] = {
        0x42, 0x4D, 0x36, 0xCA, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
        0x00, 0x00, 0x90, 0x01, 0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xCA, 0x08, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
    if (!CheckSD()) // not mounted, nothing to do
        return;
    
    if (path == NULL) {
        static u32 n = 0;
        for (; n < 1000; n++) {
            char filename[16];
            snprintf(filename, 16, "snap%03i.bmp", (int) n);
            if (!FileOpen(filename)) {
                FileCreate(filename, true);
                break;
            }
            FileClose();
        }
        if (n >= 1000)
            return;
    } else {
        FileCreate(path, true);
    }
    
    memset(buffer, 0x1F, 400 * 240 * 3 * 2);
   
 for (u32 x = 0; x < 400; x++)
        
		for (u32 y = 0; y < 240; y++)
            memcpy(buffer_t + (y*400 + x) * 3, TOP_SCREEN0 + (x*240 + y) * 3, 3);
   
 for (u32 x = 0; x < 320; x++)
       
 for (u32 y = 0; y < 240; y++)
            memcpy(buffer + (y*400 + x + 40) * 3, BOT_SCREEN0 + (x*240 + y) * 3, 3);
   
 FileWrite(bmp_header, 54, 0);
    FileWrite(buffer, 400 * 240 * 3 * 2, 54);
    FileClose();
}

void DebugClear()
{
    memset(debugstr, 0x00, DBG_N_CHARS_X * DBG_N_CHARS_Y);
    ClearScreen(TOP_SCREEN0, SCREEN_WIDTH_TOP, DBG_COLOR_BG);
    ClearScreen(TOP_SCREEN1, SCREEN_WIDTH_TOP, DBG_COLOR_BG);
    
}



void Debug(const char *format, ...)
{
    static bool adv_output = true;
    char tempstr[DBG_N_CHARS_X] = { 0 };
    va_list va;
    
    va_start(va, format);
    vsnprintf(tempstr, DBG_N_CHARS_X - 1, format, va);
    va_end(va);
    
    if (adv_output) {
        memmove(debugstr + DBG_N_CHARS_X, debugstr, DBG_N_CHARS_X * (DBG_N_CHARS_Y - 1));
    } else {
        adv_output = true;
    }
    
    if (*tempstr != '\r') { // not a good way of doing this - improve this later
        snprintf(debugstr, DBG_N_CHARS_X, "%-*.*s", DBG_N_CHARS_X - 1, DBG_N_CHARS_X - 1, tempstr);
    } else {
        snprintf(debugstr, DBG_N_CHARS_X, "%-*.*s", DBG_N_CHARS_X - 1, DBG_N_CHARS_X - 1, tempstr + 1);
        adv_output = false;
    }
    
    int pos_y = DBG_START_Y;
    for (char* str = debugstr + (DBG_N_CHARS_X * (DBG_N_CHARS_Y - 1)); str >= debugstr; str -= DBG_N_CHARS_X) {
        if (str[0] != '\0') {
            DrawString(TOP_SCREEN0, str, DBG_START_X, pos_y, DBG_COLOR_FONT, DBG_COLOR_BG);
            DrawString(TOP_SCREEN1, str, DBG_START_X, pos_y, DBG_COLOR_FONT, DBG_COLOR_BG);
            pos_y += DBG_STEP_Y;
        }
    }
}
void DebugColor(int colorfont, int colorbg, u8* screen, const char *format, ...)
{
    static bool adv_output = true;
    char tempstr[DBG_N_CHARS_X] = { 0 };
    va_list va;
    
    va_start(va, format);
    vsnprintf(tempstr, DBG_N_CHARS_X - 1, format, va);
    va_end(va);
    
    if (adv_output) {
        memmove(debugstr + DBG_N_CHARS_X, debugstr, DBG_N_CHARS_X * (DBG_N_CHARS_Y - 1));
    } else {
        adv_output = true;
    }
    
    if (*tempstr != '\r') { // not a good way of doing this - improve this later
        snprintf(debugstr, DBG_N_CHARS_X, "%-*.*s", DBG_N_CHARS_X - 1, DBG_N_CHARS_X - 1, tempstr);
    } else {
        snprintf(debugstr, DBG_N_CHARS_X, "%-*.*s", DBG_N_CHARS_X - 1, DBG_N_CHARS_X - 1, tempstr + 1);
        adv_output = false;
    }
    
    int pos_y = DBG_START_Y;
    for (char* str = debugstr + (DBG_N_CHARS_X * (DBG_N_CHARS_Y - 1)); str >= debugstr; str -= DBG_N_CHARS_X) {
        if (str[0] != '\0') {
            DrawString(screen, str, DBG_START_X, pos_y, colorfont, colorbg);
            DrawString(screen, str, DBG_START_X, pos_y, colorfont, colorbg);
            pos_y += DBG_STEP_Y;
        }
    }
}

static FIL file;
void loadSplash(){
    //If FB was clear, and the image exists, display
    ClearScreenFull(true, true);
        if(f_open(&file, "/Launcher_A9LH/splash.bin", FA_READ) == FR_OK)
		{
		
			UINT br;
			f_read(&file, TOP_SCREEN0, 0x288000 , &br);
			
			
			f_close(&file);
		}
        u64 i = 0xFFFFFF; while(--i) __asm("mov r0, r0"); //Less Ghetto sleep func
    
}

u32 dumpram()
{

	
	char bin[8];
	snprintf(bin, 8, "ram.bin");
	FileCreate(bin, true);
	FileWrite((u8*) 0x28000000, 0x4000000,0);
    FileWrite((u8*) 0x32000000, 0x4000000,0x4000000);
    FileClose();
}