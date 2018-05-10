#include <stdbool.h>
#include "resources.h"
#include "libs/SMSlib.h"
#include "libs/PSGlib.h"
#include "bank2.h" 
#include "bank3.h"
#include "montylib.h"
#include "spriteengine.h"

unsigned int scroll_x;
unsigned int scroll_y;

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

void load_fnaclogo_assets() {
	SMS_mapROMBank(fnaclogotiles_psgcompr_bank);
    SMS_loadPSGaidencompressedTiles(fnaclogotiles_psgcompr,0);
    SMS_loadTileMap(0, 0, fnaclogotilemap_bin, fnaclogotilemap_bin_size);
}

void load_fnaclogo_halfpalette() {
	SMS_loadBGPaletteHalfBrightness(fnaclogopalette_bin);
}

void load_fnaclogo_fullpalette() {
	SMS_loadBGPalette(fnaclogopalette_bin);
}

void load_level1_assets() {
	SMS_mapROMBank(level1_palette_bin_bank);
	SMS_loadBGPalette(level1_palette_bin);
    SMS_loadTiles(level1_tiles_bin,0, level1_tiles_bin_size);
    SMS_mapROMBank(player_palette_bin_bank);
    SMS_loadSpritePalette(player_palette_bin);
}

void init_level1_scroll() {
	scroll_x = 0;
	scroll_y = 0;
	init_scroll(&level1_scrolltable_bin, &level1_metatiles_bin, scroll_x, scroll_y);
}

void update_scroll(signed char delta_x, signed char delta_y) {
	move_scroll(delta_x, delta_y);
}


void update_resources() {
	updateTiles();
	updateAnimations();
	drawEntities();
}

void load_background_blackpalette() {
	SMS_zeroBGPalette();
}

void load_system_font() {
	SMS_mapROMBank(systemfont_psgcompr_bank);
	SMS_loadPSGaidencompressedTiles(systemfont_psgcompr,0);
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