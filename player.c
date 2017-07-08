#include <stdbool.h>
#include "spriteengine.h"
#include "player.h"
#include "bank2.h"
#include "playercharacter.h"
#include "SMSlib.h"
#include "montylib.h"

player player1;
player player2;

void add_player(unsigned char player_number) {
	if(player_number == 1) {
		initEngine();
		if(player1.entityreference == 0) {
			player1.vx = 0;
			player1.vy = 0;
			player1.xdirection = 1;
			player1.ydirection = 1;
			player1.entityreference = addEntity(&player_character, 32, 100);
			player1.status = PLAYER_STATUS_IDDLE;
			player1.status2 = PLAYER_STATUS2_IDDLE;
		}	
	} else {
		if(player2.entityreference == 0) {
			player2.vx = 0;
			player2.vy = 0;
			player2.xdirection = 1;
			player2.ydirection = 1;
			player2.entityreference = addEntity(&player_character, 132, 100);
			player2.status = PLAYER_STATUS_IDDLE;
			player2.status2 = PLAYER_STATUS2_IDDLE;
		}
	}
}

void manage_input(unsigned int keys) {
	switch(player1.status) {
		case PLAYER_STATUS_IDDLE:
			manage_iddle_status(1, keys);
			break;
		case PLAYER_STATUS_WALKING:
			manage_walking_status(1, keys);
			break;
		case PLAYER_STATUS_JUMPING:
			manage_jumping_status(1, keys);
			break;
		case PLAYER_STATUS_CROUCHED:
			break;
		default:
			break;
	}
	switch(player2.status) {
		case PLAYER_STATUS_IDDLE:
			manage_iddle_status(2, keys);
			break;
		case PLAYER_STATUS_WALKING:
			manage_walking_status(2, keys);
			break;
		case PLAYER_STATUS_JUMPING:
			manage_jumping_status(2, keys);
			break;
		case PLAYER_STATUS_CROUCHED:
			break;
		default:
			break;
	}
	update_positions();
}

void manage_iddle_status(unsigned char player_number, unsigned int keys) {
	if(player_number == 1) {
		if(player1.status2 == PLAYER_STATUS2_PUNCHING) {
			if(isAnimationEnded(player1.entityreference->entityIndex)) {
				player1.status2 = PLAYER_STATUS2_IDDLE;
				setAnimation(player1.entityreference->entityIndex,0);
			}
		} else {
			if(keys & PORT_A_KEY_RIGHT) {
				player1.status = PLAYER_STATUS_WALKING;
				player1.vx = CHAR2UFIX(1);
				player1.xdirection = 1;
				player1.entityreference->direction = RIGHT_DIRECTION;
			} else if(keys & PORT_A_KEY_LEFT) {
				player1.status = PLAYER_STATUS_WALKING;
				player1.vx = CHAR2UFIX(1);
				player1.xdirection = -1;
				player1.entityreference->direction = LEFT_DIRECTION;
			} else if(keys & PORT_A_KEY_UP) {
				player1.status = PLAYER_STATUS_WALKING;
				player1.vy = CHAR2UFIX(1);
				player1.ydirection = -1;
			} else if(keys & PORT_A_KEY_DOWN) {
				player1.status = PLAYER_STATUS_WALKING;
				player1.vy = CHAR2UFIX(1);
				player1.ydirection = 1;
			} else if(keys & PORT_A_KEY_1) {
				player1.status2 = PLAYER_STATUS2_PUNCHING;
				setAnimation(player1.entityreference->entityIndex,3);
			} else if(keys & PORT_A_KEY_2) {
				player1.status = PLAYER_STATUS_JUMPING;
				player1.vy = CHAR2UFIX(3);
				player1.ydirection = -1;
			}	
		}
		
	} else {
		if(player2.status2 == PLAYER_STATUS2_PUNCHING) {
			if(isAnimationEnded(player2.entityreference->entityIndex)) {
				player2.status2 = PLAYER_STATUS2_IDDLE;
				setAnimation(player2.entityreference->entityIndex,0);
			}
		} else {
			if(keys & PORT_B_KEY_RIGHT) {
				player2.status = PLAYER_STATUS_WALKING;
				player2.vx = CHAR2UFIX(1);
				player2.xdirection = 1;
				player2.entityreference->direction = RIGHT_DIRECTION;
			} else if(keys & PORT_B_KEY_LEFT) {
				player2.status = PLAYER_STATUS_WALKING;
				player2.vx = CHAR2UFIX(1);
				player2.xdirection = -1;
				player2.entityreference->direction = LEFT_DIRECTION;
			} else if(keys & PORT_B_KEY_UP) {
				player2.status = PLAYER_STATUS_WALKING;
				player2.vy = CHAR2UFIX(1);
				player2.ydirection = -1;
			} else if(keys & PORT_B_KEY_DOWN) {
				player2.status = PLAYER_STATUS_WALKING;
				player2.vy = CHAR2UFIX(1);
				player2.ydirection = 1;
			}else if(keys & PORT_B_KEY_1) {
				player2.status2 = PLAYER_STATUS2_PUNCHING;
				setAnimation(player2.entityreference->entityIndex,3);
			} else if(keys & PORT_B_KEY_2) {
				player2.status = PLAYER_STATUS_JUMPING;
				player2.vy = CHAR2UFIX(3);
				player2.ydirection = -1;
			}
		}
	}
}

void manage_walking_status(unsigned char player_number, unsigned int keys) {
	if(player_number == 1) {
		if(player1.status2 == PLAYER_STATUS2_PUNCHING) {
			if(isAnimationEnded(player1.entityreference->entityIndex)) {
				player1.status2 = PLAYER_STATUS2_IDDLE;
				setAnimation(player1.entityreference->entityIndex,2);
			}
		} else {
			if(keys & PORT_A_KEY_RIGHT) {
				if(isAnimationEnded(player1.entityreference->entityIndex)) {
					setAnimation(player1.entityreference->entityIndex,2);	
				}
				if(player1.xdirection == -1) {
					forceReload(player1.entityreference->entityIndex);
				}
				player1.xdirection = 1;
				player1.entityreference->direction = RIGHT_DIRECTION;
			} else if(keys & PORT_A_KEY_LEFT) {
				if(isAnimationEnded(player1.entityreference->entityIndex)) {
					setAnimation(player1.entityreference->entityIndex,2);
				}
				if(player1.xdirection == 1) {
					forceReload(player1.entityreference->entityIndex);
				}
				player1.xdirection = -1;
				player1.entityreference->direction = LEFT_DIRECTION;
			} else {
				if(isAnimationEnded(player1.entityreference->entityIndex)) {
					player1.vx = 0;
				}
			}
			if(keys & PORT_A_KEY_UP) {
				if(isAnimationEnded(player1.entityreference->entityIndex)) {
					setAnimation(player1.entityreference->entityIndex,2);	
				}
				player1.vy = CHAR2UFIX(1);
				player1.ydirection = -1;
			} else if(keys & PORT_A_KEY_DOWN) {
				if(isAnimationEnded(player1.entityreference->entityIndex)) {
					setAnimation(player1.entityreference->entityIndex,2);	
				}
				player1.vy = CHAR2UFIX(1);
				player1.ydirection = 1;
			} else {
				if(isAnimationEnded(player1.entityreference->entityIndex)) {
					player1.vy = 0;
				}
			}

			if(player1.vx == 0 && player1.vy == 0) {
				setAnimation(player1.entityreference->entityIndex,0);
				player1.status = PLAYER_STATUS_IDDLE;
			}
			if(keys & PORT_A_KEY_2) {
				player1.status = PLAYER_STATUS_JUMPING;
				player1.vy = CHAR2UFIX(3);
				player1.ydirection = -1;
			}
			if(keys & PORT_A_KEY_1) {
				player1.status2 = PLAYER_STATUS2_PUNCHING;
				setAnimation(player1.entityreference->entityIndex,3);
			}
		}
	} else {
		if(player2.status2 == PLAYER_STATUS2_PUNCHING) {
			if(isAnimationEnded(player2.entityreference->entityIndex)) {
				player2.status2 = PLAYER_STATUS2_IDDLE;
				setAnimation(player2.entityreference->entityIndex,2);
			}
		} else {
			if(keys & PORT_B_KEY_RIGHT) {
				if(isAnimationEnded(player2.entityreference->entityIndex)) {
					setAnimation(player2.entityreference->entityIndex,2);	
				}
				if(player2.xdirection == -1) {
					forceReload(player2.entityreference->entityIndex);
				}
				player2.xdirection = 1;
				player2.entityreference->direction = RIGHT_DIRECTION;
			} else if(keys & PORT_B_KEY_LEFT) {
				if(isAnimationEnded(player2.entityreference->entityIndex)) {
					setAnimation(player2.entityreference->entityIndex,2);
				}
				if(player2.xdirection == 1) {
					forceReload(player2.entityreference->entityIndex);
				}
				player2.xdirection = -1;
				player2.entityreference->direction = LEFT_DIRECTION;
			} else {
				if(isAnimationEnded(player2.entityreference->entityIndex)) {
					player2.vx = 0;
				}
			}
			if(keys & PORT_B_KEY_UP) {
				if(isAnimationEnded(player2.entityreference->entityIndex)) {
					setAnimation(player2.entityreference->entityIndex,2);	
				}
				player2.vy = CHAR2UFIX(1);
				player2.ydirection = -1;
			} else if(keys & PORT_B_KEY_DOWN) {
				if(isAnimationEnded(player2.entityreference->entityIndex)) {
					setAnimation(player2.entityreference->entityIndex,2);	
				}
				player2.vy = CHAR2UFIX(1);
				player2.ydirection = 1;
			} else {
				if(isAnimationEnded(player2.entityreference->entityIndex)) {
					player2.vy = 0;
				}
			}
			if(player2.vx == 0 && player2.vy == 0) {
				setAnimation(player2.entityreference->entityIndex,0);
				player2.status = PLAYER_STATUS_IDDLE;
			}
			if(keys & PORT_B_KEY_2) {
				player2.status = PLAYER_STATUS_JUMPING;
				player2.vy = CHAR2UFIX(3);
				player2.ydirection = -1;
			}
			if(keys & PORT_B_KEY_1) {
				player2.status2 = PLAYER_STATUS2_PUNCHING;
				setAnimation(player2.entityreference->entityIndex,3);
			}
		}
	}
}

void manage_jumping_status(unsigned char player_number, unsigned int keys) {
	if(player_number == 1) {
		if(player1.status2 == PLAYER_STATUS2_PUNCHING) {
			if(isAnimationEnded(player1.entityreference->entityIndex)) {
				player1.status2 = PLAYER_STATUS2_IDDLE;
				setAnimation(player1.entityreference->entityIndex,4);
			}
		} else {
			if(keys & PORT_A_KEY_RIGHT) {
				if(player1.xdirection == -1) {
					forceReload(player1.entityreference->entityIndex);
				}
				player1.xdirection = 1;
				player1.entityreference->direction = RIGHT_DIRECTION;
				player1.vx = CHAR2UFIX(1);
			} else if(keys & PORT_A_KEY_LEFT) {
				if(player1.xdirection == 1) {
					forceReload(player1.entityreference->entityIndex);
				}
				player1.xdirection = -1;
				player1.entityreference->direction = LEFT_DIRECTION;
				player1.vx = CHAR2UFIX(-1);
			} else {
				if(isAnimationEnded(player1.entityreference->entityIndex)) {
					player1.vx = 0;
				}
			}
			if(isAnimationEnded(player1.entityreference->entityIndex)) {
				setAnimation(player1.entityreference->entityIndex,4);	
			}
			if(player1.ydirection == -1) {
				player1.vy = player1.vy - 32;	
			} else {
				player1.vy = player1.vy + 32;
			}
			
			if(player1.vy == 0) {
				player1.ydirection = 1;	
			}
			if(player1.vy >= CHAR2UFIX(3)) {
				setAnimation(player1.entityreference->entityIndex,0);
				player1.status = PLAYER_STATUS_IDDLE;
				player1.status2 = PLAYER_STATUS2_IDDLE;
				player1.vx = 0;
				player1.vy = 0;
			}
			
			if(keys & PORT_A_KEY_1) {
				player1.status2 = PLAYER_STATUS2_PUNCHING;
				setAnimation(player1.entityreference->entityIndex,5);
			}
		}
	} else {

	}
}

void update_positions() {
	move_entity(player1.entityreference->entityIndex, UFIX2CHAR(player1.vx)*player1.xdirection, UFIX2CHAR(player1.vy)*player1.ydirection);
	
	if(player1.entityreference->py > 160) {
		player1.entityreference->py = 160;
	} else if(player1.entityreference->py < 100 && player1.status != PLAYER_STATUS_JUMPING) {
		player1.entityreference->py = 100;
	}
	if(player1.entityreference->px < 12) {
		player1.entityreference->px = 12;
	} else if(player1.entityreference->px > 240) {
		player1.entityreference->px = 240;
	}

	move_entity(player2.entityreference->entityIndex, UFIX2CHAR(player2.vx)*player2.xdirection, UFIX2CHAR(player2.vy)*player2.ydirection);
	
	if(player2.entityreference->py > 160) {
		player2.entityreference->py = 160;
	} else if(player2.entityreference->py < 100 && player2.status != PLAYER_STATUS_JUMPING) {
		player2.entityreference->py = 100;
	}
	if(player2.entityreference->px < 12) {
		player2.entityreference->px = 12;
	} else if(player2.entityreference->px > 240) {
		player2.entityreference->px = 240;
	}
}