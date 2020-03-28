#include <inc/types.h>
#include <inc/port.h>

// Misc
#define S2_VGAPORT_MISCW 0x3C2 

// Sequencer
#define S2_VGAPORT_SEQIP 0x3C4
#define S2_VGAPORT_SEQDP 0x3C5

// Graphics controller
#define S2_VGAPORT_GCTIP 0x3CE
#define S2_VGAPORT_GCTDP 0x3CF

// Attribute controller
#define S2_VGAPORT_ACTIP 0x3C0 // Index port
#define S2_VGAPORT_ACTRP 0x3C1 // Read port
#define S2_VGAPORT_ACTWP 0x3C0 // Write port
#define S2_VGAPORT_ACTRS 0x3DA // Reset port

// CRT contorller
#define S2_VGAPORT_CRTIP 0x3D4
#define S2_VGAPORT_CRTDP 0x3D5

bool s2_VGARegistersWrite(s2_Byte *registers);

bool s2_VGASupportsMode(s2_UInt32 width, s2_UInt32 height, s2_UInt32 depth);

s2_Byte* s2_VGAGetBufferLocation();

bool s2_VGASetMode(s2_UInt32 width, s2_UInt32 height, s2_UInt32 depth);

// extern inline void s2_VGAPlotPixelI(s2_UInt32 x, s2_UInt32 y, s2_UInt8 colorIndexed);

void s2_VGAClear4I(s2_UInt8 colorIndexed);

void s2_VGADrawBuffer8I(s2_UInt8 *buffer);

void s2_VGADrawBuffer4I(s2_UInt8 *buffer);

void s2_VGAReprogramPallete8();