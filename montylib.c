//
// Created by Jordi Montornes on 04/04/2017.
//
#include <stdbool.h>
#include "montylib.h"
#include "SMSlib.h"
#include "PSGlib.h"
#include "randLut.h"

bool pause;
unsigned char rand_index =  0;
unsigned int frame_cnt;


void init_console() {
    SMS_init();
    SMS_setSpriteMode(SPRITEMODE_NORMAL);
    SMS_displayOn();
    SMS_VDPturnOnFeature(VDPFEATURE_LEFTCOLBLANK);
    SMS_setBackdropColor(1);
    pause = false;
}

void waitForFrame(){
    if(!pause) {
        if(PSGGetStatus() == PSG_PLAYING){
            PSGFrame();
        }
        if(PSGSFXGetStatus() == PSG_PLAYING){
            PSGSFXFrame();
        } else {
            PSGSFXStop();
        }
    }
    SMS_waitForVBlank();
    SMS_copySpritestoSAT();
}

unsigned char rand() {

    return randLUT[rand_index++];
}
