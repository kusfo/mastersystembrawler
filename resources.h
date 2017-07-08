#ifndef MSB_RESOURCES_H
#define MSB_RESOURCES_H

void load_logo_assets();
void load_logo_halfpalette();
void load_logo_fullpalette();
void load_game_assets();
void play_logo_music();

void load_background_blackpalette();
void prepare_player_sprites();
void draw_player_sprites();
void move_player_right();
void move_player_left();
void move_player_up();
void move_player_down();
void player_punch();
void player_jump();
void stop_player();

void load_system_font();
void print_debug_info();
void print_unsigned_char(unsigned char number);
void print_signed_char(signed char number);
#endif //MSB_RESOURCES_H
