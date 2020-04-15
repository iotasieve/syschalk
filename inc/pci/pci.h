#pragma once

#include <inc/types.h>
#include <inc/io.h>
#include <inc/port.h>
#include <inc/pci/bar.h>
#include <inc/mem/manager.h>
#define S2_PCI_DATAPORT 0xCFC
#define S2_PCI_COMMANDPORT 0xCF8

typedef struct {
    s2_UInt16 deviceId;
    s2_UInt16 vendorId;
    s2_UInt16 status;
    s2_UInt16 command;
    s2_UInt8 classCode;
    s2_UInt8 subclass;
    s2_UInt8 progIF;
    s2_UInt8 revId;
    s2_UInt8 headerType;
    s2_UInt8 interruptLine;

    s2_BaseAddressRegister **bars;

    s2_UInt8 bus;
    s2_UInt8 device;
    s2_UInt8 function;
} s2_PCIDeviceDescriptor;

extern bool s2_supportsVGA;

/**
 * Read from PIC Controller
 * @param bus Bus id
 * @param device Device id
 * @param function Function id
 * @param regoffset Register offset
 * @param value Value to write
 * @returns 32 bit value from PCI table according to PCI Device Structure using register offset
 */
s2_UInt16 s2_PCIReadW(s2_UInt32 bus, s2_UInt32 device, s2_UInt32 function, s2_UInt32 regoffset);

/**
 * Write to PIC controller
 * @param bus Bus id
 * @param device Device id
 * @param fu
￼
chibill_SCSPOToday at 1:00 PM
Like I said I have had bad experiences with Rustnction Function id
 * @param regoffset Register offset
 * @param value Value to write
 */
void s2_PCIWriteDW(s2_UInt32 bus, s2_UInt32 device, s2_UInt32 function, s2_UInt32 regoffset, s2_UInt32 value);

/**
 * Create struct from PCI table (Look up PCI table)
 * @returns Device descriptor
 * @param bus Bus id
 * @param device Device id
 * @param function Function id
*/
s2_PCIDeviceDescriptor* s2_PCIGetDeviceDescriptor(s2_UInt8 bus, s2_UInt8 device, s2_UInt8 function);

/**
 * Check if device has multiple functions
 * @param bus Bus id
 * @param device Device id
 */ 
bool s2_PCIIsDeviceMultifunction(s2_UInt8 bus, s2_UInt8 device);


void s2_PCIDevicesScanBruteforce();

s2_PCIDeviceDescriptor* s2_PCIScanFor(int class, int subclass, int progIF);

s2_BaseAddressRegister* s2_PCIGetBaseAddressRegister(s2_PCIDeviceDescriptor *dev, s2_UInt8 barN);
