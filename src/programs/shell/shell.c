#include <inc/programs/shell/shell.h>

void s2_ShellStart(s2_ShellInstance *si)
{
    bool entr = false;
    char pkey = '\0';
    si->cursorY = -1;
    s2_ShellRenderPrompt(si);
    while (1)
    {
        si->cursorY += si->cursorX/80;
        si->cursorX -= (si->cursorX/80)*80;
        s2_Event *ev = s2_EventAwait();
        if (ev->ev_type == S2_EPS2KEYBOARD)
        {
            s2_PS2KeyboardEvent *e = (s2_PS2KeyboardEvent*)ev->event;
            if (e->pressed)
            {
                entr = !s2_ShellReadCommand(si, e->key);
            }
        }
        if (entr)
        {
            s2_ShellInterpretCommand(si);
            s2_ShellFlushInput(si);
            s2_ShellRenderPrompt(si);
            entr = false;
        }
        s2_ShellRenderCursor(si);
    }
       
}

void s2_ShellCreate(s2_ShellInstance *si)
{
    s2_CtVector *cmdv = NEW(s2_CtVector, 1);

    si->cmdv = s2_CtVectorCreate(cmdv, OF(char));
    si->color = S2_SHELLC_DEFAULT;
    si->cursorY = 0;
    si->cursorX = 0;
}

void s2_ShellTVMPrint(s2_ShellInstance *si, char* what)
{
    s2_TVMPrint(what, si->color, ((si->cursorY+1)*80)%(80*20));
    si->cursorY += 1;
}

void s2_ShellFlushInput(s2_ShellInstance *si)
{
    for (s2_Size i = 0; i < si->cmdv->size; i++)
    {
        *ITERPTR(si->cmdv->data, si->cmdv->typeSize, i) = '\0';
    }
    
    si->cmdv->size = 0;
}

bool s2_ShellReadCommand(s2_ShellInstance *si, s2_Byte key)
{
    if (key == S2_KENTER)
    {
        
        s2_TVMPrint(" ", 0x07, si->cursorX+((si->cursorY*80))%(80*25));
        return false;
    }
    if (key == S2_KBACKSPACE)
    {
        if (si->cmdv->size < 1) return true;
        s2_TVMPrint(" ", 0x07, si->cursorX+((si->cursorY*80)%(80*25)));
        si->cursorX -= 1;
        s2_CtVectorPop(si->cmdv);
    }
    else {
        char oc[2];
        oc[0] = s2_PS2ConvertCodeToChar(key);
        oc[1] = '\0';
        s2_TVMPrint(oc, si->color, si->cursorX+((si->cursorY*80)%(80*25)));
        s2_CtVectorPush(si->cmdv, s2_PS2ConvertCodeToChar(key));
        si->cursorX += 1;
    }
    return true;
}


void s2_ShellRenderCursor(s2_ShellInstance *si)
{
    if (!lolmode)
    {
        s2_TVMPrint(" ", 0x70, si->cursorX+((si->cursorY*80)%(80*25)));
    }
    else 
    {
        s2_TVMPrint("_", 0x70, si->cursorX+((si->cursorY*80)%(80*25)));
    }
}

void s2_ShellInterpretCommand(s2_ShellInstance *si)
{
    if (si->cmdv->size < 1) return;
    char cmd = *(char*)s2_CtVectorIndex(si->cmdv, 0);

    if (cmd == 'h')
    {
        s2_ShellTVMPrint(si, "v -> version");
        s2_ShellTVMPrint(si, "lol -> lolmode");
    }
    else if (cmd == 'l')
    {
        lolmode = !lolmode;
    }
    else if (cmd == 'c')
    {
        if (lolmode)
        {
            lolmode = false;
            s2_TVMClear(si->color & 0xF0);
            lolmode = true;
        }
        else 
        {
            s2_TVMClear(si->color & 0xF0);
        }
        si->cursorX = 0;
        si->cursorY = -1;
    }
    else if (cmd == 'p')
    {
        s2_Panic(S2_PANICERR_GENERATED, "very cool stuff mm yes stuff cool", true, __FILE__, s2_ToHex(__LINE__));
    }
    else if (*(char*)s2_CtVectorIndex(si->cmdv, 0) == 'v')
    {
        s2_ShellTVMPrint(si, "s2 OS version v0.0.9 | shell v0.0.1");
    }
    else 
    {
        s2_ShellTVMPrint(si, "unknown command");
    }
}

void s2_ShellRenderPrompt(s2_ShellInstance *si)
{
    s2_TVMPrint("u# ", si->color, (80*(1+si->cursorY)%(80*25)));
    si->cursorY += 1;
    si->cursorX = 3;
}

void s2_ShellDispose(s2_ShellInstance *si)
{
    s2_CtVectorDispose(si->cmdv);
    DELETE(si->cmdv);
}