#include "hid.h"
#include "draw.h"
#include "timer.h"
#include "fs.h"

u32 InputWait() {
    static u64 delay = 0;
    u32 pad_state_old = HID_STATE;
    delay = (delay) ? 72 : 128;
    timer_start();
	u32 HIDFlag = 0;
	
	while (true) {
		
		padflag_state = 0;
		HIDFlag = HID_Flag();
		if (HIDFlag == 0x01){padflag_state = BUTTON_POWER;return 0;}
		if (HIDFlag == 0x04){padflag_state = BUTTON_HOME;return 0;}
		if (HIDFlag == 0x10){padflag_state = BUTTON_WIFI;return 0;}
		
		
		u32 pad_state = HID_STATE;
		if (!(~pad_state & BUTTON_ANY)) { // no buttons pressed
            pad_state_old = pad_state;
            delay = 0;
			continue;
		}
		if ((pad_state == pad_state_old) && 
		(!(~pad_state & BUTTON_ARROW) || 
		(delay && (timer_msec() < delay)))) 
		continue;
		//Make sure the key is pressed
        u32 t_pressed = 0;
        for(; (t_pressed < 0x13000) && (pad_state == HID_STATE); t_pressed++);
		
		if (t_pressed >= 0x13000)return ~pad_state;
	}
}


