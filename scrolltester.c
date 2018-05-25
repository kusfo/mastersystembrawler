#include "bank2.h"
#include "libs\SMSlib.h"
#include "libs\GSLib.h"

signed char scrollXOffset = 0;
signed char scrollYOffset = 0;



void processUpKey()
{
	unsigned int currenty = GSL_getCurrentY();
	scrollXOffset = 0;
	if (currenty > 1)
	{
		scrollYOffset = -2;
	}
	else
	{
		scrollYOffset = 0;
	}
}

void processDownKey()
{
	unsigned int currenty = GSL_getCurrentY();
	scrollXOffset = 0;
	if (currenty < GSL_getMapHeightInPixels() - 3)
	{
		scrollYOffset = 2;
	}
	else
	{
		scrollYOffset = 0;
	}
}

void processLeftKey() 
{
	unsigned int currentx = GSL_getCurrentX();
	scrollYOffset = 0;
	if (currentx > 1)
	{
		scrollXOffset = -2;
	}
	else
	{
		scrollXOffset = 0;
	}
}

void processRightKey() 
{
	unsigned int currentx = GSL_getCurrentX();
	scrollYOffset = 0;
	if (currentx < GSL_getMapWidthInPixels() - 3)
	{
		scrollXOffset = 2;
	}
	else
	{
		scrollXOffset = 0;
	}
}

void processUserInput(unsigned int ks)
{
	scrollXOffset = 0;
	scrollYOffset = 0;
	
	if (ks & PORT_A_KEY_UP)
	{
		processUpKey();
		return;
	}
	
	else if (ks & PORT_A_KEY_DOWN)
	{
		processDownKey();
		return;
	}
	
	else if (ks & PORT_A_KEY_LEFT)
	{
		processLeftKey();
		return;
	}
	
	else if (ks & PORT_A_KEY_RIGHT)
	{
		processRightKey();
		return;
	}
}

void main(void) 
{
	unsigned int ks; 
	
	SMS_VRAMmemset(0x4000, 0x00, 0x4000);
	SMS_VRAMmemset(0xC000, 0x00, 0x0020);
	SMS_loadTiles(&level1_tiles_bin, 0, level1_tiles_bin_size);
	SMS_loadBGPalette(&level1_palette_bin);

	GSL_initializeMap(&level1_scrolltable_bin, &level1_metatiles_bin);
	GSL_positionWindow(0,0);
	GSL_refreshVDP();
	
	SMS_VDPturnOnFeature(VDPFEATURE_HIDEFIRSTCOL);
	SMS_displayOn();
	
	for(;;)
	{
		ks = SMS_getKeysStatus(); 
		processUserInput(ks);
		GSL_scroll(scrollXOffset,scrollYOffset);
		SMS_waitForVBlank(); 
		GSL_VBlank();
	}
	SMS_displayOff();
}