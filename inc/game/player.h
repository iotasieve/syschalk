#pragma once

#include <inc/types.h>
#include <inc/game/bmath.h>
#include <inc/game/gamestruct.h>


#define PLAYER_STATE_IDLE 0
#define PLAYER_STATE_WALK 1


int GGameplayPlayerScript(GGame *game);
void GGameplayPlayerRender(GGame *game);