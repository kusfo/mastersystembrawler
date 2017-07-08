#include <stdbool.h>
#include "SMSlib.h"
#include "gamelogic.h"
#include "resources.h"
#include "montylib.h"

unsigned char game_status;
unsigned int score;

void init_game() {
    init_console();
}

void play_game(){
    logo_screen();
    while(game_status != GAME_STATUS_GAME_OVER) {
        game_loop();
    }
}


void logo_screen() {
    load_logo_assets();
    load_background_blackpalette();
    frame_cnt = 0;
    play_logo_music();
    music_bank = get_music_bank();
    while (frame_cnt < 300) {
        frame_cnt++;
        if(frame_cnt == 25) {
            load_logo_halfpalette();
        }
        if(frame_cnt == 50) {
            load_logo_fullpalette();
        }
        if(frame_cnt == 250) {
            load_logo_halfpalette();
        } 
        if(frame_cnt == 275) {
            load_background_blackpalette();
        }
        waitForFrame();
    }
}

void game_loop() {
    load_level1_assets();
    init_gamestatus();
    init_entities();
    prepare_player_sprites();
    play_game_music();
    music_bank = get_music_bank();
    while (1) {
        if(!pause) {
            execute_game_logic();
        }
        draw_player_sprites();
        waitForFrame();
    }
}

void init_gamestatus() {
    game_status = GAME_STATUS_PLAYING;
    score = 0;
}

void execute_game_logic() {
    unsigned int keys;
    switch (game_status) {
        case GAME_STATUS_PLAYING:
            keys = SMS_getKeysStatus();
            if(keys & PORT_A_KEY_RIGHT) {
                move_player_right();
            } else if(keys & PORT_A_KEY_LEFT) {
                move_player_left();
            } else if(keys & PORT_A_KEY_UP) {
                move_player_up();
            } else if(keys & PORT_A_KEY_DOWN) {
                move_player_down();
            } else if(keys & PORT_A_KEY_1) {
                player_punch();
            } else if(keys & PORT_A_KEY_2) {
                player_jump();
            } else {
                stop_player();
            }
            break;
        default:
            break;
    }
    frame_cnt++;
}