#ifndef __IDT_H
#define __IDT_H

struct IDT_entry{
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};
 
struct IDT_entry IDT[256];

void IDT_init(void);


#endif

