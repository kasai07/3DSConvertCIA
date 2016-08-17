#include "fs.h"
#include "draw.h"
#include "pathmenu.h"

u32 PathMenu3ds(const char *path)
{

	
	int lsize = 0x1000;
	char* list = (char*) 0x21000000;
	memcpy(list, "\0", 0x21000);
	
	
	GetFileListp("/Game3ds/3ds", list, lsize, false);
	
	u32 j = 0;
	int countpay = 0;
	
	for(int i = 0; i < 30; i++)
	{	
		c[i][0] = 0;
	}
	for(int i = 0; i < 13; i++)
	{	
		list++;
	}

	while(*list != '\0')
	{

		if(*list == '\n')
		{
			
			c[countpay][j-4] = '\0';
			compteur[countpay] = j-4;
			j = 0;
			countpay++;
			
			for(int g = 0; g < 14; g++)
			{	
				list++;
			}
		}
		c[countpay][j] = list[0];
		list++;
		j++;
		
	}

  return countpay;
}
u32 PathMenucia()
{

	
	int lsize = 0x1000;
	char* list = (char*) 0x21000000;
	memcpy(list, "\0", 0x21000);
	
	
	GetFileListp("/Game3ds/cia", list, lsize, false);
	
	u32 j = 0;
	int countpay = 0;
	
	for(int i = 0; i < 30; i++)
	{	
		c[i][0] = 0;
	}

	for(int i = 0; i < 13; i++)
	{	
		list++;
	}

	while(*list != '\0')
	{

		if(*list == '\n')
		{
			
			c[countpay][j-4] = '\0';
			compteur[countpay] = j-4;
			j = 0;
			countpay++;
			
			for(int g = 0; g < 14; g++)
			{	
				list++;
			}
		}
		c[countpay][j] = list[0];
		list++;
		j++;
		
	}

  return countpay;
}