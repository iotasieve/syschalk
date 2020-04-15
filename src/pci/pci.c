#include <inc/pci/pci.h>
/**
 * Read from PIC Controller
 * @param bus Bus id
 * @param device Device id
 * @param function Function id
 * @param regoffset Register offset
 * @param value Value to write
 * @returns 32 bit value from PCI table according to PCI Device Structure using register offset
 */

bool s2_supportsVGA = false;

s2_UInt16 s2_PCIReadW(s2_UInt32 bus, s2_UInt32 device, s2_UInt32 function, s2_UInt32 regoffset)
{
    s2_UInt32 object = 
        1 << 31                  | // Enable bit
        ((bus & 0xff) << 16)     | // Bus number (I suppose its a bitmask since there are 8 buses)
        ((device & 0x1f) << 11)  | // Device number
        ((function & 0x07) << 8) | // Function number
        (regoffset & 0xfc);        // Register offset

    s2_OutL(S2_PCI_COMMANDPORT, object); // Write to data port to obtain information
    s2_UInt16 res = (s2_UInt16)((s2_InL(S2_PCI_DATAPORT) >> ((regoffset & 2) * 8)) & 0xffff);
    return res;
}

/**
 * Write to PIC controller
 * @param bus Bus id
 * @param device Device id
 * @param function Function id
 * @param regoffset Register offset
 * @param value Value to write
 */
void s2_PCIWriteDW(s2_UInt32 bus, s2_UInt32 device, s2_UInt32 function, s2_UInt32 regoffset, s2_UInt32 value)
{
    s2_UInt32 object = 
        1 << 31                  | // Enable bit
        ((bus & 0xff) << 16)     | // Bus number (I suppose its a bitmask since there are 8 buses)
        ((device & 0x1f) << 11)  | // Device number
        ((function & 0x07) << 8) | // Function number
        (regoffset & 0xfc);        // Register offset
    s2_OutL(S2_PCI_COMMANDPORT, object);
    s2_OutL(S2_PCI_DATAPORT, value);
}


/**
 * Check if device has multiple functions
 * @param bus Bus id
 * @param device Device id
 */ 
bool s2_PCIIsDeviceMultifunction(s2_UInt8 bus, s2_UInt8 device)
{
    return s2_PCIReadW(bus, device, 0, 0x0e) & 0x80;
}

/**
 * Create struct from PCI table (Look up PCI table)
 * @returns Device descriptor
 * @param bus Bus id
 * @param device Device id
 * @param function Function id
*/
s2_PCIDeviceDescriptor* s2_PCIGetDeviceDescriptor(s2_UInt8 bus, s2_UInt8 device, s2_UInt8 function)
{
    s2_PCIDeviceDescriptor *res = NEW(s2_PCIDeviceDescriptor, 1);

    res->bus = bus;
    res->device = device;
    res->function = function;   

    res->vendorId = s2_PCIReadW(bus, device, function, 0);

    res->deviceId = s2_PCIReadW(bus, device, function, 2);
    res->command = s2_PCIReadW(bus, device, function, 4);
    res->status = s2_PCIReadW(bus, device, function, 6);

    // Since following parameters are 8 bit and read function returns 16 bit values we need to drop out unneeded values and shift them
    s2_UInt16 progIFRevId = s2_PCIReadW(bus, device, function, 8);
    res->progIF = (progIFRevId & 0xff00) >> 8;
    res->revId = progIFRevId & 0x00ff;

    s2_UInt16 classSubclass = s2_PCIReadW(bus, device, function, 10);
    res->classCode = (classSubclass & 0xff00) >> 8;
    res->subclass = classSubclass & 0x00ff;
    
    // Header type
    res->headerType = s2_PCIReadW(bus, device, function, 0x0e) & 0x7F;


    if (res->headerType == 0)
    {
        res->bars = NEW(s2_BaseAddressRegister*, 6);
        for (int i = 0; i < 6; i++)
        {
            res->bars[i] = s2_PCIGetBaseAddressRegister(res, i);
        }
    }
    else
    {
        res->bars = NEW(s2_BaseAddressRegister*, 2);
        for (int i = 0; i < 2; i++)
        {
            res->bars[i] = s2_PCIGetBaseAddressRegister(res, i);
        }
    }

    res->interruptLine = s2_PCIReadW(bus, device, function, 0x3c) & 0xFF;
    return res;
}

void s2_PCIDevicesScanBruteforce()
{
    s2_PCIDeviceDescriptor *dev;
    int printn = 0;
    for (int bus = 0; bus < 256; bus++)
    {
        for (int device = 0; device < 32; device++)
        {
            int functionCount = s2_PCIIsDeviceMultifunction(bus, device) ? 8 : 1;
            for (int function = 0; function < functionCount; function++)
            {
                dev = s2_PCIGetDeviceDescriptor(bus, device, function);
                if (dev->vendorId == 0xffff || dev->vendorId == 0x0000) continue;
                if (dev->classCode == 0x03 && dev->subclass == 0x00)
                {
                    s2_TVMPrint(" VGA COMPATIBLE ", 0x43, 86+(printn*80));
                    s2_supportsVGA = true;
                    return;
                }
                if (dev->classCode == 0x02 && dev->subclass == 0x00)
                {
                    s2_TVMPrint(" ETHERNET COMPATIBLE ", 0x43, 86+(printn*80));
                }
                if (dev->classCode == 0x01 && dev->subclass == 0x01)
                {
                    s2_TVMPrint(" IDE COMPATIBLE ", 0x43, 86+(printn*80));
                }
                if (dev->classCode == 0x06 && dev->subclass == 0x00)
                {
                    s2_TVMPrint(" HOST BRIDGE ", 0x43, 86+(printn*80));
                }
                if (dev->classCode == 0x06 && dev->subclass == 0x01)
                {
                    s2_TVMPrint(" ISA BRIDGE ", 0x43, 86+(printn*80));
                }
                s2_TVMPrint("BUS ", 0x70, 0+(printn*80));
                s2_TVMPrint(s2_ToHex(bus), 0x70, 4+(printn*80));
                
                s2_TVMPrint(" DEVIID ", 0x70, 12+(printn*80));
                s2_TVMPrint(s2_ToHex(dev->device), 0x70, 20+(printn*80));
 
                s2_TVMPrint(" FUNCTION ", 0x70, 28+(printn*80));
                s2_TVMPrint(s2_ToHex(function), 0x70, 38+(printn*80));
                 
                s2_TVMPrint(" VENDOR ", 0x70, 46+(printn*80));
                s2_TVMPrint(s2_ToHex(dev->vendorId), 0x70, 54+(printn*80));
 
                s2_TVMPrint(" CL/SB ", 0x70, 62+(printn*80));
                s2_TVMPrint(s2_ToHex(dev->classCode), 0x70, 69+(printn*80));
                s2_TVMPrint(s2_ToHex(dev->subclass), 0x70, (78)+(printn*80));
                s2_TVMPrint(" HTYPE ", 0x70, 102+(printn*80));
                s2_TVMPrint(s2_ToHex(dev->headerType), 0x70, 109+(printn*80));              
                printn += 2;
                s2_MemoryFree(dev);
            }   
        }
    }
}


s2_PCIDeviceDescriptor *s2_PCIScanFor(int class, int subclass, int progIF)
{
    s2_PCIDeviceDescriptor *dev;
    int printn = 0;
    for (int bus = 0; bus < 256; bus++)
    {
        for (int device = 0; device < 32; device++)
        {
            int functionCount = s2_PCIIsDeviceMultifunction(bus, device) ? 8 : 1;
            for (int function = 0; function < functionCount; function++)
            {
                dev = s2_PCIGetDeviceDescriptor(bus, device, function);
                if (dev->vendorId == 0xffff || dev->vendorId == 0x0000) continue;

                // -1 Means we ignore value of progIF
                if (dev->classCode == class && dev->subclass == subclass && (progIF == -1 ? true : (dev->progIF == progIF)))
                {
                    return dev;
                }
                s2_MemoryFree(dev);
            }   
        }
    }  

    return NULL;
}

s2_BaseAddressRegister *s2_PCIGetBaseAddressRegister(s2_PCIDeviceDescriptor *dev, s2_UInt8 barN)
{
    int maxBarN = 0;
    if (dev->headerType == 0) maxBarN = 5;
    else if (dev->headerType == 1) maxBarN = 1;

    if (barN > maxBarN) return NULL;

    s2_BaseAddressRegister *bar = NEW(s2_BaseAddressRegister, 1);
    s2_UInt32 barRaw = (((s2_UInt32)(s2_PCIReadW(dev->bus, dev->device, dev->function, (0x12 + (barN*4)))) << 16) | ((s2_UInt32)s2_PCIReadW(dev->bus, dev->device, dev->function, (0x10 + (barN*4) ) )));
    bar->type = ((barRaw & 0x01) == 1) ? S2_PCI_BARTYPE_IO : S2_PCI_BARTYPE_MEM;



    if (bar->type == S2_PCI_BARTYPE_IO)
    {
        bar->address = barRaw & (~0b11);
        bar->prefetchable = false;
        return bar;
    }
    else
    {
        bar->prefetchable = barRaw & (0b1000);
        bar->address = barRaw & (~0b1111);
        return bar;
    }

    // No support for PCI to CardBus so far
    return NULL;
}
