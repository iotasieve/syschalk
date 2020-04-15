#include <inc/game/player.h>
#include <inc/io.h>
#include <inc/containers/vector/vector.h>
#include <inc/game/bmath.h>
#include <inc/game/entity.h>
bool apressed = false;
int GGameplayPlayerScript(GGame *game)
{
    
    game->gameplay.cam.x = game->gameplay.player.pos.x;
    game->gameplay.cam.y = game->gameplay.player.pos.y;

    float sp = 10;
    if (game->keys.a1)
    {
        apressed = true;
    }
    if (game->keys.a2)
    {
        sp = 20;
        apressed = false;
    }


    bool pressed = false;
    if (game->keys.w)
    {
        game->gameplay.player.accel.y = -sp;
        game->gameplay.player.vel.y += game->gameplay.player.accel.y;
        game->gameplay.player.dir = 0;
        pressed = true;
    }
    if (game->keys.a)
    {
        
        game->gameplay.player.accel.x = -sp;
        game->gameplay.player.vel.x += game->gameplay.player.accel.x;
        game->gameplay.player.dir = 1;
        pressed = true;
    }
    if (game->keys.s)
    {
        game->gameplay.player.accel.y = sp;
        game->gameplay.player.vel.y += game->gameplay.player.accel.y;
        game->gameplay.player.dir = 2;
        pressed = true;
    }
    if (game->keys.d)
    {

        game->gameplay.player.accel.x = sp;
        game->gameplay.player.vel.x += game->gameplay.player.accel.x;
        game->gameplay.player.dir = 3;
        pressed = true;
    }
    GCollider sc;
    sc.r.x = game->gameplay.player.pos.x+game->gameplay.player.vel.y*game->delta+2;
    sc.r.y = game->gameplay.player.pos.y+game->gameplay.player.vel.y*game->delta+10;
    sc.r.w = 16-4;
    sc.r.h = 16-10;

    game->gameplay.player.state = PLAYER_STATE_WALK;
    if (!pressed)
    {
        game->gameplay.player.state = PLAYER_STATE_IDLE;
    game->gameplay.player.vel.y /= 1.1;
    }
    for (int i = 0; i < (game->gameplay.colliders->size); i++)
    {
        GCollider *srect = s2_CtVectorIndex(game->gameplay.colliders, i);
        bool col = GCheckCollision(srect, &sc);
        if (col)
        {
            if (srect->solid)
            {
                GVec2 displacement = {game->gameplay.player.pos.x+game->gameplay.player.vel.x*game->delta,
                        game->gameplay.player.pos.y+game->gameplay.player.vel.y*game->delta};
                GVec2 difference = {(displacement.x - srect->r.x), (displacement.y - srect->r.y)};
                game->gameplay.player.vel.x = difference.x;
                game->gameplay.player.vel.y = difference.y;
            }
            else if (srect->zombie && srect->active)
            {
                game->state = 3;
            }
        }
    }

    if (game->gameplay.player.vel.x > 10*sp) game->gameplay.player.vel.x = 100;
    if (game->gameplay.player.vel.x < -10*sp) game->gameplay.player.vel.x = -100;
    if (game->gameplay.player.vel.y > 10*sp) game->gameplay.player.vel.y = 100;
    if (game->gameplay.player.vel.y < -10*sp) game->gameplay.player.vel.y = -100;

    game->gameplay.player.pos.x += game->gameplay.player.vel.x*game->delta;
    game->gameplay.player.pos.y += game->gameplay.player.vel.y*game->delta;

    if (!game->keys.a1 && apressed == true)
    {
        apressed = false;
        if (game->gameplay.player.dir == 1)
            GAddProjectile(game, game->gameplay.player.pos.x, game->gameplay.player.pos.y, -100, 0);
        if (game->gameplay.player.dir == 3)
            GAddProjectile(game, game->gameplay.player.pos.x, game->gameplay.player.pos.y, 100, 0);
        if (game->gameplay.player.dir == 0)
            GAddProjectile(game, game->gameplay.player.pos.x, game->gameplay.player.pos.y, 0, -100);
        if (game->gameplay.player.dir == 2)
            GAddProjectile(game, game->gameplay.player.pos.x, game->gameplay.player.pos.y, 0, 100);
    }

    game->gameplay.player.accel.x = 0;
    game->gameplay.player.accel.y = 0;

    game->gameplay.player.vel.x /= 1.1;
    game->gameplay.player.vel.y /= 1.1;


    

}

void GGameplayPlayerRender(GGame *game)
{
    GVec2 r = {game->gameplay.player.pos.x, game->gameplay.player.pos.y};   
    GRemapToCameraP(&r, &game->gameplay.cam);
    
    if (game->gameplay.player.state == PLAYER_STATE_IDLE)
    {
        if (game->gameplay.player.dir == 1)
        {
            GScreenBlitB(game->textures.tileset, r.x, r.y, 0, 0+16, 16, 16);
        }
        else if (game->gameplay.player.dir == 3)
        {
            GScreenBlitB(game->textures.tileset, r.x, r.y, 0, 0, 16, 16);
        }
        else if (game->gameplay.player.dir == 0)
        {
            GScreenBlitB(game->textures.tileset, r.x, r.y, 0, 0+32, 16, 16);
        }
        else if (game->gameplay.player.dir == 2)
        {
            GScreenBlitB(game->textures.tileset, r.x, r.y, 0, 0+64, 16, 16);
        }
    }
    else
    {       
        if (game->gameplay.player.dir == 1)
        {
            GScreenBlitB(game->textures.tileset, r.x, r.y, ((int)(game->uptime * 5) % 2) ? 0 : 32, 0+16, 16, 16);
        }
        else if (game->gameplay.player.dir == 3)
        {
            GScreenBlitB(game->textures.tileset, r.x, r.y, ((int)(game->uptime * 5) % 2) ? 0 : 32, 0, 16, 16);
        }
        else if (game->gameplay.player.dir == 0)
        {
            GScreenBlitB(game->textures.tileset, r.x, r.y, ((int)(game->uptime * 5) % 3)*16, 0+48, 16, 16);
        }
        else if (game->gameplay.player.dir == 2)
        {
            GScreenBlitB(game->textures.tileset, r.x, r.y, ((int)(game->uptime * 5) % 3)*16, 0+80, 16, 16);
        }  
    }
    
}