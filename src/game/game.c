#include <inc/game/game.h>
#include <inc/game/bmp.h>
#include <inc/game/player.h>
#include <inc/game/room.h>
#include <inc/game/entity.h>

#define GAME_STATE_MENU 0
#define GAME_STATE_GAMEPLAY 1
#define GAME_STATE_FINISH 2
#define GAME_STATE_FAIL 3


int GMenuRender(GGame *game)
{
    GScreenBlitB(game->textures.tileset, 160-(197/2), 20, 0, 128, 197, 32);
    GScreenText("Easy", 160-(197/4), 100);
    GScreenText("Medium", 160-(197/4), 120);
    GScreenText("Hard", 160-(197/4), 140);
    GScreenText("WASD, J - Action 1, K - Action 2", 80-(197/4), 180);
    GScreenBlitB(game->textures.tileset, 160-(197/4)-12, 100+(game->menu.picked*20), 128, 8, 8, 8);
}

int GGenericHandle(GGame *game)
{
    s2_Event *ev = s2_EventUnqueue();
    if ((ev != NULL) && (ev->ev_type == S2_EPS2KEYBOARD))
    {
        s2_PS2KeyboardEvent *kev = (s2_PS2KeyboardEvent*)(ev->event);
        if (kev->pressed)
        {
            // s2_QEMUDebugPrint("PRS");   
        }
        else
        {
            // s2_QEMUDebugPrint("REL");   
        }
        switch (s2_PS2ConvertCodeToChar(kev->key))
        {
        case 'w':
            game->keys.w = kev->pressed;
            break;
        case 'a':
            game->keys.a = kev->pressed;
            break;
        case 's':
            game->keys.s = kev->pressed;
            break;
        case 'd':
            game->keys.d = kev->pressed;
            break;
        case 'j':
            game->keys.a1 = kev->pressed;
            break;
        case 'k':
            game->keys.a2 = kev->pressed;
            break;
        
        default:
            break;
        }
    } 

}

int GMenuHandle(GGame *game)
{
    s2_Event *ev = s2_EventUnqueue();
    if (ev->ev_type == S2_EPS2KEYBOARD)
    {
        s2_PS2KeyboardEvent *kev = (s2_PS2KeyboardEvent*)(ev->event);
        if (!kev->pressed)
        {
            if (s2_PS2ConvertCodeToChar(kev->key) == 'w' && game->menu.picked > 0)
            {
                game->menu.picked -= 1;
            }
            else if (s2_PS2ConvertCodeToChar(kev->key) == 's' && game->menu.picked < 2)
            {
                game->menu.picked += 1;
            }
            else if (s2_PS2ConvertCodeToChar(kev->key) == 'j' || s2_PS2ConvertCodeToChar(kev->key) == 'k')
            {
                game->timeRem = game->times[game->menu.picked];
                game->state = GAME_STATE_GAMEPLAY;
            }
        }
    }
}

int snap(float v)
{
    if ((v) < 0) return ((int)v-1);
    return v;
}

int GGameplayRender(GGame *game)
{
    game->timeRem -= game->delta*1000;
    // if ((int)game->uptime % 10)
    // {

    //     s2_QEMUDebugPrint();
    //     s2_QEMUDebugPrint(s2_ToDec((game->gameplay.exitRoom->x)));
    //     s2_QEMUDebugPrint("================================");
    //     s2_QEMUDebugPrint(s2_ToDec(((game->gameplay.player.pos.x/16)/11)));
    //     s2_QEMUDebugPrint(s2_ToDec(((game->gameplay.player.pos.y/16)/11)));
    //     s2_QEMUDebugPrint("pppppppppppppppppppppppppppppppp");
    // }

    if (snap(game->gameplay.player.pos.x/16/11) == game->gameplay.exitRoom->x &&
        snap(game->gameplay.player.pos.y/16/11) == game->gameplay.exitRoom->y)
    {
        s2_QEMUDebugPrint("GOT");
        game->state = GAME_STATE_FINISH;
    }
    
    
    GGameRoomsRender(game, game->gameplay.rootRoom, 0, 0);
    GGameplayEntitiesRender(game);

    GGameplayPlayerRender(game);
    char *gt = s2_ToDec((game->timeRem/1000)%60);
    GScreenText(gt, 16, 0);
    s2_MemoryFree(gt);
    gt = s2_ToDec((game->timeRem/1000)/60);
    GScreenText(gt, 0, 0);
    GScreenText(":", 8, 0);
    s2_MemoryFree(gt);


    gt = s2_ToHex(game->gameplay.exitRoom);
    GScreenText(gt, 0, 8);
    s2_MemoryFree(gt);

    // gt = s2_ToDec((game->gameplay.exitRoom->y));
    // GScreenText(gt, 0, 16);
    // s2_MemoryFree(gt);

    // gt = s2_ToDec((game->gameplay.exitRoom->x));
    // GScreenText(gt, 0, 24);
    // s2_MemoryFree(gt);

    // gt = s2_ToDec(snap(game->gameplay.player.pos.x/16/11));
    // GScreenText(gt, 0, 24+8);
    // s2_MemoryFree(gt);

    // gt = s2_ToDec(snap(game->gameplay.player.pos.y/16/11));
    // GScreenText(gt, 0, 24+16);
    // s2_MemoryFree(gt);
}


int GGameplayHandle(GGame *game)
{
    if (game->timeRem > 40000000) {
        game->state = GAME_STATE_FAIL;
        return 0;
    }
    GGameplayEntitiesHandle(game);
    GGameplayPlayerScript(game);    
}

GGame* GInit()
{
    GGame *g = NEW(GGame, 1);
    GSrand(s2_msUptime);
    g->gameplay.colliders = s2_CtVectorCreate(NEW(s2_CtVector, 1), OF(GCollider*));

    
    g->gameplay.killCount = 0;

    g->textures.mazes = GReadBMPFile("ASSETS/MAZES.BMP");
    // Camera
    g->gameplay.cam.x = 0;
    g->gameplay.cam.y = 0;

    // Entites
    g->gameplay.entityCount = 0;
    g->gameplay.entities = NEW(GEntity*, 256);

    
    GameRoomsCreate(g, g->gameplay.rootRoom, 0, 0, 2, 0, 0);
    GameRoomCreateColliders(g, g->gameplay.rootRoom);
    
    

    // Player
    g->gameplay.player.pos.x = 16*5;
    g->gameplay.player.pos.y = 16*5;
    g->gameplay.player.dir = 0;
    g->gameplay.player.vel.x = 0;
    g->gameplay.player.vel.y = 0;

    // Time limits
    g->times[0] = 180000;
    g->times[1] = 120000;
    g->times[2] = 60000;

    // Default keys
    g->keys.w = 0;
    g->keys.a = 0;
    g->keys.s = 0;
    g->keys.d = 0;
    g->keys.a1 = 0;
    g->keys.a2 = 0;


    // Tileset read
    GTexture *tst = GReadBMPFile("ASSETS/TILESET.BMP");

    g->textures.tileset = tst;
    g->uptime = 0;

    g->state = GAME_STATE_MENU;
    // Menu
    g->menu.picked = 0;

    return g;
}

void GRoomsDestroy(GGame *g, GRoom *r)
{
    if (r)
    {
        // s2_QEMUDebugPrint(s2_ToHex(r));

        GRoomsDestroy(g, r->bottom);
        GRoomsDestroy(g, r->top);
        GRoomsDestroy(g, r->left);
        GRoomsDestroy(g, r->right);

        DELETE(r);
    }
    return;
}

void GReinitialize(GGame *g)
{
    // for (int i = 0; i < g->gameplay.colliders->size; i++)
    // {
    //     GCollider *c = s2_CtVectorPop(g->gameplay.colliders);    
    //     DELETE(c);
    // }

    // s2_CtVectorDispose(g->gameplay.colliders);
    // DELETE(g->gameplay.colliders);

    // GRoomsDestroy(g, g->gameplay.rootRoom);

    // Initialize again
    GSrand(s2_msUptime);
    g->gameplay.killCount = 0;

    // for (int i = 0; i < 64; i++)
    // {
    //     if(g->gameplay.entities[i]->collider > heap_start) g->gameplay.entities[i]->collider->active = false;
    // }

    // g->gameplay.colliders->size = 0;
    g->gameplay.colliders = s2_CtVectorCreate(g->gameplay.colliders, sizeof(GCollider*));
    // s2_QEMUDebugPrint(s2_ToHex(g->gameplay.colliders->size));
    g->gameplay.cam.x = 0;
    g->gameplay.cam.y = 0;
    g->gameplay.entityCount = 0;
    GameRoomsCreate(g, g->gameplay.rootRoom, 0, 0, 10, 0, 0);
    GameRoomCreateColliders(g, g->gameplay.rootRoom);

    // Player
    g->gameplay.player.pos.x = 16*5;
    g->gameplay.player.pos.y = 16*5;
    g->gameplay.player.dir = 0;
    g->gameplay.player.vel.x = 0;
    g->gameplay.player.vel.y = 0;

    // Time limits
    g->times[0] = 180000;
    g->times[1] = 120000;
    g->times[2] = 60000;

    // Default keys
    g->keys.w = 0;
    g->keys.a = 0;
    g->keys.s = 0;
    g->keys.d = 0;
    g->keys.a1 = 0;
    g->keys.a2 = 0;

    g->uptime = 0;

    g->state = GAME_STATE_MENU;
    // Menu
    g->menu.picked = 0;
}


int GMain()
{
    GInitScreen();
    GScreenClear(0xFF008080);
    GTexture *fnt = GReadBMPFile("ASSETS/FONT.BMP");
    GTexture *cd = GReadBMPFile("ASSETS/CDLOAD.BMP");
    GFontInit(fnt, "abcdefghijklmnopqrstuvwxyz1234567890!.,?:-", "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!.,?:-");
    GScreenText("Loading resources", (gScr->width/2)-(17*8)/2, (gScr->height/2)-cd->height/2-16);
    GScreenBlitB(cd, (gScr->width/2)-cd->width/6, (gScr->height/2)-cd->height/2, 0, 0, cd->width/3, cd->height);
    GScreenDraw();
    s2_PITSleep(1000);
    while (s2_EventUnqueue()) {}

    char *tx;


    GGame *g = GInit();

    float delta = 0;
    s2_UInt32 lastTime = s2_msUptime;
    float cscore = 0;

    while (true)
    {
        
        GScreenClear(0x0);

        switch (g->state)
        {
        case GAME_STATE_MENU:

            cscore = 0;
            GScreenText("score: ", 110, 170);
            tx = s2_ToDec(g->score);
            GScreenText(tx, 80+(8*15), 170);
            DELETE(tx);
            GMenuHandle(g);
            
            GMenuRender(g);
            break;
        case GAME_STATE_GAMEPLAY:
            GGenericHandle(g);
            GGameplayHandle(g);
            GGameplayRender(g);
            // for (int i = 0; i < g->gameplay.colliders->size; i++)
            // {
            //     GCollider *c = s2_CtVectorIndex(g->gameplay.colliders, i);
            //     GVec2 v = {c->r.x, c->r.y};
            //     GRemapToCameraP(&v, &g->gameplay.cam);
                
            //     GScreenRect(v.x, v.y, c->r.w, c->r.h, 0xFF00FF00 | (c->solid ? 0xFF : 0));
            // }
            break;
        case GAME_STATE_FINISH:
            GScreenText("You did it!", (gScr->width/2)-(11*8/2), 48);
            cscore += (g->timeRem/1000)*((g->menu.picked+1));
            GScreenText("Time bonus: ", 80, 48+16);
            tx = s2_ToDec(cscore);
            GScreenText(tx, 80+(8*12), 48+16);
            DELETE(tx);

            GScreenText("Kill score: ", 80, 48+32);
            cscore += g->gameplay.killCount*100;

            tx = s2_ToDec(g->gameplay.killCount*100);
            GScreenText(tx, 80+(8*12), 48+32);
            DELETE(tx);

            GScreenText("Total score: ", 80, 48+48);
            tx = s2_ToDec(cscore);
            GScreenText(tx, 80+(8*13), 48+48);
            DELETE(tx);

            g->score += cscore;

            GScreenText("Overall score: ", 80, 48+64);
            tx = s2_ToDec(g->score);
            GScreenText(tx, 80+(8*15), 48+64);
            DELETE(tx);

            GScreenText("Press any key to continue", 80, 150);
            GScreenDraw();
            
            GReinitialize(g);

            s2_PITSleep(1000);
            while (s2_EventUnqueue()) {}

            while (s2_EventAwait()->ev_type != S2_EPS2KEYBOARD) {}
            g->state = GAME_STATE_MENU;
            while (s2_EventUnqueue()) {}

            break;
        case GAME_STATE_FAIL:
            GScreenText("You lost :c", (gScr->width/2)-(11*8/2), 48);
            GScreenText("Press any key to continue", 80, 150);

            GReinitialize(g);
            GScreenDraw();

            s2_PITSleep(1000);
            while (s2_EventUnqueue()) {}

            while (s2_EventAwait()->ev_type != S2_EPS2KEYBOARD) {}
            
            g->state = GAME_STATE_MENU;
            while (s2_EventUnqueue()) {}

            break;
        default:
            break;
        }

        // GScreenText("Unfreed-Mem", 320-(11*8), 0);
        // char *d = s2_ToDec(memleaks);
        // GScreenText(d, 320-(8*8), 8);
        // DELETE(d);

        // d = s2_ToDec((s2_UInt32)(1/delta));
        // GScreenText(d, 320-(8*8), 16);
        // GScreenText(" - FPS", 320-(6*8), 16);
        // DELETE(d);

        char *d = s2_ToHex(memUpper);
        // GScreenText("MemUpper", 0, 70);
        GScreenText(d, 0, 32);
        DELETE(d);



        delta = (s2_msUptime - lastTime)/1000.0;
        g->delta = delta;
        g->uptime += delta;
        lastTime = s2_msUptime;
        // Throttle
        s2_PITSleep(1);
        GScreenDraw();
    }
}