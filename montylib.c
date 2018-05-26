//
// Created by Jordi Montornes on 04/04/2017.
//
#include <stdbool.h>
#include "montylib.h"
#include "libs/SMSlib.h"
#include "libs/PSGlib.h"
#include "libs/GSlib.h"
#include "randLut.h"
#include "soundengine.h"

bool pause;
unsigned char rand_index =  0;
unsigned int frame_cnt;


void init_console() {
    SMS_init();
    SMS_VRAMmemset(0x4000, 0x00, 0x4000);
    SMS_VRAMmemset(0xC000, 0x00, 0x0020);
    SMS_setSpriteMode(SPRITEMODE_NORMAL);
    SMS_displayOn();
    SMS_VDPturnOnFeature(VDPFEATURE_LEFTCOLBLANK);
    SMS_setBackdropColor(0);
    pause = false;
}

void init_scroll(void *scrolltable, void *metatiles, unsigned int scroll_x, unsigned int scroll_y) {
    GSL_initializeMap(scrolltable, metatiles);
    GSL_positionWindow(scroll_x,scroll_y);
    GSL_refreshVDP();
}

void move_scroll(signed char delta_x, signed char delta_y) {
    signed int currentx = GSL_getCurrentX();
    signed int currenty = GSL_getCurrentY();
    if((currentx + delta_x) < 0) delta_x = 0;
    if((currentx + delta_x) > (GSL_getMapWidthInPixels() - 256)) delta_x = 0;
    if((currenty + delta_y) < 0) delta_y = 0;
    if((currenty + delta_y) > (GSL_getMapHeightInPixels() - 192)) delta_y = 0;
    
    GSL_scroll(delta_x,delta_y);
}

unsigned int get_scroll_x() {
    return GSL_getCurrentX();
}

unsigned int get_scroll_y() {
    return GSL_getCurrentY();
}

void waitForFrame(){
    if(!pause) {
        manage_sound_frame();
    }
    SMS_waitForVBlank();
    GSL_VBlank();
    SMS_copySpritestoSAT();
}

unsigned char rand() {

    return randLUT[rand_index++];
}
