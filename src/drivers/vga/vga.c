#include <inc/drivers/vga/vga.h>
#include <inc/mem/manager.h>
#include <inc/io.h>

s2_Byte *s2_VGALoc; 

void* memcpy(void *dest, void *src, s2_Size count)
{
    return s2_MemoryCopy(dest, src, count);
}

bool s2_VGARegistersWrite(s2_Byte *registers)
{
    // s2_TVMPrint(s2_ToHex(*(registers)), 0x70, 00);
    // Misc 
    s2_OutB(S2_VGAPORT_MISCW, *(registers++));


    // Sequencer
    for (int i = 0; i < 5; i++)
    {
        s2_OutB(S2_VGAPORT_SEQIP, i);
        s2_OutB(S2_VGAPORT_SEQDP, *(registers++));
    }
    
    // CRT controller 
    
    // CRT Safety thing 
    s2_OutB(S2_VGAPORT_CRTIP, 0x03);
    s2_OutB(S2_VGAPORT_CRTDP, s2_InB(S2_VGAPORT_CRTDP) | 0x80);
    s2_OutB(S2_VGAPORT_CRTIP, 0x11);
    s2_OutB(S2_VGAPORT_CRTDP, s2_InB(S2_VGAPORT_CRTDP) & ~0x80);

    registers[0x03] = registers[0x03] | 0x80;
    registers[0x11] = registers[0x11] & ~0x80;
 
    for (int i = 0; i < 25; i++)
    {
        s2_OutB(S2_VGAPORT_CRTIP, i);
        s2_OutB(S2_VGAPORT_CRTDP, *(registers++));
    }
   
    // Graphics contorller  
    for (int i = 0; i < 9; i++)
    {
        s2_OutB(S2_VGAPORT_GCTIP, i);
        s2_OutB(S2_VGAPORT_GCTDP, *(registers++));
    }

    // Attribute controller 
    for (int i = 0; i < 21; i++)
    {
        s2_InB(S2_VGAPORT_ACTRS);
        s2_OutB(S2_VGAPORT_ACTIP, i);
        s2_OutB(S2_VGAPORT_ACTWP, *(registers++));
    }
    
    s2_InB(S2_VGAPORT_ACTRS);
    s2_OutB(S2_VGAPORT_ACTIP, 0x20);
}

bool s2_VGASupportsMode(s2_UInt32 width, s2_UInt32 height, s2_UInt32 depth)
{
    return (width == 640 && height == 480 && depth == 4) || (width == 320 && height == 200 && depth == 8);
}

s2_Byte* s2_VGAGetBufferLocation()
{
    s2_OutB(S2_VGAPORT_GCTIP, 0x06);
    s2_UInt8 seg = s2_InB(S2_VGAPORT_GCTDP) & (3 << 2);
    switch (seg)
    {
        case 0: return (s2_Byte*)0x00000;
        case 4: return (s2_Byte*)0xA0000;
        case 8: return (s2_Byte*)0xB0000;
        case 12: return (s2_Byte*)0xB8000;
    default:
        return 0;
        break;
    }
}


bool s2_VGASetMode(s2_UInt32 width, s2_UInt32 height, s2_UInt32 depth)
{
    if (!s2_VGASupportsMode(width, height, depth))
        return false;
    // Mode descriptor taken from osdev wiki https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
    if (width == 640 && height == 480 && depth == 4)
    {
        unsigned char g_640x480x16[] =
        {
        /* MISC */
            0xE3,
        /* SEQ */
            0x03, 0x01, 0x08, 0x00, 0x06,
        /* CRTC */
            0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
            0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
            0xFF,
        /* GC */
            0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
            0xFF,
        /* AC */
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
            0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
            0x01, 0x00, 0x0F, 0x00, 0x00
        };
        
        s2_VGARegistersWrite(g_640x480x16);
    }
    else
    {

        unsigned char g_320x200x256[] =
        {
            /* MISC */
                0x63,
            /* SEQ */
                0x03, 0x01, 0x0F, 0x00, 0x0E,
            /* CRTC */
                0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
                0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
                0xFF,
            /* GC */
                0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
                0xFF,
            /* AC */
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                0x41, 0x00, 0x0F, 0x00, 0x00
        };
        s2_VGARegistersWrite(g_320x200x256);
    }
    s2_VGALoc = s2_VGAGetBufferLocation();
}

void s2_VGAReprogramPallete8()
{
    s2_UInt8 pallete[] = {
        0,0,0,128,0,0,0,128,0,128,128,0,0,0,128,128,0,128,0,128,128,192,192,192,128,128,128,255,0,0,0,255,0,255,255,0,0,0,255,255,0,255,0,255,255,255,255,255,0,0,0,0,0,95,0,0,135,0,0,175,0,0,215,0,0,255,0,95,0,0,95,95,0,95,135,0,95,175,0,95,215,0,95,255,0,135,0,0,135,95,0,135,135,0,135,175,0,135,215,0,135,255,0,175,0,0,175,95,0,175,135,0,175,175,0,175,215,0,175,255,0,215,0,0,215,95,0,215,135,0,215,175,0,215,215,0,215,255,0,255,0,0,255,95,0,255,135,0,255,175,0,255,215,0,255,255,95,0,0,95,0,95,95,0,135,95,0,175,95,0,215,95,0,255,95,95,0,95,95,95,95,95,135,95,95,175,95,95,215,95,95,255,95,135,0,95,135,95,95,135,135,95,135,175,95,135,215,95,135,255,95,175,0,95,175,95,95,175,135,95,175,175,95,175,215,95,175,255,95,215,0,95,215,95,95,215,135,95,215,175,95,215,215,95,215,255,95,255,0,95,255,95,95,255,135,95,255,175,95,255,215,95,255,255,135,0,0,135,0,95,135,0,135,135,0,175,135,0,215,135,0,255,135,95,0,135,95,95,135,95,135,135,95,175,135,95,215,135,95,255,135,135,0,135,135,95,135,135,135,135,135,175,135,135,215,135,135,255,135,175,0,135,175,95,135,175,135,135,175,175,135,175,215,135,175,255,135,215,0,135,215,95,135,215,135,135,215,175,135,215,215,135,215,255,135,255,0,135,255,95,135,255,135,135,255,175,135,255,215,135,255,255,175,0,0,175,0,95,175,0,135,175,0,175,175,0,215,175,0,255,175,95,0,175,95,95,175,95,135,175,95,175,175,95,215,175,95,255,175,135,0,175,135,95,175,135,135,175,135,175,175,135,215,175,135,255,175,175,0,175,175,95,175,175,135,175,175,175,175,175,215,175,175,255,175,215,0,175,215,95,175,215,135,175,215,175,175,215,215,175,215,255,175,255,0,175,255,95,175,255,135,175,255,175,175,255,215,175,255,255,215,0,0,215,0,95,215,0,135,215,0,175,215,0,215,215,0,255,215,95,0,215,95,95,215,95,135,215,95,175,215,95,215,215,95,255,215,135,0,215,135,95,215,135,135,215,135,175,215,135,215,215,135,255,215,175,0,215,175,95,215,175,135,215,175,175,215,175,215,215,175,255,215,215,0,215,215,95,215,215,135,215,215,175,215,215,215,215,215,255,215,255,0,215,255,95,215,255,135,215,255,175,215,255,215,215,255,255,255,0,0,255,0,95,255,0,135,255,0,175,255,0,215,255,0,255,255,95,0,255,95,95,255,95,135,255,95,175,255,95,215,255,95,255,255,135,0,255,135,95,255,135,135,255,135,175,255,135,215,255,135,255,255,175,0,255,175,95,255,175,135,255,175,175,255,175,215,255,175,255,255,215,0,255,215,95,255,215,135,255,215,175,255,215,215,255,215,255,255,255,0,255,255,95,255,255,135,255,255,175,255,255,215,255,255,255,8,8,8,18,18,18,28,28,28,38,38,38,48,48,48,58,58,58,68,68,68,78,78,78,88,88,88,98,98,98,108,108,108,118,118,118,128,128,128,138,138,138,148,148,148,158,158,158,168,168,168,178,178,178,188,188,188,198,198,198,208,208,208,218,218,218,228,228,228,238,238,238
    };
 
    
    s2_OutB(0x03c8, 0);
    for (int i = 0; i < 3*256; i++)
    {
        s2_OutB(0x03c9, pallete[i]/4);
    }
}
/**
 * Optimized clear function
 */
void s2_VGAClear4I(s2_UInt8 colorIndexed)
{
    
    s2_OutB(S2_VGAPORT_SEQIP, 0x02);
    for (int i = 0; i < 9600; i++)
    {
        s2_UInt32 *loc = (s2_VGALoc)+((32*i)>>3);
        s2_OutB(S2_VGAPORT_SEQDP, (colorIndexed & 0x0F));
        *loc = 0xFFFFFFFF;
        s2_OutB(S2_VGAPORT_SEQDP, ~(colorIndexed & 0x0F));
        *loc = 0x00000000;
        if (loc > (s2_VGALoc+4*(9600-2))) return;
    }
}


void s2_VGADrawBuffer8I(s2_UInt8 *buffer)
{
    s2_OutB(S2_VGAPORT_SEQIP, 0x02);
    s2_UInt32 *ldw = (s2_UInt32*)s2_VGAGetBufferLocation();
    s2_UInt32 *bdw = (s2_UInt32*)buffer;
    for(int i = 0; i < (320*200); i++)
    {
        s2_VGALoc[i] = buffer[i];
    }
}

void s2_VGADrawBuffer4I(s2_UInt8 *buffer)
{
    // For each channel

    for (int c = 0; c < 4; c++)
    {
        // Create color mask
        s2_UInt8 colorMask = 1<<c;
        
        // Switch plane in sequencer
        s2_OutB(S2_VGAPORT_SEQIP, 0x02);
        s2_OutB(S2_VGAPORT_SEQDP, colorMask);
        // For each pixel in buffer
        for (s2_UInt32 i = 0; i < (640*480)/8; ++i)
        {

            // 8 bit plane-plotting
            s2_UInt8 bitmask = 
                (!!(buffer[i*8]   & colorMask) << 7) |
                (!!(buffer[i*8+1] & colorMask) << 6) |
                (!!(buffer[i*8+2] & colorMask) << 5) |
                (!!(buffer[i*8+3] & colorMask) << 4) |
                (!!(buffer[i*8+4] & colorMask) << 3) |
                (!!(buffer[i*8+5] & colorMask) << 2) |
                (!!(buffer[i*8+6] & colorMask) << 1) |
                (!!(buffer[i*8+7] & colorMask)     ) ;

            // When end of bitwise writing reached
            // Get location divided by amount of bits per write (8)
            s2_VGALoc[i] = bitmask;

            // Reset value
        }
    }
}

