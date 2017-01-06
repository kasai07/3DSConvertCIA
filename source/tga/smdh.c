#include "common.h"
#include "fs.h"
#include "pathmenu.h"
#include "tga.h"
#include "draw.h"

u32 icone_Large(u32 index, bool topscreen, bool botscreen, u32 posX, u32 posY)
{
	u8 tab2[6912];
	u8 tab3[6912];
	u8 icone[4608];
	u16 Pixel565[2];
	u32 IconePixel;
	u8 header[0x20];
	u8 smdh[0x36C0];
	char path[60];
	snprintf(path, 60, "Game3ds/cia/%s.cia",c[index]);
	FileGetData(path, header, 0x20, 0x0);
	
	if(header[16] == 0x34)
	{
		if(header[25] > 0xC2)header[26]++;
		header[25] = (header[25] + 0x3D);
	}
	if(header[16] == 0x64)
	{
		if(header[24] > 0xBF)header[25]++;
		header[24] = (header[24] + 0x40);
		if(header[25] > 0xC2)header[26]++;
		header[25] = (header[25] + 0x3D);
	}
	if(header[16] == 0x94)
	{
		if(header[24] > 0x7F)header[25]++;
		header[24] = (header[24] + 0x80);
		if(header[25] > 0xC2)header[26]++;
		header[25] = (header[25] + 0x3D);
	}
	
	u32 offset = (header[27]<<24)|(header[26]<<16)|(header[25]<<8)|header[24];
	
	FileGetData(path, smdh, 0x36C0, offset);
	
	for (int y = 0; y < 34; y++)
	{	
	DrawStringFColor(BLACK, TRANSPARENT, 90 + (y*8), 172, true, "%c",smdh[5256 + (y*2)]);
	}
	for (int y = 0; y < 34; y++)
	{	
	DrawStringFColor(BLACK, TRANSPARENT,90 + (y*8), 198, true, "%c",smdh[5512 + (y*2)]);
	}
	
	int count = 0x24C0; 
	
	u8 OrdrePixel[64] = {
	0,  1,  8,  9,  2,  3,  10, 11, 16, 17, 24, 25, 18, 19, 26, 27,
	4,  5,  12, 13, 6,  7,  14, 15, 20, 21, 28, 29, 22, 23, 30, 31,
	32, 33, 40, 41, 34, 35, 42, 43, 48, 49, 56, 57, 50, 51, 58, 59,
	36, 37, 44, 45, 38, 39, 46, 47, 52, 53, 60, 61, 54, 55, 62, 63 };
	for( int y = 0; y < 48; y += 8)
	{
		for(int x = 0; x < 48; x += 8)
		{
			for(int k = 0; k < 64; k++)
			{
				u8 xx =  (OrdrePixel[k] & 0x7);
				u8 yy =  (OrdrePixel[k] >> 3);

				Pixel565[1] = smdh[count++];
				Pixel565[0] = smdh[count++];
				
				IconePixel = (Pixel565[0] << 8 )|Pixel565[1];
				
				u8 red = ((IconePixel & 0xF800) >> 11);  // 5
				u8 green = ((IconePixel & 0x07E0) >> 5); // 6
				u8 blue = (IconePixel & 0x001F);         // 5

				red = red << 3;
				green = green << 2;
				blue = blue << 3;
				
				tab2[(((x + xx) * 48 + (y + yy)) * 3)+0] = blue;
				tab2[(((x + xx) * 48 + (y + yy)) * 3)+1] = green;
				tab2[(((x + xx) * 48 + (y + yy)) * 3)+2] = red;
			}
		}
	}
	
	int n = 0;
	int i = 0;
	int o = 47;
	int j = 48;
	for(int t = 0; t < 48*48; t++)
	{
		tab3[n++] = tab2[(((j*i)+o)*3)+0];
		tab3[n++] = tab2[(((j*i)+o)*3)+1];
		tab3[n++] = tab2[(((j*i)+o)*3)+2];
		i++;
		if(i == 48)
		{
		 o--;
		 i = 0;
		}
	}
		u8 r,g,b,a;
		int dir = 0;
		for(int i = 48; 0 < i; i--)
		{	 
			
			for(int j = 0; j < 48; j++)	
			{
				b = (tab3[dir++]);
				g = (tab3[dir++]);
				r = (tab3[dir++]);
				if(topscreen == true)
				{
					SET_PIXEL(TOP_SCREEN0, (posX+j), (posY+i), RGBCOLOR(r,g,b));
				}
				if(botscreen == true)
				{
					SET_PIXEL(BOT_SCREEN0, (posX+j), (posY+i), RGBCOLOR(r,g,b));
				}
			}			
		}
	
}
u32 icone_Small(u32 index, bool topscreen, bool botscreen, u32 posX, u32 posY)
{
	u8 tab2[6912];
	u8 tab3[6912];
	u8 icone[4608];
	u16 Pixel565[2];
	u32 IconePixel;
	u8 header[0x20];
	u8 smdh[0x36C0];
	char path[60];
	snprintf(path, 60, "Game3ds/cia/%s.cia",c[index]);
	FileGetData(path, header, 0x20, 0x0);
	
	if(header[16] == 0x34)
	{
		if(header[25] > 0xC2)header[26]++;
		header[25] = (header[25] + 0x3D);
	}
	if(header[16] == 0x64)
	{
		if(header[24] > 0xBF)header[25]++;
		header[24] = (header[24] + 0x40);
		if(header[25] > 0xC2)header[26]++;
		header[25] = (header[25] + 0x3D);
	}
	if(header[16] == 0x94)
	{
		if(header[24] > 0x7F)header[25]++;
		header[24] = (header[24] + 0x80);
		if(header[25] > 0xC2)header[26]++;
		header[25] = (header[25] + 0x3D);
	}
	
	u32 offset = (header[27]<<24)|(header[26]<<16)|(header[25]<<8)|header[24];
	
	FileGetData(path, smdh, 0x36C0, offset);
	int count = 0x2040;
	
	u8 OrdrePixel[64] = {
	0,  1,  8,  9,  2,  3,  10, 11, 16, 17, 24, 25, 18, 19, 26, 27,
	4,  5,  12, 13, 6,  7,  14, 15, 20, 21, 28, 29, 22, 23, 30, 31,
	32, 33, 40, 41, 34, 35, 42, 43, 48, 49, 56, 57, 50, 51, 58, 59,
	36, 37, 44, 45, 38, 39, 46, 47, 52, 53, 60, 61, 54, 55, 62, 63 };
	for( int y = 0; y < 24; y += 8)
	{
		for(int x = 0; x < 24; x += 8)
		{
			for(int k = 0; k < 64; k++)
			{
				u8 xx =  (OrdrePixel[k] & 0x7);
				u8 yy =  (OrdrePixel[k] >> 3);

				Pixel565[1] = smdh[count++];
				Pixel565[0] = smdh[count++];
				
				IconePixel = (Pixel565[0] << 8 )|Pixel565[1];
				
				u8 red = ((IconePixel & 0xF800) >> 11);  // 5
				u8 green = ((IconePixel & 0x07E0) >> 5); // 6
				u8 blue = (IconePixel & 0x001F);         // 5

				red = red << 3;
				green = green << 2;
				blue = blue << 3;
				
				tab2[(((x + xx) * 24 + (y + yy)) * 3)+0] = blue;
				tab2[(((x + xx) * 24 + (y + yy)) * 3)+1] = green;
				tab2[(((x + xx) * 24 + (y + yy)) * 3)+2] = red;
			}
		}
	}
	
	int n = 0;
	int i = 0;
	int o = 23;
	int j = 24;
	for(int t = 0; t < 24*24; t++)
	{
		tab3[n++] = tab2[(((j*i)+o)*3)+0];
		tab3[n++] = tab2[(((j*i)+o)*3)+1];
		tab3[n++] = tab2[(((j*i)+o)*3)+2];
		i++;
		if(i == 24)
		{
		 o--;
		 i = 0;
		}
	}
		u8 r,g,b,a;
		int dir = 0;
		for(int i = 24; 0 < i; i--)
		{	 
			
			for(int j = 0; j < 24; j++)	
			{
				b = (tab3[dir++]);
				g = (tab3[dir++]);
				r = (tab3[dir++]);
				if(topscreen == true)
				{
					SET_PIXEL(TOP_SCREEN0, (posX+j), (posY+i), RGBCOLOR(r,g,b));
				}
				if(botscreen == true)
				{
					SET_PIXEL(BOT_SCREEN0, (posX+j), (posY+i), RGBCOLOR(r,g,b));
				}
			}			
		}
	
}
