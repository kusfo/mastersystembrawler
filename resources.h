#ifndef MSB_RESOURCES_H
#define MSB_RESOURCES_H

void load_logo_assets();
void load_logo_halfpalette();
void load_logo_fullpalette();

void load_fnaclogo_assets();
void load_fnaclogo_halfpalette();
void load_fnaclogo_fullpalette();

void load_presentation_assets();
void load_presentation_halfpalette();
void load_presentation_fullpalette();

void load_level1_assets();
void init_level1_scroll();

void update_scroll(signed char delta_x, signed char delta_y);
unsigned int get_scroll_position_x();
unsigned int get_scroll_position_y();

void update_resources();

void load_background_blackpalette();
unsigned char get_music_bank();

void load_system_font();
void print_debug_info();
void print_unsigned_char(unsigned char number);
void print_signed_char(signed char number);

#endif //MSB_RESOURCES_H
