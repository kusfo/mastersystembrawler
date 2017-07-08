#include <stdbool.h>
#include "resources.h"
#include "SMSlib.h"
#include "PSGlib.h"
#include "bank2.h" 
#include "bank3.h"
#include "spriteengine.h"

unsigned char current_music_bank = 0;

void load_logo_assets() {
	SMS_mapROMBank(logo1985tiles_bin_bank);
    SMS_loadTiles(logo1985tiles_bin, 0, logo1985tiles_bin_size);
    SMS_loadTileMap(0, 0, logo1985tilemap_bin, logo1985tilemap_bin_size);
}

void load_logo_halfpalette() {
	SMS_loadBGPaletteHalfBrightness(logo1985palette_bin);
}

void load_logo_fullpalette() {
	SMS_loadBGPalette(logo1985palette_bin);
}

void load_level1_assets() {
	SMS_mapROMBank(levelbackgroundpalette_bin_bank);
	SMS_loadBGPalette(levelbackgroundpalette_bin);
    SMS_loadPSGaidencompressedTiles(levelbackgroundtiles_psgcompr,0);
    SMS_loadTileMap(0, 0, levelbackgroundtilemap_bin, levelbackgroundtilemap_bin_size);
   	SMS_mapROMBank(systemfont_psgcompr_bank);
    SMS_loadPSGaidencompressedTiles(systemfont_psgcompr,80);
    SMS_mapROMBank(player_palette_bin_bank);
    SMS_loadSpritePalette(player_palette_bin);
}


void update_resources() {
	updateTiles();
	updateAnimations();
	drawEntities();
}

void load_background_blackpalette() {
	SMS_zeroBGPalette();
}

void play_logo_music() {
	SMS_mapROMBank(logo1985music_psg_bank);
	current_music_bank = logo1985music_psg_bank;
	PSGPlay(logo1985music_psg);
}

void play_game_music() {
	SMS_mapROMBank(level1music_psg_bank);
	current_music_bank = level1music_psg_bank;
	PSGPlay(level1music_psg);
}

void load_system_font() {
	SMS_mapROMBank(systemfont_psgcompr_bank);
	SMS_loadPSGaidencompressedTiles(systemfont_psgcompr,0);
}

unsigned char get_music_bank() {
	return current_music_bank;
}

void print_debug_info() {
	unsigned char i;
	i = 0;
	//print_unsigned_char(entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].framelist[entitylist[i]->currentFrame].frame_time);
}

void print_unsigned_char(unsigned char number) {
	unsigned char tmpHundreds, tmpTens, tmpUnits;
	SMS_setNextTileatXY(1,0);
	tmpHundreds = number / 100;
	SMS_setTile(96 + tmpHundreds);
	tmpTens = (number - (tmpHundreds * 100)) / 10;
	SMS_setTile(96 + tmpTens);
	tmpUnits = (number - (tmpHundreds * 100) - (tmpTens * 10));
	SMS_setTile(96 + tmpUnits);
}

void print_signed_char(signed char number) {
	unsigned char tmpHundreds, tmpTens, tmpUnits;
	SMS_setNextTileatXY(1,0);
	if(number < 0) {
		SMS_setTile(93);
		number = number * -1;	
	}
	tmpHundreds = number / 100;
	SMS_setTile(96 + tmpHundreds);
	tmpTens = (number - (tmpHundreds * 100)) / 10;
	SMS_setTile(96 + tmpTens);
	tmpUnits = (number - (tmpHundreds * 100) - (tmpTens * 10));
	SMS_setTile(96 + tmpUnits);	
}