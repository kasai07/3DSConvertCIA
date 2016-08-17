// Copyright 2013 Normmatt
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "common.h"

#define BYTES_PER_PIXEL 3
#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH_TOP 400
#define SCREEN_WIDTH_BOT 320

#define RGB(r,g,b) (r<<24|b<<16|g<<8|r)

#define RED 			COLOR_RED
#define WHITE 			COLOR_WHITE
#define BLACK 			COLOR_BLACK
#define TRANSPARENT 	COLOR_TRANSPARENT
#define BLUE			COLOR_BLUE
#define GREEN			COLOR_GREEN
#define YELLOW			COLOR_YELLOW
#define PURPLE			COLOR_PURPLE
#define GREY			COLOR_GREY

#define SELECT			COLOR_SELECT

#define COLOR_SELECT        RGB(0xD7, 0x9A, 0x28)

#define COLOR_YELLOW        RGB(0xFF, 0xFF, 0x00)
#define COLOR_BLACK         RGB(0x00, 0x00, 0x00)
#define COLOR_WHITE         RGB(0xFF, 0xFF, 0xFF)
#define COLOR_RED           RGB(0xFF, 0x00, 0x00)
#define COLOR_GREEN         RGB(0x00, 0xFF, 0x00)
#define COLOR_BLUE          RGB(0x00, 0x00, 0xFF)
#define COLOR_GREY          RGB(0x77, 0x77, 0x77)
#define COLOR_PURPLE        RGB(0xff, 0x00, 0xFF)
#define COLOR_TRANSPARENT   RGB(0xFF, 0x00, 0xEF) // otherwise known as 'super fuchsia'

#ifdef EXEC_GATEWAY
	#define TOP_SCREEN0 (u8*)(*(u32*)((uint32_t)0x080FFFC0 + 4 * (*(u32*)0x080FFFD8 & 1)))
	#define BOT_SCREEN0 (u8*)(*(u32*)((uint32_t)0x080FFFD0 + 4 * (*(u32*)0x080FFFDC & 1)))
	#define TOP_SCREEN1 TOP_SCREEN0
	#define BOT_SCREEN1 BOT_SCREEN0
#elif defined(EXEC_A9LH)
	#define TOP_SCREEN0 (u8*)(*(u32*)0x23FFFE00)
	#define TOP_SCREEN1 (u8*)(*(u32*)0x23FFFE00)
	#define BOT_SCREEN0 (u8*)(*(u32*)0x23FFFE08)
	#define BOT_SCREEN1 (u8*)(*(u32*)0x23FFFE08)
#else
	#error "Unknown execution method"
#endif

void ClearScreen(unsigned char *screen, int width, int color);
void ClearScreenFull(bool clear_top, bool clear_bottom);

void DrawCharacter(unsigned char *screen, int character, int x, int y, int color, int bgcolor);
void DrawString(unsigned char *screen, const char *str, int x, int y, int color, int bgcolor);
void DrawStringF(int x, int y, bool use_top, const char *format, ...);
void DrawStringFColor(int colorfont, int colorbg, int x, int y, bool use_top, const char *format, ...);


void Screenshot(const char* path);
void DebugClear();
void Debug(const char *format, ...);
void DebugColor(int colorfont, int colorbg, u8* screen, const char *format, ...);


void ShowProgress(u64 current, u64 total);

void loadSplash();