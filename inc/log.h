#include <inc/io.h>
#include <inc/va_list.h>

#define S2_LOGCOLOR_ERROR 0
#define S2_LOGCOLOR_WARN 1
#define S2_LOGCOLOR_SUCCESS 2
#define S2_LOGCOLOR_INFO 3

int s2_StrLen(char *s);

char* s2_Cat(char *s1, char *s2, char *dest);

void s2_LogSection(char *secname);

void s2_Log(char *ns, char *str, char type);

void s2_LogTest();

char* s2_Format(char *out, char *format, ...);