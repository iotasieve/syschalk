#include <inc/game/bmath.h>
#include <inc/mem/manager.h>
static unsigned int seed = 1;

GVec2* GRemapToCamera(GVec2 *orig, GCamera *cam)
{
    GVec2 *r = NEW(GVec2, 1);
    r->x = -(orig->x + (gScr->width/2) + cam->x);
    r->y = -(orig->y + (gScr->height/2) + cam->y);
    return r;
}

void GSrand(int ns) 
{
    seed = (unsigned int) ns & 0x7fffffffU;
}

int GRand(void) 
{
    seed = (seed * 1103515245U + 12345U) & 0x7fffffffU;
    return (int)seed;
}
// Quick square root (from wikipedia);
float Q_sqrt( float number )
{	
	const float x2 = number * 0.5F;
	const float threehalfs = 1.5F;

	union {
		float f;
		s2_UInt32 i;
	} conv = {number}; // member 'f' set to value of 'number'.
	conv.i  = 0x5f3759df - ( conv.i >> 1 );
	conv.f  *= ( threehalfs - ( x2 * conv.f * conv.f ) );
	return 1/conv.f;
}

float GVecMagnitude(GVec2 *vec)
{
    return Q_sqrt((vec->x*vec->x) + (vec->y*vec->y));
}

GVec2* GVectorNormal(GVec2 *vec)
{
    float m = GVecMagnitude(vec);
    vec->x /= m;
    vec->y /= m;
    return vec;
}

GVec2* GRemapToCameraP(GVec2 *orig, GCamera *cam)
{
    orig->x = orig->x + (gScr->width/2) - cam->x;
    orig->y = orig->y + (gScr->height/2) - cam->y;
    return orig;
}

bool GCheckCollision(GCollider *c1, GCollider *c2)
{
    return (c2->r.x+c2->r.w) > (c1->r.x) &&
           (c2->r.y+c2->r.h) > (c1->r.y) &&
           (c2->r.x) < (c1->r.x+c1->r.w) &&
           (c2->r.y) < (c1->r.y+c1->r.h);
}
