#include <stdbool.h>
#include "spriteengine.h"
#include "enemy.h"
#include "bank2.h"
#include "bank3.h"
#include "bank4.h"
#include "enemycharacter.h"
#include "montylib.h"
#include "soundengine.h"
#include "libs/SMSlib.h"

enemy enemy1;
enemy enemy2;
enemy enemy3;

void add_enemy(unsigned char enemy_number) {
/*	enemy* enemypointer = 0;
	if(enemy_number == 0) {
		enemypointer = &enemy1;
	} else if(enemy_number == 1) {
		enemypointer = &enemy2;
	} else if(enemy_number == 2) {
		enemypointer = &enemy3;
	}
	if(enemypointer != 0 && enemypointer->entityreference == 0) {
		enemypointer->vx = 0;
		enemypointer->vy = 0;
		enemypointer->xdirection = 1;
		enemypointer->ydirection = 1;
		enemypointer->entityreference = addEntity(&enemy_character, 220, 100);
		enemypointer->status = ENEMY_STATUS_IDDLE;
		enemypointer->status2 = ENEMY_STATUS_IDDLE;
	}*/
}

void decide_action() {
/*	enemy* enemypointer;
	if(enemy1.entityreference != 0) {
		enemypointer = &enemy1;
		decide_action_enemy(enemypointer);
	}
	if(enemy1.entityreference != 0) {
		enemypointer = &enemy1;
		decide_action_enemy(enemypointer);
	}
	if(enemy1.entityreference != 0) {
		enemypointer = &enemy1;
		decide_action_enemy(enemypointer);
	}*/
}

void decide_action_enemy(enemy* enemypointer) {
/*	unsigned int action = PORT_A_KEY_LEFT;
	switch(enemypointer->status) {
		case ENEMY_STATUS_IDDLE:
			manage_enemy_iddle_status(enemypointer, action);
			break;
		case ENEMY_STATUS_WALKING:
			manage_enemy_walking_status(enemypointer, action);
			break;
		default:
			break;
	}*/
}

void manage_enemy_iddle_status(enemy* enemypointer, unsigned int action) {
/*	if(enemypointer->status2 == ENEMY_STATUS2_PUNCHING) {
		if(isAnimationEnded(enemypointer->entityreference->entityIndex)) {
			enemypointer->status2 = ENEMY_STATUS2_IDDLE;
			play_punch_sound2();
			setAnimation(enemypointer->entityreference->entityIndex,0);
		} else if(isFrameEnded(enemypointer->entityreference->entityIndex) && !(action & PORT_A_KEY_1)) {
			enemypointer->status2 = ENEMY_STATUS2_IDDLE;
			play_punch_sound2();
			setAnimation(enemypointer->entityreference->entityIndex,0);
		}
	} else {
		if(action & PORT_A_KEY_RIGHT) {
			enemypointer->status = ENEMY_STATUS_WALKING;
			enemypointer->vx = CHAR2UFIX(1);
			enemypointer->xdirection = 1;
			enemypointer->entityreference->direction = RIGHT_DIRECTION;
		} else if(action & PORT_A_KEY_LEFT) {
			enemypointer->status = ENEMY_STATUS_WALKING;
			enemypointer->vx = CHAR2UFIX(1);
			enemypointer->xdirection = -1;
			enemypointer->entityreference->direction = LEFT_DIRECTION;
		} else if(action & PORT_A_KEY_UP) {
			enemypointer->status = ENEMY_STATUS_WALKING;
			enemypointer->vy = CHAR2UFIX(1);
			enemypointer->ydirection = -1;
		} else if(action & PORT_A_KEY_DOWN) {	
			enemypointer->status = ENEMY_STATUS_WALKING;
			enemypointer->vy = CHAR2UFIX(1);
			enemypointer->ydirection = 1;	
		} else if((action & PORT_A_KEY_1) && enemypointer->status2 == ENEMY_STATUS_IDDLE) {
			enemypointer->status2 = ENEMY_STATUS2_PUNCHING;
			setAnimation(enemypointer->entityreference->entityIndex,3);
			play_punch_sound1();
		} 	
	}*/	
}

void manage_enemy_walking_status(enemy* enemypointer, unsigned int action) {
	/*if(enemypointer->status2 == ENEMY_STATUS2_PUNCHING) {	
		if(isAnimationEnded(enemypointer->entityreference->entityIndex)) {
			enemypointer->status2 = ENEMY_STATUS2_IDDLE;
			enemypointer->status = ENEMY_STATUS_IDDLE;
			enemypointer->vx = 0;
			setAnimation(enemypointer->entityreference->entityIndex,0);
			play_punch_sound2();
		} else if(isFrameEnded(enemypointer->entityreference->entityIndex) && !(action & PORT_A_KEY_1)) {
			enemypointer->status2 = ENEMY_STATUS2_IDDLE;
			enemypointer->status = ENEMY_STATUS_IDDLE;
			enemypointer->vx = 0;
			play_punch_sound2();
			setAnimation(enemypointer->entityreference->entityIndex,0);
		}
	} else {
		if(action & PORT_A_KEY_RIGHT) {
			if(isAnimationEnded(enemypointer->entityreference->entityIndex)) {
				setAnimation(enemypointer->entityreference->entityIndex,2);	
			}
			if(enemypointer->xdirection == -1) {
				forceReload(enemypointer->entityreference->entityIndex);
			}
			enemypointer->xdirection = 1;
			enemypointer->entityreference->direction = RIGHT_DIRECTION;
		} else if(action & PORT_A_KEY_LEFT) {
			if(isAnimationEnded(enemypointer->entityreference->entityIndex)) {
				setAnimation(enemypointer->entityreference->entityIndex,2);
			}
			if(enemypointer->xdirection == 1) {
				forceReload(enemypointer->entityreference->entityIndex);
			}
			enemypointer->xdirection = -1;
			enemypointer->entityreference->direction = LEFT_DIRECTION;
		} else {
			if(isFrameEnded(enemypointer->entityreference->entityIndex)) {
				enemypointer->vx = 0;
			}
		}
		if(action & PORT_A_KEY_UP) {
			if(isAnimationEnded(enemypointer->entityreference->entityIndex)) {
				setAnimation(enemypointer->entityreference->entityIndex,2);	
			}
			enemypointer->vy = CHAR2UFIX(1);
			enemypointer->ydirection = -1;
		} else if(action & PORT_A_KEY_DOWN) {
			if(isAnimationEnded(enemypointer->entityreference->entityIndex)) {
				setAnimation(enemypointer->entityreference->entityIndex,2);	
			}
			enemypointer->vy = CHAR2UFIX(1);
			enemypointer->ydirection = 1;
		} else {
			if(isFrameEnded(enemypointer->entityreference->entityIndex)) {
				enemypointer->vy = 0;
			}
		}

		if(enemypointer->vx == 0 && enemypointer->vy == 0) {
			setAnimation(enemypointer->entityreference->entityIndex,0);
			enemypointer->status = ENEMY_STATUS_IDDLE;
		}
		
		if((action & PORT_A_KEY_1) && enemypointer->status2 == ENEMY_STATUS2_IDDLE) {
			enemypointer->status2 = ENEMY_STATUS2_PUNCHING;
			setAnimation(enemypointer->entityreference->entityIndex,3);
			play_punch_sound1();
		}
	}*/
}

void update_enemy_positions(signed char delta_x, signed char delta_y) {
	/*if(enemy1.entityreference != 0) {
		move_entity(enemy1.entityreference->entityIndex, UFIX2CHAR(enemy1.vx)*enemy1.xdirection + (delta_x * -1), UFIX2CHAR(enemy1.vy)*enemy1.ydirection);		
	}
	if(enemy2.entityreference != 0) {
		move_entity(enemy2.entityreference->entityIndex, UFIX2CHAR(enemy2.vx)*enemy2.xdirection + (delta_x * -1), UFIX2CHAR(enemy2.vy)*enemy2.ydirection);	
	}
	if(enemy3.entityreference != 0) {
		move_entity(enemy3.entityreference->entityIndex, UFIX2CHAR(enemy3.vx)*enemy3.xdirection  + (delta_x * -1), UFIX2CHAR(enemy3.vy)*enemy3.ydirection);	
	}*/	
}