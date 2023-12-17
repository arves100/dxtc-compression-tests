/*
	gamebryo surface blt test
*/
#include "stdafx.h"
#include "dd.h"
#include "test.h"
#include "bmp.h"

extern LPDIRECTDRAWSURFACE7 g_bitmapSurface;
extern bmp_info	g_bitmapInfo;
extern LPDIRECTDRAWSURFACE7 g_bitmapTest3;

bool CreateTest3(int testMode)
{
    unsigned long dwFourCC;

    switch (testMode)
    {
    case TEST_DXT1:
        dwFourCC = FOURCC_DXT1;
        break;
    case TEST_DXT3:
        dwFourCC = FOURCC_DXT3;
        break;
    case TEST_DXT5:
        dwFourCC = FOURCC_DXT5;
        break;
    default:
        return false;
    }

    g_bitmapTest3 = DDCreateDXTSurface(g_bitmapInfo.w, g_bitmapInfo.h, dwFourCC);
    if (!g_bitmapTest3)
        return false;

    // *2* Compress the RGBA surface.

    if (!DDBlt(g_bitmapSurface, 0, 0, g_bitmapInfo.w, g_bitmapInfo.h, g_bitmapTest3, 0, 0, g_bitmapInfo.w, g_bitmapInfo.h))
    {
        g_bitmapTest3->Release();
        return false;
    }

    return true;
}
