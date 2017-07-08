
#ifndef MSB_MONTYLIB_H_H
#define MSB_MONTYLIB_H_H

#define VRAM_BASE_SPRITES 256
#define SCREEN_COLUMNS 32
#define SCREEN_ROWS 24
#define NUM_FPS_FRAME 60

extern bool pause;
extern unsigned char rand_index;
extern unsigned int frame_cnt;

void init_console();
void waitForFrame();

unsigned char rand();

#endif //MSB_MONTYLIB_H_H
