#include <inc/game/room.h>
#include <inc/game/texture.h>
#include <inc/containers/vector/vector.h>
#include <inc/game/entity.h>


void GameRoomRender(GGame *game, GRoom *room, int atX, int atY)
{
    GVec2 tilepos;
    int roomw = 9;   
    int roomh = 9;   
    tilepos.x = atY*16*(roomh+2);
    tilepos.y = atX*16*(roomw+2);
    
    GRemapToCameraP(&tilepos, &game->gameplay.cam);


    if (tilepos.x > gScr->width) return;
    if (tilepos.y > gScr->height) return;
    if ((tilepos.x+((roomw+2)*16)) < 0) return;
    if ((tilepos.y+((roomh+2)*16)) < 0) return;

    for (int i = 0; i < roomw+2; i++)
    {
        for (int j = 0; j < roomh+2; j++)
        {

            tilepos.x = i*16+atY*16*(roomh+2);
            tilepos.y = j*16+atX*16*(roomw+2);
            GRemapToCameraP(&tilepos, &game->gameplay.cam);

            if (game->gameplay.exitRoom->x == atY && game->gameplay.exitRoom->y == atX)
            {
                GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 48, 32, 16, 16);
                continue;
            }
            if ((room->mazeType != 0) && (i >= 1) && (j >= 1) && (j < roomh+1) && (i < roomw+1))
            {
                int mazeIdx = (room->mazeType-1);
                int pix = GTexMap(game->textures.mazes, (i-1)+mazeIdx*9, (j-1));
                if (pix & 0xFF000000)
                {
                    GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 48, 48, 16, 16);
                    continue;
                }
            }
            
   
            if ((room->doorLeft) && i == 0)
                if (j == roomw/2+1)
                {
                    GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 48, 16, 16, 16);
                    continue;
                }
            if (room->doorTop && j == 0)
                if (i == roomh/2+1)
                {
                    GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 48, 16, 16, 16);
                    continue;
                }
            if (room->doorRight && i == roomh+1)
                if (j == roomw/2+1)
                {
                    GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 48, 16, 16, 16);
                    continue;
                }
            if (room->doorBottom && j == roomw+1)
                if (i == roomh/2+1)
                {
                    GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 48, 16, 16, 16);
                    continue;
                }

            if (i == 0 && j == 0)
            {
                GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 160, 0, 16, 16);
                continue;
            }
            if (i == (roomw+1) && j == 0)
            {
                GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 160+32, 0, 16, 16);
                continue;
            }
            if (i == 0 && j == (roomh+1))
            {
                GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 160, 32, 16, 16);
                continue;
            }
            if (i == (roomw+1) && j == (roomh+1))
            {
                GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 160+32, 32, 16, 16);
                continue;
            }
            if (i == (roomw+1) && j == (roomh+1))
            {
                GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 192, 32, 16, 16);
                continue;
            }
            if (j == 0)
            {
                GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 160+16, 0, 16, 16);
                continue;
            }
            if (i == 0)
            {
                GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 160, 16, 16, 16);
                continue;
            }
            if (i == (roomw+1))
            {
                GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 192, 16, 16, 16);
                continue;
            }
            if (j == (roomh+1))
            {
                GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 176, 32, 16, 16);
                continue;
            }
            // Stone Tile
            GScreenBlitB(game->textures.tileset, tilepos.x, tilepos.y, 48, 16, 16, 16);
        }
    }
}


void GGameRoomsRender(GGame *game, GRoom *cr, int in, int jn)
{
    if (cr)
    {
        GameRoomRender(game, cr, cr->y, cr->x);
        GGameRoomsRender(game, cr->bottom, in, jn+1);
        GGameRoomsRender(game, cr->top, in, jn-1);
        GGameRoomsRender(game, cr->left, in-1, jn);
        GGameRoomsRender(game, cr->right, in+1, jn);
    }
    return;
}
    
void GameRoomHandle(GGame *game)
{

}

GCollider* CreateCollider()
{

}

bool GameCheckRoomIn(GRoom *root, int i, int j, int in, int jn)
{
    if (root)
    {
        if (root->x == i && root->y == j)
        {
            return true;
        }
        return GameCheckRoomIn(root->bottom, i, j, in, jn+1) ||
            GameCheckRoomIn(root->top, i, j, in, jn-1) ||
            GameCheckRoomIn(root->left, i, j, in-1, jn) ||
            GameCheckRoomIn(root->right, i, j, in+1, jn);
    }
    return false;
}

void GameRoomsList(GRoom *room)
{
    if (room)
    {
        s2_QEMUDebugPrint("xy=========");
        s2_QEMUDebugPrint(s2_ToHex(room->x));
        s2_QEMUDebugPrint(s2_ToHex(room->y));
        GameRoomsList(room->bottom);
        GameRoomsList(room->top);
        GameRoomsList(room->left);
        GameRoomsList(room->right);
    }
    return;
}

GRoom* GameRoomsCreate(GGame *game, GRoom *croom, int in, int jn, int min, int it, s2_CtVector *v)
{
    if (it == 0)
    {
        v = s2_CtVectorCreate(v, OF(GVec2*));
    }
    if (v->size > 511) return NULL;

    if (it == min)
    {
        return NULL;
    }
    // s2_QEMUDebugPrint("================================");
    // s2_QEMUDebugPrint(s2_ToHex(in));
    // s2_QEMUDebugPrint(s2_ToHex(jn));
    // GameRoomsList(game->gameplay.rootRoom);

    for (int i = 0; i < v->size; i++)
    {
        GVec2 *gv = s2_CtVectorIndex(v, i);
        if((in == gv->x) && (jn == gv->y))
        {
            return NULL;
        }
    }
    GVec2 *vr = NEW(GVec2, 1);
    vr->x = in;
    vr->y = jn;

    s2_CtVectorPush(v, vr);

    GRoom *r = GameRoomCreate(game, in, jn);
    game->gameplay.exitRoom = r;
    
    r->x = in;
    r->y = jn;


    if (it == 0)
    {
        s2_QEMUDebugPrint("ok");
        game->gameplay.rootRoom = r;
    }

    if ((GRand()/1000)%2)
    {
        r->bottom = GameRoomsCreate(game, r->bottom, in, jn+1, min, it+1, v);
        r->doorBottom = false;

        if (r->bottom != NULL)
        {
            r->doorBottom = true;
            r->bottom->doorTop = true;
        }
    }

    r->top =    GameRoomsCreate(game, r->top, in, jn-1, min, it+1, v);
    r->doorTop = false;

    

    if (r->top)
    {
        r->doorTop = true;
        r->top->doorBottom = true;
    }

    r->left =   GameRoomsCreate(game, r->left, in-1, jn, min, it+1, v);
    r->doorLeft = false;

    if (r->left)
    {
        r->doorLeft = true;
        r->left->doorRight = true;
    }

    if ((GRand()/1000)%2)
    {
        r->right =  GameRoomsCreate(game, r->right, in+1, jn, min, it+1, v);
        r->doorRight = false;

        if (r->right)
        {
            r->doorRight = true;
            r->right->doorLeft = true;
        }
    }
    // if (it == 0)
    // {
    //     s2_CtVectorDispose(v);
    //     DELETE(v);
    // }

    return r;
}

void GameRoomCreateColliders(GGame *game, GRoom *r)
{
    if (r)
    {     
        if (GRand()%2 && game->gameplay.entityCount < 60)
            GAddZombie(game, 16*2+(r->x*16*11), 16*2+(r->y*16*11));

        for (int i = 1; i < 10; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                GCollider *c = NEW(GCollider, 1);
                if (j == 0)
                    c->r.x = 16*i;
                if (j == 1)
                    c->r.y = 16*i;
                if (j == 2)
                {
                    c->r.y = (10*16);
                    c->r.x = 16*i;
                }
                if (j == 3)
                {
                    c->r.x = (10*16);
                    c->r.y = 16*i;
                }
                c->solid = true;
                
                if (i == 9/2+1)
                {
                    if (j == 0 && r->doorTop)
                        c->solid = false;
                    if (j == 1 && r->doorLeft)
                        c->solid = false;
                    if (j == 2 && r->doorBottom)
                        c->solid = false;
                    if (j == 3 && r->doorRight)
                        c->solid = false;
                }
                c->r.x += r->x*16*11;
                c->r.y += r->y*16*11;
                c->r.w = 16;
                c->r.h = 16;
                s2_CtVectorPush(game->gameplay.colliders, c);
            }
        }

        if (r->mazeType != 0)
        {
            int mazeIdx = (r->mazeType-1);
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    int pix = GTexMap(game->textures.mazes, i+mazeIdx*9, j);
                    if (pix & 0xFF000000)
                    {
                        GCollider *c = NEW(GCollider, 1);
                        c->r.x = 16*i+16+r->x*16*11;
                        c->r.y = 16*j+16+r->y*16*11;
                        c->r.w = 16;
                        c->r.h = 16;
                        c->solid = true;
                        s2_CtVectorPush(game->gameplay.colliders, c);

                    }
                }
            }
        }

        GameRoomCreateColliders(game, r->top);
        GameRoomCreateColliders(game, r->bottom);
        GameRoomCreateColliders(game, r->left);
        GameRoomCreateColliders(game, r->right);
    }
}

GRoom* GameRoomCreate(GGame *game, int atX, int atY)
{
    // Room
    GRoom *res = NEW(GRoom, 1);

    res->mazeType = 0;
    if ((GRand()/1000)%2 == 0)
    {
        res->mazeType = (GRand()/1000)%4;

    }

    
    return res;
}