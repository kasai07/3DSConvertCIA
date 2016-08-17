#include "tga.h"
#include "fatfs/ff.h"

static FIL file;

ui32 fileRead(void *dest, const char *path)
{
    
   

    if(f_open(&file, path, FA_READ) == FR_OK)
    {
	
		f_lseek(&file, 0);
		f_sync(&file);
		uint fsize = f_size(&file);
		
		uint bytes_read;
       
		f_read(&file, dest, fsize, &bytes_read);
        
		f_close(&file);
    }
    else return 1;

    return 0;
}



void loadtga(bool top_screen, bool bot_screen, char* path, int poswidth, int posheight)
{	
	int point0 = 0;
	tga_header_t data;
	int r, g, b, a = 1;
	int dir = 0;
						
	
	if(fileRead(&data, path) == SUCCESS_OK)
	{
		
		int width  = data.width2 * 256 + data.width1;					
		int height = data.height2 * 256 + data.height1;
	
	
		for(int i = height; point0 < i; i--)
		{	 
			for(int j = 0; j < width; j++)	
			{
				if(data.pixel_depth == 24)//24bits
				{
					b = (data.data[dir++]);
					g = (data.data[dir++]);
					r = (data.data[dir++]);
					
					
				}	
				if(data.pixel_depth == 32)//32bits
				{
					
					b = (data.data[dir++]);
					g = (data.data[dir++]);
					r = (data.data[dir++]);
					a = (data.data[dir++]);
					
					
				}
				
				if(top_screen)//0 top screen
				{
					if(a == 0)
					{
						ui32 passe = (HEIGHT * poswidth+j + HEIGHT - posheight+i ) * BPP;
						*((ui8*)TOP_RIGHT_SCREEN + passe++);
						*((ui8*)TOP_RIGHT_SCREEN + passe++);
						*((ui8*)TOP_RIGHT_SCREEN + passe++);
						
						
					}else
					{
						SET_PIXEL(TOP_RIGHT_SCREEN, (poswidth+j), (posheight+i), RGBCOLOR(r,g,b));
						SET_PIXEL(TOP_LEFT_SCREEN, (poswidth+j), (posheight+i), RGBCOLOR(r,g,b));
					}
				}
				if(bot_screen)//0 bot scrren
				{
					
					if(a == 0)
					{
						ui32 passe = (HEIGHT * poswidth+j + HEIGHT - posheight+i ) * BPP;
						*((ui8*)BOTTOM_SCREEN + passe++);
						*((ui8*)BOTTOM_SCREEN + passe++);
						*((ui8*)BOTTOM_SCREEN + passe++);
						
						
					}else
					{
						SET_PIXEL(BOTTOM_SCREEN, (poswidth+j), (posheight+i), RGBCOLOR(r,g,b));
					}
				}
				
				
			}			
		}
		
	}
}
