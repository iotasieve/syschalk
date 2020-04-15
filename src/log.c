#include <inc/log.h>


int s2_StrLen(char *s)
{
    int i = 0;
    while (s[i])
    {
        i++;
    }
    return i;
}

char* s2_Cat(char *s1, char *s2, char *dest)
{
    int i = 0;
    while (s1[i])
    {
        dest[i] = s1[i];
        i++;
    }

    int j = 0;
    while (s2[j])
    {
        dest[i+j] = s2[j];

        j++;
    }

    dest[i+j] = 0;

    return dest;
}

char* s2_CatSZ(char *s1, char *s2, char *dest, int size)
{
    int i = 0;
    while (s1[i])
    {
        if (i > size) break;
        dest[i] = s1[i];
        i++;
    }

    int j = 0;
    while (s2[j])
    {
        if ((i + j) > size) break;
        dest[i+j] = s2[j];
        j++;
    }

    dest[i+j] = 0;
    return dest;
}

void s2_LogSection(char *secname)
{
    char out[128];

    int size = s2_StrLen(secname) + 2;
    if (size > 64)
    {
        s2_Log("Logger", "Section title too long", S2_LOGCOLOR_ERROR);
        return;
    }

    size -= 2;

    int eq = (64-size)/2;

    for (int i = 0; i < 64; i++)
    {
        out[i] = ' ';

        if ((i < eq) || (i > ((size-1)+eq)))
        {
            out[i] = '=';
            if (i == eq-1)
            {
                out[i] = ' ';
            }
            else if (i == (size-1+eq+1))
            {
                out[i] = ' ';
            }
        }
        else
        {
            out[i] = secname[i-eq];
        }
        out[i+1] = 0;
    }
    s2_QEMUDebugPrint(out);
}

void s2_Log(char *ns, char *str, char type)
{
    char out[256];
    int size = s2_StrLen(ns) + s2_StrLen(str) + 5;
    if (type == S2_LOGCOLOR_INFO)
        s2_Cat("\033[0;34m[ ", ns, out);
    if (type == S2_LOGCOLOR_ERROR)
        s2_Cat("\033[0;31m[ ", ns, out);
    if (type == S2_LOGCOLOR_WARN)
        s2_Cat("\033[0;33m[ ", ns, out);
    if (type == S2_LOGCOLOR_SUCCESS)
        s2_Cat("\033[0;32m[ ", ns, out);
    s2_Cat(out, " ] ", out);
    s2_Cat(out, str, out);
    s2_Cat(out, "\033[0m", out);


    if (size > 256)
    {
        s2_Log("Logger", "Log too big", S2_LOGCOLOR_ERROR);
        return;
    }
    s2_QEMUDebugPrint(out);
}

void s2_LogTest()
{

    char out[256];
    s2_QEMUDebugPrint(s2_Format(out, "Test, %s, %u", "string", 123));
    s2_Log("LoggerTest", "Check error log type", S2_LOGCOLOR_ERROR);
    s2_Log("LoggerTest", "Check warning log type", S2_LOGCOLOR_WARN);
    s2_Log("LoggerTest", "Check success log type", S2_LOGCOLOR_SUCCESS);
    s2_Log("LoggerTest", "Check info log type", S2_LOGCOLOR_INFO);
    s2_LogSection("End logger test");
}

int s2_CountFormat(char *format)
{
    int i = 0;
    int cnt = 0;
    while (format[i])
    {
        if (format[i] == '%' && format[i+1] != '%')
        {
            cnt += 1;
        }
        i += 1;
    }

    return cnt;
}

char* s2_ToHexNoA(unsigned int num)
{
    char res[9];
    int i = 0;
    do {
        unsigned char rem = num % 16;
        num /= 16;
        res[7-i] = "0123456789ABCDEF"[rem];
        i += 1;

    } while (i < 8);
    res[8] = 0;

    return res;
}

char* s2_ToDecNoA(unsigned int num)
{
    char res[17];
    int i = 0;
    int scnt = 0;
    do {
        unsigned char rem = num % 10;
        num /= 10;
        if (num == 0 && rem == 0 && i != 0)
        {
            res[15-i] = ' ';
            i += 1;
            scnt += 1;
            continue;
        }
        res[15-i] = "0123456789"[rem];
        i += 1;
    } while (i < 16);
    for (int i = 0; i < 16; i++)
    {
        res[i] = res[i+scnt];
        res[i+1] = 0;
    }

    return res;
}

char* s2_Format(char *out, char *format, ...)
{
    VaList vparams;
    int count = s2_CountFormat(format);
    VaStart(vparams, count);
    int i = 0;
    int j = 0;

    char *convs;
    while (format[i])
    {
        out[j+1] = 0;

        if (format[i] == '%')
        {
            if (format[i+1] == 's')
            {
                char *vp = VaNext(vparams, char*);

                s2_Cat(out, vp, out);
                j += s2_StrLen(vp);
            }
            else if (format[i+1] == 'u')
            {
                s2_UInt32 vp = VaNext(vparams, s2_UInt32);
                convs = s2_ToDecNoA(vp);
                s2_QEMUDebugPrint(convs);

                s2_Cat(out, convs, out);
                j += s2_StrLen(convs);
            }
            else if (format[i+1] == 'x' || format[i+1] == 'X')
            {
                s2_UInt32 vp = VaNext(vparams, s2_UInt32);
                convs = s2_ToHexNoA(vp);
                s2_QEMUDebugPrint(convs);

                s2_Cat(out, convs, out);
                j += s2_StrLen(convs);
            }
            i += 2;
            continue;
        }
        else
        {
            out[j] = format[i];
        }
        
        i++;
        j++;
    }
    return out;
}