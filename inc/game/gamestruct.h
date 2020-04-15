
#pragma once
#include <inc/game/texture.h>
#include <inc/types.h>
#include <inc/game/bmath.h>
#include <inc/containers/vector/vector.h>


#define GAME_ENTITY_ZOMBIE 0
#define GAME_ENTITY_PROJECTILE 1

typedef struct
{
    GVec2 pos;
    GVec2 vel;
    GVec2 accel;
    int dir;
    int state;
} GPlayer;

typedef struct
{
    GVec2 pos;
    GVec2 vel;
    int dir;
    int entityType;
    int state;
    GCollider *collider;
} GEntity;


typedef struct GRoomS
{
    struct GRoomS *top;
    struct GRoomS *bottom;
    struct GRoomS *left;
    struct GRoomS *right;
    bool doorBottom;
    bool doorLeft;
    bool doorRight;
    bool doorTop;
    int mazeType;
    int x;
    int y;
    int zombies;
} GRoom;

typedef struct
{
    int state;

    struct
    {
        GTexture *tileset;
        GTexture *mazes;
    } textures;
    struct
    {
        s2_UInt32 picked;
    } menu;
    struct // Scene
    {
        GPlayer player;
        GRoom *rootRoom;
        GRoom *exitRoom;
        GCamera cam;
        s2_CtVector *colliders;
        GEntity** entities;
        int entityCount;
        int killCount;
    } gameplay;
    struct
    {
        bool w;
        bool a;
        bool s;
        bool d;
        bool a1;
        bool a2;
    } keys;
    
    s2_UInt32 times[3];
    s2_UInt32 timeRem;
    float uptime;
    float delta;
    int score;
} GGame;