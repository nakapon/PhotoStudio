#pragma once

// HDC
typedef void *HDC;

// RECT
typedef struct tagRECT
{
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;

// POINT
typedef struct tagPOINT
{
	INT x;
	INT y;
} POINT, *PPOINT;

// SIZE
typedef struct tagSIZE
{
	INT cx;
	INT cy;
} SIZE, *PSIZE;

// COLOR
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define PALETTERGB(r,g,b)   (0x02000000 | RGB(r,g,b))
#define PALETTEINDEX(i)     ((COLORREF)(0x01000000 | (DWORD)(WORD)(i)))

#define GetRValue(rgb)      (0xFF & (rgb))
#define GetGValue(rgb)      (0xFF & ((rgb) >> 8))
#define GetBValue(rgb)      (0xFF & ((rgb) >> 16))
