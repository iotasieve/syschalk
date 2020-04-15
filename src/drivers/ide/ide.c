#include <inc/drivers/ide/ide.h>
s2_Byte s2_atapiPacket[12]; 
s2_UInt8 s2_IDERead(s2_IDEDevice *dev, s2_UInt16 offset)
{   
    return s2_InB(dev->channel->base + offset);
}
void s2_IDEWrite(s2_IDEDevice *dev, s2_UInt16 offset, s2_UInt16 data)
{
    
    
    s2_OutB(dev->channel->base + offset, data);
}

s2_UInt8 s2_IDEPoll(s2_IDEDevice *dev, bool acheck)
{
    for (int i = 0; i < 4; i++)
        s2_IDERead(dev, S2_ATA_REG_ALTSTATUS);

    while (s2_IDERead(dev, S2_ATA_REG_ALTSTATUS) & S2_ATA_BIT_BSY);

    if (acheck)
    {    
        s2_UInt8 state = s2_IDERead(dev, S2_ATA_REG_ALTSTATUS);
        

        if (state & S2_ATA_BIT_ERR)
            return S2_ATAERR_ERR;

        if (state & S2_ATA_BIT_DEVF)
            return S2_ATAERR_DEVF;

        if ((state & S2_ATA_BIT_DRQ) == 0)
            return S2_ATAERR_DRQ;

    }
    return S2_ATAERR_NOERR;
}

void s2_IDESleep(int ms)
{
    s2_PITSleep(ms);
}

s2_UInt16 s2_IDEPrintError(s2_IDEDevice *dev, s2_UInt16 err)
{
    if (err == 0) return 0;

    s2_TVMPrintA("=========================================");
    s2_TVMPrintA("IDE Error: ");
    if (err == S2_ATAERR_DEVF)
        s2_TVMPrintA("Device fault");
    else if (err == S2_ATAERR_ERR)
    {
        s2_TVMPrintA("ERR");
        s2_UInt8 errType = s2_IDERead(dev, S2_ATA_REG_ERROR);
        if (errType & S2_ATA_IERR_AMNF) s2_TVMPrintA("No address mark");
        if (errType & S2_ATA_IERR_TK0NF) s2_TVMPrintA("Track 0 not found");
        if (errType & S2_ATA_IERR_ABRT) s2_TVMPrintA("Command aborted");
        if (errType & S2_ATA_IERR_MCR) s2_TVMPrintA("Media change request");
        if (errType & S2_ATA_IERR_IDNF) s2_TVMPrintA("Id mark not found");
        if (errType & S2_ATA_IERR_MC) s2_TVMPrintA("Media changed");
        if (errType & S2_ATA_IERR_UNC) s2_TVMPrintA("Uncorrectable data");
        if (errType & S2_ATA_IERR_BBK) s2_TVMPrintA("Bad block");
    }
    if (err == S2_ATAERR_DRQ)
        s2_TVMPrintA("Reads nothing");

    s2_TVMPrintA("On");
    s2_TVMPrintA(dev->channel->isPrimary ? "Primary" : "Secondary");
    s2_TVMPrintA(dev->isMaster ? "Master" : "Slave");

    s2_TVMPrintA(s2_ToHex(s2_IDERead(dev, S2_ATA_REG_ALTSTATUS)));
    s2_TVMPrintA("=========================================");


    return err;
}

s2_IDEDevice* s2_IDECreateATADeviceInstance(s2_PCIDeviceDescriptor *dev, bool primary, bool master)
{
    s2_IDEDevice *res = NEW(s2_IDEDevice, 1);
    s2_IDEChannel *ch = NEW(s2_IDEChannel, 1);
    s2_UInt8 status;
    res->channel = ch;
    res->channel->isPrimary = primary;
    if (primary)
    {
        res->channel->base = ((s2_UInt32)dev->bars[0]->address) + 0x1F0 * (!((s2_UInt32)dev->bars[0]->address));
        res->channel->busMaster = (s2_UInt32)dev->bars[4]->address;
    }
    else 
    {
        res->channel->base = ((s2_UInt32)dev->bars[2]->address) + 0x170 * (!((s2_UInt32)dev->bars[2]->address));
        res->channel->busMaster = ((s2_UInt32)dev->bars[4]->address) + 8;
    }
    res->exists = true;
    res->isMaster = master;

    // Disable irqs
 //   s2_IDEWrite(res, S2_ATA_REG_CONTROL, 2);

    s2_UInt8 error;

    // Identify device
    s2_IDEWrite(res, S2_ATA_REG_HDDEVSEL, master << 4);

    s2_IDEWrite(res, S2_ATA_REG_COMMAND, 0xEC); // Identify command
    // Wait

    if (s2_IDERead(res, S2_ATA_REG_ALTSTATUS) == 0) 
    {
        res->exists = false;
        return res;
    } 

    res->type = S2_ATA_TYPE_ATA;

    while (1)
    {
        status = s2_IDERead(res, S2_ATA_REG_ALTSTATUS);
        if (!(status & S2_ATA_BIT_BSY) && (status & S2_ATA_BIT_DRQ)) break;
        if ((status & S2_ATA_BIT_ERR))
        {
            error = 1;
            break;
        }
    }
    // error = 1;
    // Error happened, must be atapi device then
    if (error != 0)
    {
        // Check if device is ATAPI

        s2_UInt8 sig1 = s2_IDERead(res, S2_ATA_REG_LBA1);
        s2_UInt8 sig2 = s2_IDERead(res, S2_ATA_REG_LBA2);
        if (sig1 == 0x14 && sig2 == 0xeb)
        {
            res->type = S2_ATA_TYPE_ATAPI;
            s2_IDEWrite(res, S2_ATA_REG_COMMAND, 0xA1); // Identify Packet command
            
            if (error = s2_IDEPoll(res, 1)) s2_IDEPrintError(res, error);
            s2_IDESleep(10);
        }
    }

    s2_UInt16 *buf = NEW(s2_UInt16, 256);
    for (int j = 0; j < 256; j++)
    {
        buf[j] = s2_InW(res->channel->base);
    }



    
    
    
    res->channel->nIEN = true;

    // TODO: Read buffer

    ////////////////////
    return res;
}

s2_UInt8 s2_IDEATAPIRead(s2_IDEDevice *dev, s2_UInt32 lba, s2_Byte *buffer, s2_UInt32 cropAt)
{
 
    
    
    

    s2_UInt8 error;

    ataLock = false;
    s2_IDEWrite(dev, S2_ATA_REG_CONTROL, 0);

    s2_atapiPacket[ 0] = 0xA8;
    s2_atapiPacket[ 1] = 0x0;
    s2_atapiPacket[ 2] = (lba >> 24) & 0xFF;
    s2_atapiPacket[ 3] = (lba >> 16) & 0xFF;
    s2_atapiPacket[ 4] = (lba >> 8) & 0xFF;
    s2_atapiPacket[ 5] = (lba >> 0) & 0xFF;
    s2_atapiPacket[ 6] = 0x0;
    s2_atapiPacket[ 7] = 0x0;
    s2_atapiPacket[ 8] = 0x0;
    s2_atapiPacket[ 9] = 1; // Number of sectors
    s2_atapiPacket[10] = 0x0;
    s2_atapiPacket[11] = 0x0;

    // s2_IDEWrite(dev, S2_ATA_REG_HDDEVSEL, dev->isMaster << 4);    for(int i = 0; i < 4; i++)
    //    s2_IDERead(dev, S2_ATA_REG_ALTSTATUS);

    s2_IDEWrite(dev, S2_ATA_REG_HDDEVSEL, (dev->isMaster) << 4);
    
    for (int i = 0; i < 4; i++)
        s2_IDERead(dev, S2_ATA_REG_ALTSTATUS);

    

    // PIO mode
    s2_IDEWrite(dev, S2_ATA_REG_FEATURES, 0);

    // Size of buffer
    s2_IDEWrite(dev, S2_ATA_REG_LBA1, S2_ATAPI_SECTOR_SIZE & 0xFF);
    s2_IDEWrite(dev, S2_ATA_REG_LBA2, (S2_ATAPI_SECTOR_SIZE >> 8));
    
    // Send packet
    
    
    s2_IDEWrite(dev, S2_ATA_REG_COMMAND, 0xA0); // Packet command

    if (error = s2_IDEPoll(dev, 1)) return error;         // Polling and return if error.

    
    s2_UInt16 *bp = (s2_UInt16*)s2_atapiPacket;    
    
    s2_OutW(dev->channel->base, bp[0]);
    s2_OutW(dev->channel->base, bp[1]);
    s2_OutW(dev->channel->base, bp[2]);
    s2_OutW(dev->channel->base, bp[3]);
    s2_OutW(dev->channel->base, bp[4]);
    s2_OutW(dev->channel->base, bp[5]);

    // Wait for irq
    while (!ataLock) {
        asm volatile("hlt");
    }

    s2_UInt32 size = (((int) s2_IDERead(dev, S2_ATA_REG_LBA2)) << 8) |
	(int) (s2_IDERead(dev, S2_ATA_REG_LBA1));

    for (int i = 0; i < size; i++)
    {
        if (i >= cropAt)
        {
            s2_InW(dev->channel->base);
        }
        else
        {
            *(((s2_UInt16*)buffer)+i) = s2_InW(dev->channel->base);
        }
    }
    
    // ataLock = false;


    // if (error = s2_IDEPoll(dev, true)) return error;
}
