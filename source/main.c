#include "common.h"
#include "draw.h"
#include "fs.h"
#include "menu.h"





int main()
{
    
	ClearScreenFull(true, true);
	DebugClear();
	InitFS();
	InitKeys();
    
	Menu3DS();
    
	return 0;
}
