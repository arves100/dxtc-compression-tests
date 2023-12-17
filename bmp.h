#ifndef _BMP_H_
#define _BMP_H_

typedef struct s_bmp_info
{
	LONG w, h;
	WORD bpp;
} bmp_info;

extern LPDIRECTDRAWSURFACE7 LoadBitmapToDirectDraw(LPTSTR path, bmp_info& info);

#endif
