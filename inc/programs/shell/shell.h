#include <inc/io.h>
#include <inc/events/events.h>
#include <inc/drivers/ps2_keyboard.h>
#include <inc/containers/vector/vector.h>
#define S2_SHELL_MAXHEIGHT 25

#define S2_SHELLC_DEFAULT 0x07

typedef struct
{
    short cursorX, cursorY;
    s2_CtVector *cmdv;
    s2_Byte color;
} s2_ShellInstance;

void s2_ShellStart(s2_ShellInstance *si);

void s2_ShellCreate(s2_ShellInstance *si);

void s2_ShellTVMPrint(s2_ShellInstance *si, char* what);

void s2_ShellFlushInput(s2_ShellInstance *si);

bool s2_ShellReadCommand(s2_ShellInstance *si, s2_Byte key);

void s2_ShellInterpretCommand(s2_ShellInstance *si);

void s2_ShellRenderPrompt(s2_ShellInstance *si);

void s2_ShellDispose(s2_ShellInstance *si);

void s2_ShellRenderCursor(s2_ShellInstance *si);