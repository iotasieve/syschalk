#pragma once
#include <inc/port.h>
#include <inc/drivers/ps2_keyboard.h>
#include <inc/events/events.h>
bool ataLock;
s2_UInt32 s2_msUptime;
s2_UInt32 s2_sysDelta;


void irq0_handler(void);

void irq1_handler(void);

void irq2_handler(void);

void irq3_handler(void);

void irq4_handler(void);

void irq5_handler(void);

void irq6_handler(void);

void irq7_handler(void);

void irq8_handler(void);

void irq9_handler(void);

void irq10_handler(void);

void irq11_handler(void);

void irq12_handler(void);

void irq13_handler(void);

void irq14_handler(void);

void irq15_handler(void);

