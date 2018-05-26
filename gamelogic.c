#include <stdbool.h>
#include "libs/SMSlib.h"
#include "gamelogic.h"
#include "resources.h"
#include "spriteengine.h"
#include "soundengine.h"
#include "player.h"
#include "enemy.h"
#include "montylib.h"

unsigned char game_status;
unsigned int score;
bool scroll_enabled;

void init_game() {
    init_console();
}

void play_game(){
    logo_screen();
    fnaclogo_screen();
    presentation_screen();
    while(game_status != GAME_STATUS_GAME_OVER) {
        game_loop();
    }
}

void logo_screen() {
    load_logo_assets();
    load_background_blackpalette();
    frame_cnt = 0;
    play_logo_music();
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

void fnaclogo_screen() {
    load_fnaclogo_assets();
    load_background_blackpalette();
    frame_cnt = 0;
    while (frame_cnt < 300) {
        frame_cnt++;
        if(frame_cnt == 25) {
            load_fnaclogo_halfpalette();
        }
        if(frame_cnt == 50) {
            load_fnaclogo_fullpalette();
        }
        if(frame_cnt == 250) {
            load_fnaclogo_halfpalette();
        } 
        if(frame_cnt == 275) {
            load_background_blackpalette();
        }
        waitForFrame();
    }
}

void presentation_screen() {
    unsigned int keys;
    load_presentation_assets();
    load_background_blackpalette();
    play_presentation_music();
    frame_cnt = 0;
    while (frame_cnt < 800) {
        frame_cnt++;
        if(frame_cnt == 25) {
            load_presentation_halfpalette();
        }
        if(frame_cnt == 50) {
            load_presentation_fullpalette();
        }
        if(frame_cnt == 750) {
            load_presentation_halfpalette();
        } 
        if(frame_cnt == 775) {
            load_background_blackpalette();
        }
        keys = SMS_getKeysStatus();
        if(keys & PORT_A_KEY_1 && (frame_cnt < 740)) {
            frame_cnt = 740;
        }
        waitForFrame();
    }
}

void game_loop() {
    SMS_setBackdropColor(1);
    load_level1_assets();
    init_gamestatus();
    init_level1_scroll();
    play_game_music();
    add_player(1);
    //add_player(2);
    frame_cnt = 0;
    scroll_enabled = true;
    while (1) {
        if(!pause) {
            execute_game_logic();
        }
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
            //if(frame_cnt & 1) {
                keys = SMS_getKeysStatus();
                manage_input(keys);
                //decide_action();
            //} else {
                update_resources();
            //}
            if(player1.vx > 0 && scroll_enabled) {
                if(player1.entityreference->px > 200 && player1.xdirection == 1) {
                    update_scroll(UFIX2CHAR(player1.vx),0);
                    if(get_scroll_position_x() > 1200){
                        scroll_enabled = false;
                        //add_enemy(0);
                    }
                    update_player_positions(UFIX2CHAR(player1.vx),0); 
                } else if(player1.entityreference->px < 56 && player1.xdirection == -1) {
                    update_scroll(UFIX2CHAR(player1.vx)*-1,0);
                    update_player_positions(UFIX2CHAR(player1.vx)*-1,0); 
                } else {
                   update_player_positions(0,0); 
                }
            } else {
                update_player_positions(0,0);    
            }
            frame_cnt++;
            break;
        default:
            break;
    }
}