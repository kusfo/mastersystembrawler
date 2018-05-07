

void GSL_initializeMap(void *scrolltable, void *metatiles);
void GSL_positionWindow(unsigned int X, unsigned int Y);
unsigned int * GSL_metatileLookup(unsigned int x, unsigned int y);
void GSL_tileLookup(unsigned int x, unsigned int y);
void GSL_refreshVDP();
void GSL_scroll(char x, char y);
void GSL_VBlank();
void GSL_metatileUpdate();
void GSL_metatileUpdateCustom(unsigned int x, unsigned int y, unsigned int offset);
unsigned int GSL_getMapWidthInPixels();
unsigned int GSL_getMapHeightInPixels();
unsigned int GSL_getMapWidthInMetatiles();
unsigned int GSL_getMapHeightInMetatiles();
unsigned int GSL_getScrolltableSize();
unsigned int GSL_getCurrentX();
unsigned int GSL_getCurrentY();
unsigned char GSL_getCollisionCount();
unsigned char * GSL_getScrolltableAddress();
