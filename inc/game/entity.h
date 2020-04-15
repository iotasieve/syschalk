#pragma once


#define ENTITY_STATE_IDLE 0
#define ENTITY_STATE_WALK 1

#include <inc/types.h>
#include <inc/game/bmath.h>
#include <inc/game/gamestruct.h>

void GAddZombie(GGame *game, float x, float y);

void GAddProjectile(GGame *game, float x, float y, float vx, float vy);
void GGameplayEntitiesRender(GGame *game);
void GGameplayEntitiesHandle(GGame *game);