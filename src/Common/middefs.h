#ifndef INC_117_COMMON_MIDDEFS_H
#define INC_117_COMMON_MIDDEFS_H

#pragma warning(disable : 4244)		// int or float down-conversion
//#pragma warning(disable : 4305)		// int or float data truncation
//#pragma warning(disable : 4201)		// nameless struct/union
#pragma warning(disable : 4514)		// unreferenced inline function removed
#pragma warning(disable : 4100)		// unreferenced formal parameter
#pragma warning(disable : 4103)		// #pragma pack
#pragma warning(disable : 4996)		// deprecation
#define _CRT_SECURE_NO_DEPRECATE

#ifdef _WINDOWS
#include "windefs.h"
#endif

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <math.h>

#include "lowdefs.h"
#include "my_mem.h"

#define DOHUYA 1024
#define MAX_LOADSTRING 100

#define V_PI 3.1415926f

typedef struct  {
	v_uint32 magic;
	v_uint32 version;
} info_preheader_t;

#endif