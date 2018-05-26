#include "libs/SMSlib.h"
#include "libs/PSGlib.h"
#include "soundengine.h"
#include "bank2.h"
#include "bank3.h"
#include "bank4.h"

unsigned char current_music_bank = 0;
unsigned char current_fx_bank = 0;

void play_logo_music() {
	SMS_mapROMBank(logo1985music_psg_bank);
	current_music_bank = logo1985music_psg_bank;
	PSGPlay(logo1985music_psg);
}

void play_presentation_music() {
	SMS_mapROMBank(presentation_psg_bank);
	current_music_bank = presentation_psg_bank;
	PSGPlay(presentation_psg);
}

void play_game_music() {
	SMS_mapROMBank(level1music_psg_bank);
	current_music_bank = level1music_psg_bank;
	PSGPlay(level1music_psg);
}

void play_punch_sound1() {
	SMS_mapROMBank(punch1_psg_bank);
	current_fx_bank = punch1_psg_bank;
	PSGSFXPlay(punch1_psg, SFX_CHANNEL2);
}

void play_punch_sound2() {
	SMS_mapROMBank(punch2_psg_bank);
	current_fx_bank = punch2_psg_bank;
	PSGSFXPlay(punch1_psg, SFX_CHANNEL3);
}

void manage_sound_frame() {
	if(PSGGetStatus() == PSG_PLAYING){
	    SMS_mapROMBank(current_music_bank);
	    PSGFrame();
	}
	if(PSGSFXGetStatus() == PSG_PLAYING){
	    SMS_mapROMBank(current_fx_bank);
	    PSGSFXFrame();
	} else {
	    PSGSFXStop();
	}
}