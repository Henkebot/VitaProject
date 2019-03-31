#pragma once

#include <stdio.h>
#include <stdarg.h>

// Opens the debug file
void InitDebugFile();

// Print simple 
void PrintToFile(const char* _pMsg);

__attribute__((__format__ (__printf__,1,2)))
void PrintError(const char* _pFormat,...);

// This will be used in defines later
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)



