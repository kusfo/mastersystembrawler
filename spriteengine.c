#include <stdlib.h>
#include <stdbool.h>
#include "spriteengine.h"
#include "SMSlib.h"

entity *entitylist[MAX_ENTITIES];
unsigned char entityIndex = 0;
unsigned int entityVramPosition = 256;

void initEngine() {
	int i;
	for(i = 0;i < MAX_ENTITIES; i++) {
		free(entitylist[i]);
		entitylist[i] = 0;
	}
	entityIndex = 0;
}

entity* addEntity(character* characterData, unsigned char px, unsigned char py) {
	entitylist[entityIndex] = malloc(sizeof(entity));
	entitylist[entityIndex]->entityIndex = entityIndex;
	entitylist[entityIndex]->currentAnimation = 0;
	entitylist[entityIndex]->currentFrame = 0;
	entitylist[entityIndex]->framecnt = 0;
	entitylist[entityIndex]->has2ReloadTiles = true;
	entitylist[entityIndex]->animationEnded = true;
	entitylist[entityIndex]->characterData = characterData;
	entitylist[entityIndex]->px = px;
	entitylist[entityIndex]->py = py;
	entitylist[entityIndex]->direction = RIGHT_DIRECTION;
	entitylist[entityIndex]->vramposition = entityVramPosition;
	entityVramPosition = entityVramPosition + characterData->maxsprites;
	entityIndex++;
	return entitylist[entityIndex - 1];
}

void setAnimation(unsigned char entitynumber, int animationNumber) {
	if(entitylist[entitynumber]->currentAnimation != animationNumber) {
		entitylist[entitynumber]->has2ReloadTiles = true;	
	}
	entitylist[entitynumber]->currentAnimation = animationNumber;
	entitylist[entitynumber]->animationEnded = false;
	entitylist[entitynumber]->currentFrame = 0;
	entitylist[entitynumber]->framecnt = 0;	
}

void forceReload(unsigned char entitynumber) {
	entitylist[entitynumber]->has2ReloadTiles = true;
}

bool isAnimationEnded(unsigned char entitynumber) {
	return entitylist[entitynumber]->animationEnded;
}

void setDirection(unsigned char entitynumber, unsigned char direction) {
	entitylist[entitynumber]->direction = direction;
	entitylist[entitynumber]->has2ReloadTiles = true;
}

void updateAnimations() {
	int i;
	for(i = 0;i < entityIndex; i++) {
		entitylist[i]->framecnt++;
		if(entitylist[i]->framecnt > entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].framelist[entitylist[i]->currentFrame].frame_time){
			if(entitylist[i]->currentFrame < entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].numframes - 1) { //tenir en compte els mirrored!
				entitylist[i]->currentFrame++;
				entitylist[i]->has2ReloadTiles = true;
			} else {
				entitylist[i]->animationEnded = true;
				entitylist[i]->currentFrame = 0;
			}
			entitylist[i]->framecnt = 0;
		}
	}
}

void updateTiles() {
	int i;
	for(i = 0;i < entityIndex; i++) {
		if(entitylist[i]->has2ReloadTiles) {	
			if(entitylist[i]->direction == RIGHT_DIRECTION) {
				SMS_mapROMBank(entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].framelist[entitylist[i]->currentFrame].tile_bank);
				SMS_loadTiles(entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].framelist[entitylist[i]->currentFrame].tiles, entitylist[i]->vramposition, entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].framelist[entitylist[i]->currentFrame].tile_bin_size);	
			} else {
				SMS_mapROMBank(entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].framelist[entitylist[i]->currentFrame].tile_bank);
				SMS_loadTiles(entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].mirroredframelist[entitylist[i]->currentFrame].tiles, entitylist[i]->vramposition, entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].mirroredframelist[entitylist[i]->currentFrame].tile_bin_size);
			}
			entitylist[i]->has2ReloadTiles = false;
		}
	}
}

void drawEntities() {
	int i,j;
	SMS_initSprites();
	for(i = 0; i < entityIndex; i++) {
		if(entitylist[i]->direction == RIGHT_DIRECTION) {
			for(j = 0; j < entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].framelist[entitylist[i]->currentFrame].numsprites; j++){
				SMS_addSprite (entitylist[i]->px + entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].framelist[entitylist[i]->currentFrame].tilelist[j].dpx,entitylist[i]->py + entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].framelist[entitylist[i]->currentFrame].tilelist[j].dpy,entitylist[i]->vramposition + entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].framelist[entitylist[i]->currentFrame].tilelist[j].tile);
			}
		} else {
			for(j = 0; j < entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].mirroredframelist[entitylist[i]->currentFrame].numsprites; j++){
				SMS_addSprite (entitylist[i]->px + entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].mirroredframelist[entitylist[i]->currentFrame].tilelist[j].dpx,entitylist[i]->py + entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].mirroredframelist[entitylist[i]->currentFrame].tilelist[j].dpy,entitylist[i]->vramposition + entitylist[i]->characterData->animationlist[entitylist[i]->currentAnimation].mirroredframelist[entitylist[i]->currentFrame].tilelist[j].tile);
			}
		}
	}
	SMS_finalizeSprites();
}

void move_entity(unsigned char entitynumber, signed char x, signed char y) {
	entitylist[entitynumber]->px = entitylist[entitynumber]->px + x;
	entitylist[entitynumber]->py = entitylist[entitynumber]->py + y;
}