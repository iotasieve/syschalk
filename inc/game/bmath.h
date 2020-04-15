#pragma once
#include <inc/game/texture.h>

typedef struct 
{
    float x;
    float y;
} GCamera;

typedef struct 
{
    float x;
    float y;
} GVec2;

typedef struct
{
    float x;
    float y;
    float w;
    float h;
} GRect;

typedef struct
{
    GRect r;
    bool solid;
    bool projectile;
    bool zombie;
    bool active;
} GCollider;


GVec2* GRemapToCamera(GVec2 *orig, GCamera *cam);
GVec2* GRemapToCameraP(GVec2 *orig, GCamera *cam);

bool GCheckCollision(GCollider *c1, GCollider *c2);
GVec2* GVectorNormal(GVec2 *vec);

void GSrand(int ns);
int GRand();