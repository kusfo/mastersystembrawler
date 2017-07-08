
#ifndef MSB_GAMELOGIC_H
#define MSB_GAMELOGIC_H

#define GAME_STATUS_PLAYING 0
#define GAME_STATUS_GAME_OVER 1

extern unsigned char game_status;
extern unsigned int score;


void init_game();
void init_gamestatus();
void play_game();
void logo_screen();
void game_loop();
void execute_game_logic();
#endif //MSB_GAMELOGIC_H
