
#define bool	_Bool
#define true	1
#define false	0

typedef __UINT8_TYPE__ __uint8_t;
typedef __uint8_t ui8;
typedef __UINT32_TYPE__ __uint32_t;
typedef __uint32_t ui32 ;
typedef unsigned int	uint;
typedef unsigned char	uchar;

#define SUCCESS_OK 0

#define RGBCOLOR(r,g,b) (r<<24|b<<16|g<<8|r)
#define TOP_RIGHT_SCREEN (ui8*)(*(ui32*)0x23FFFE00)
#define TOP_LEFT_SCREEN (ui8*)(*(ui32*)0x23FFFE04)
#define BOTTOM_SCREEN (ui8*)(*(ui32*)0x23FFFE08)

#define BPP 3
#define HEIGHT 240
#define SET_PIXEL(buffer, x, y, rgb)\
{\
	ui32 offset = (HEIGHT * x + HEIGHT - y ) * BPP;\
	*((ui8*)buffer + offset++) = rgb >> 16;\
	*((ui8*)buffer + offset++) = rgb >> 8;\
	*((ui8*)buffer + offset++) = rgb & 0xFF;\
}

#define PATHLOGO			"/Launcher_A9LH/logo"

#define MENU "Menu.tga"
#define TGA ".tga"

typedef struct
{
	uchar id_lenght;
	uchar colormap_type;
	uchar image_type;
	uchar cm_first1;
	uchar cm_first2;
	uchar cm_lenght1;
	uchar cm_lenght2;
	uchar cm_size;
	uchar x_origin1;
	uchar x_origin2;
	uchar y_origin1;
	uchar y_origin2;
	uchar width1;
	uchar width2;
	uchar height1;
	uchar height2;
	uchar pixel_depth;
	uchar image_descriptor;
	char data[288000];//400 * 240 * 3 = 288000
}tga_header_t;





void loadtga(bool top_screen, bool bot_screen, char* path, int poswidth, int posheight);

ui32 fileRead(void *dest, const char *path);
