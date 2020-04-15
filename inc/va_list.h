#pragma once


typedef __builtin_va_list VaList;
#define VaStart(list,last) __builtin_va_start(list, last)
#define VaEnd(list) __builtin_va_end(list)
#define VaCopy(source, dest) __builtin_va_copy(dest,source)
#define VaNext(list,type) __builtin_va_arg(list,type)