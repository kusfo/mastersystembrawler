#ifndef MSB_ENEMY_H
#define MSB_ENEMY_H

#define ENEMY_STATUS_IDDLE 0
#define ENEMY_STATUS_WALKING 1

#define ENEMY_STATUS2_IDDLE 0
#define ENEMY_STATUS2_PUNCHING 1
#define ENEMY_STATUS2_HITTED 2

typedef struct {
	entity *entityreference;
	unsigned int vx;
	unsigned int vy;
	signed char xdirection;
	signed char ydirection;
	unsigned char status;
	unsigned char status2;
} enemy;

extern enemy enemy1;
extern enemy enemy2;
extern enemy enemy3;

void add_enemy(unsigned char enemy_number);
void decide_action();
void decide_action_enemy(enemy* enemypointer);
void manage_enemy_iddle_status(enemy* enemypointer, unsigned int action);
void manage_enemy_walking_status(enemy* enemypointer, unsigned int action);
void update_enemy_positions(signed char delta_x, signed char delta_y);
#endif //MSB_ENEMY_H