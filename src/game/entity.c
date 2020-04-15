#include <inc/game/entity.h>

void GAddProjectile(GGame *game, float x, float y, float vx, float vy)
{
        
    GEntity *e = NEW(GEntity, 1);

    e->collider = NEW(GCollider, 1);
    e->collider->projectile = true;
    e->collider->r.x = 0;
    e->collider->r.y = 0;
    e->collider->r.h = 8;
    e->collider->r.w = 8;
    e->collider->zombie = false;
    e->collider->solid = false;
    s2_CtVectorPush(game->gameplay.colliders, e->collider);

    e->dir = 0;
    e->entityType = GAME_ENTITY_PROJECTILE;
    e->pos.x = x;
    e->pos.y = y;
    e->vel.x = vx;
    e->vel.y = vy;
    e->collider->active = 1;
    e->state = ENTITY_STATE_IDLE;
    for (int i = 0; i < 200; i++)
    {
        if (!game->gameplay.entities[i]->collider->active)
        {
            game->gameplay.entities[i] = e;
            game->gameplay.entityCount += 1;
            break;
        }
    }
}

void GAddZombie(GGame *game, float x, float y)
{
    GEntity *e = NEW(GEntity, 1);

    e->collider = NEW(GCollider, 1);
    e->collider->projectile = false;
    e->collider->r.x = 0;
    e->collider->r.y = 0;
    e->collider->r.h = 6;
    e->collider->r.w = 16;
    e->collider->active = 1;
    e->collider->zombie = true;
    e->collider->solid = false;
    s2_CtVectorPush(game->gameplay.colliders, e->collider);

    e->dir = 0;
    e->entityType = GAME_ENTITY_ZOMBIE;
    e->pos.x = x;
    e->pos.y = y;
    e->vel.x = 0;
    e->vel.y = 0;
    e->state = ENTITY_STATE_IDLE;
    
    for (int i = 0; i < 60; i++)
    {
        if (!game->gameplay.entities[i]->collider->active)
        {
            game->gameplay.entities[i] = e;
            game->gameplay.entityCount += 1;
            break;
        }
    }
}

void GGameplayEntitiesHandle(GGame *game)
{
    for (int i = 0; i < game->gameplay.entityCount; i++)
    {
        GEntity *entity = game->gameplay.entities[i];
        GVec2 r = {entity->pos.x,entity->pos.y};   
        GRemapToCameraP(&r, &game->gameplay.cam);
        if (entity->collider->active != 1) continue;
        if (entity->entityType == GAME_ENTITY_ZOMBIE)
        {
            entity->vel.x = (game->gameplay.player.pos.x-entity->pos.x)/1.1;
            entity->vel.y = (game->gameplay.player.pos.y-entity->pos.y)/1.1;
            if (entity->vel.x < 0 && entity->vel.x > entity->vel.y) entity->dir = 1; 
            if (entity->vel.x > 0 && entity->vel.x > entity->vel.y) entity->dir = 3; 
            if (entity->vel.y < 0 && entity->vel.y > entity->vel.x) entity->dir = 0; 
            if (entity->vel.y > 0 && entity->vel.y > entity->vel.x) entity->dir = 2; 
            entity->state = ENTITY_STATE_WALK;
            
            GVec2 vp = {entity->pos.x, entity->pos.y};
            GRemapToCameraP(&vp, &game->gameplay.cam);
            if (vp.x < 0) continue;
            if (vp.y < 0) continue;
            if (vp.x > gScr->width) continue;
            if (vp.y > gScr->width) continue;

            GCollider sc;
            sc.r.x = entity->pos.x+entity->vel.y*game->delta+2;
            sc.r.y = entity->pos.y+entity->vel.y*game->delta+10;
            sc.r.w = 16-4;
            sc.r.h = 16-10;

            for (int i = 0; i < (game->gameplay.colliders->size); i++)
            {
                GCollider *srect = s2_CtVectorIndex(game->gameplay.colliders, i);
                bool col = GCheckCollision(srect, &sc);
                if (col)
                {
                    if (srect->solid)
                    {
                        GVec2 displacement = {entity->pos.x+entity->vel.x*game->delta,
                                entity->pos.y+entity->vel.y*game->delta};
                        GVec2 difference = {(displacement.x - srect->r.x), (displacement.y - srect->r.y)};
                        entity->vel.x = difference.x;
                        entity->vel.y = difference.y;
                        break;
                    }
                    else if (srect->projectile && srect->active)
                    {
                        entity->collider->active = false;
                        game->gameplay.entityCount -= 1;
                        game->gameplay.killCount += 1;
                        srect->active = 0;
                        break;
                    }
                }
            }

            entity->pos.x += entity->vel.x*game->delta;
            entity->pos.y += entity->vel.y*game->delta;
            entity->collider->r.x = entity->pos.x;
            entity->collider->r.y = entity->pos.y+6;

        }
        else if (entity->entityType == GAME_ENTITY_PROJECTILE)
        {
            entity->pos.x += entity->vel.x*game->delta;
            entity->pos.y += entity->vel.y*game->delta;
            entity->collider->r.x = entity->pos.x;
            entity->collider->r.y = entity->pos.y;

            if (entity->dir > 1000)
            {
                entity->collider->active = false;
                game->gameplay.entityCount -= 1;

            }

            // Actually lifespan of entity
            entity->dir += 1000*game->delta;
        }


    }
}

void GGameplayEntitiesRender(GGame *game)
{
    for (int i = 0; i < game->gameplay.entityCount; i++)
    {
        GEntity *entity = game->gameplay.entities[i];
        GVec2 r = {entity->pos.x,entity->pos.y};   
        GRemapToCameraP(&r, &game->gameplay.cam);
        if (entity->collider->active != 1) continue;

        if (entity->entityType == GAME_ENTITY_ZOMBIE)
        {
            if (entity->state == ENTITY_STATE_IDLE)
            {
                if (entity->dir == 1)
                {
                    GScreenBlitB(game->textures.tileset, r.x, r.y, 80, 0+16, 16, 16);
                }
                else if (entity->dir == 3)
                {
                    GScreenBlitB(game->textures.tileset, r.x, r.y, 80, 0, 16, 16);
                }
                else if (entity->dir == 0)
                {
                    GScreenBlitB(game->textures.tileset, r.x, r.y, 80, 0+32, 16, 16);
                }
                else if (entity->dir == 2)
                {
                    GScreenBlitB(game->textures.tileset, r.x, r.y, 80, 0+64, 16, 16);
                }
            }
            else
            {       
                if (entity->dir == 1)
                {
                    GScreenBlitB(game->textures.tileset, r.x, r.y, ((int)(game->uptime * 5) % 2) ? 80 : 80+32, 0+16, 16, 16);
                }
                else if (entity->dir == 3)
                {
                    GScreenBlitB(game->textures.tileset, r.x, r.y, ((int)(game->uptime * 5) % 2) ? 80 : 80+32, 0, 16, 16);
                }
                else if (entity->dir == 0)
                {
                    GScreenBlitB(game->textures.tileset, r.x, r.y, 80+((int)(game->uptime * 5) % 3)*16, 0+48, 16, 16);
                }
                else if (entity->dir == 2)
                {
                    GScreenBlitB(game->textures.tileset, r.x, r.y, 80+((int)(game->uptime * 5) % 3)*16, 0+80, 16, 16);
                }  
            }
        }
        else if (entity->entityType == GAME_ENTITY_PROJECTILE)
        {
            GScreenBlitB(game->textures.tileset, r.x, r.y, 128+((int)(game->uptime * 5) % 4)*8, 0, 8, 8);
        }
       
    }
}