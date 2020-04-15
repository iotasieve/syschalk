#pragma once

#include <inc/types.h>
#include <inc/port.h>
#include <inc/io.h>
#include <inc/pci/pci.h>
#include <inc/irq_handlers.h>

// Big credit to https://wiki.osdev.org/PCI_IDE_Controller

#define S2_ATA_BIT_BSY     0x80    // Busy
#define S2_ATA_BIT_DRDY    0x40    // Drive ready
#define S2_ATA_BIT_DEVF    0x20    // Drive write fault
#define S2_ATA_BIT_DSC     0x10    // Drive seek complete
#define S2_ATA_BIT_DRQ     0x08    // Data request ready
#define S2_ATA_BIT_CORR    0x04    // Corrected data
#define S2_ATA_BIT_IDX     0x02    // Index
#define S2_ATA_BIT_ERR     0x01    // Error

#define S2_ATA_REG_DATA       0x00
#define S2_ATA_REG_ERROR      0x01
#define S2_ATA_REG_FEATURES   0x01
#define S2_ATA_REG_SECCOUNT0  0x02
#define S2_ATA_REG_LBA0       0x03
#define S2_ATA_REG_LBA1       0x04
#define S2_ATA_REG_LBA2       0x05
#define S2_ATA_REG_HDDEVSEL   0x06
#define S2_ATA_REG_COMMAND    0x07
#define S2_ATA_REG_STATUS     0x07
#define S2_ATA_REG_CONTROL    0x206
#define S2_ATA_REG_ALTSTATUS  0x206

#define S2_ATAERR_NOERR 0
#define S2_ATAERR_ERR 1
#define S2_ATAERR_DEVF 2
#define S2_ATAERR_DRQ 3

#define S2_ATA_IERR_BBK      0x80    // Bad block
#define S2_ATA_IERR_UNC      0x40    // Uncorrectable data
#define S2_ATA_IERR_MC       0x20    // Media changed
#define S2_ATA_IERR_IDNF     0x10    // ID mark not found
#define S2_ATA_IERR_MCR      0x08    // Media change request
#define S2_ATA_IERR_ABRT     0x04    // Command aborted
#define S2_ATA_IERR_TK0NF    0x02    // Track 0 not found
#define S2_ATA_IERR_AMNF     0x01    // No address mark

#define S2_ATA_TYPE_ATAPI 0
#define S2_ATA_TYPE_ATA 1

#define S2_ATAPI_SECTOR_SIZE 2048

typedef struct
{
    bool isPrimary;
    s2_UInt16 base;
    s2_UInt16 busMaster;
    s2_UInt8 nIEN;
} s2_IDEChannel;

typedef struct
{
    s2_UInt8 type;
    s2_IDEChannel *channel;
    bool exists;
    bool isMaster;
    s2_UInt16 signature;
    s2_UInt16 features;
    s2_UInt32 commands;
    s2_UInt32 capacity;
    s2_Byte model[41];
} s2_IDEDevice;


s2_UInt8 s2_IDERead(s2_IDEDevice *dev, s2_UInt16 offset);

void s2_IDEWrite(s2_IDEDevice *dev, s2_UInt16 offset, s2_UInt16 data);

s2_UInt8 s2_IDEPoll(s2_IDEDevice *dev, bool acheck);

s2_UInt16 s2_IDEPrintError(s2_IDEDevice *dev, s2_UInt16 err);

s2_IDEDevice* s2_IDECreateATADeviceInstance(s2_PCIDeviceDescriptor *dev, bool primary, bool master);

s2_UInt8 s2_IDEATAPIRead(s2_IDEDevice *dev, s2_UInt32 lba, s2_Byte *buffer, s2_UInt32 cropAt);
