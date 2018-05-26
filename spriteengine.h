#ifndef MSB_SPRITEENGINE_H
#define MSB_SPRITEENGINE_H

#define MAX_ENTITIES 5
#define RIGHT_DIRECTION 0
#define LEFT_DIRECTION 1

typedef struct {
    const unsigned char tile;
    const signed char dpx;
    const signed char dpy;
} tile;

typedef struct {
	const unsigned char frameNumber;
	const bool mirrored;
	const unsigned char numsprites;
	const unsigned char *tiles;
	const unsigned int tile_bin_size;
	const unsigned char tile_bank;
	const unsigned char frame_time;
	const signed char collisionx1;
	const signed char collisiony1;
	const signed char collisionx2;
	const signed char collisiony2;
	const signed char hitboxx1;
	const signed char hitboxy1;
	const signed char hitboxx2;
	const signed char hitboxy2;
	const tile tilelist[16];
} frame;

typedef struct {
	const unsigned char animationNumber;
	const unsigned char numframes;
	const frame framelist[5]; // 5 normal + 5 mirrored
	const frame mirroredframelist[5];
} animation;

typedef struct {
	const unsigned char numanimations;
	const unsigned char maxsprites;
	const animation animationlist[15];
} character;

typedef struct {
	character *characterData;
	unsigned char entityIndex;
	unsigned char px;
	unsigned char py;
	unsigned char direction;
	unsigned char currentAnimation;
	unsigned char currentFrame;
	unsigned char framecnt;
	unsigned int vramposition;
	bool has2ReloadTiles;
	bool animationEnded;
	bool frameEnded;
} entity;

extern entity *entitylist[MAX_ENTITIES];

void initEngine();
entity* addEntity(character* characterData, unsigned char px, unsigned char py);
void setAnimation(unsigned char entityindex, int animationNumber);
void forceReload(unsigned char entityindex);
bool isFrameEnded(unsigned char entitynumber);
bool isAnimationEnded(unsigned char entitynumber);
void setDirection(unsigned char entityindex, unsigned char direction);
void updateAnimations();
void updateTiles();
void drawEntities();
void move_entity(unsigned char entitynumber, signed char x, signed char y);
#endif