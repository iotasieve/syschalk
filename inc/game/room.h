#pragma once
#include <inc/game/gamestruct.h>

void GameRoomRender(GGame *game, GRoom *room, int atX, int atY);

void GGameRoomsRender(GGame *game, GRoom *cr, int in, int jn);

void GameRoomHandle(GGame *game);

GRoom* GameRoomCreate(GGame *game, int atX, int atY);

GRoom* GameRoomsCreate(GGame *game, GRoom *croom, int in, int jn, int min, int it, s2_CtVector *v);

void GameRoomCreateColliders(GGame *game, GRoom *r);