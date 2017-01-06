#include "hid.h"
#include "draw.h"
#include "timer.h"
#include "fs.h"
#include "i2c.h"

u32 InputWait() {
    static u64 delay = 0;
    u32 pad_state_old = HID_STATE;
    delay = (delay) ? 72 : 128;
    timer_start();
	
	while (true) {
		
		
		//Level Battery
		u8 levelbattery = BatteryLevel();
		if(levelbattery > 10){DrawStringFColor(GREEN, BLACK, 10, 10, false, "Level Battery : %lu%%",levelbattery);}
		else if(levelbattery < 11){DrawStringFColor(RED, BLACK, 10, 10, false, "Level Battery: %lu%%",levelbattery);}
		/*
		//status battery charge
		if (Batterycharge() == 0xFA){DrawStringFColor(GREEN, BLACK, 186, 10, false, "En Charge");}
		else{DrawStringFColor(BLACK, BLACK, 186, 10, false, "         ");}
		
		//status SD
		if (EMMC_STATUS & SD_CARD_INSERT)
		{DrawStringFColor(GREEN, BLACK, 5, 200, false, "SD OK!");}
		else{DrawStringFColor(RED, BLACK, 5, 200, false, "No SD!");}
		*/
		u32 pad_state = HID_STATE;
		
		u8 Special_HID = i2cReadRegister(I2C_DEV_MCU, 0x10);
		if (Special_HID & BIT_0){return ~pad_state + 0x00002000;}//button power 0x01
		if (Special_HID & BIT_2){return ~pad_state + 0x00003000;}//button home 0x04
		if (Special_HID & BIT_4){return ~pad_state + 0x00005000;}//button wifi 0x10
		
		if (TOUCH_SCREEN == 0x01){return ~pad_state + 0x00009000;}//button touch screen
		
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
		
		if (t_pressed >= 0x13000)return ~pad_state + 0x00001000;
	}
}

