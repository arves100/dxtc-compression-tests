/*
	libsquish test
*/
#include "stdafx.h"
#include "dd.h"
#include "test.h"
#include "squish.h"
#include "bmp.h"

extern LPDIRECTDRAWSURFACE7 g_bitmapSurface;
extern bmp_info	g_bitmapInfo;
extern LPDIRECTDRAWSURFACE7 g_bitmapTest1;

static BYTE* g_data = NULL;

void DeleteTest1Memory(void)
{
    if (g_data)
        delete[] g_data;
    
    g_data = NULL;
}
bool CreateTest1(int testMode)
{
    unsigned long dwFourCC;
    int d;

    switch (testMode)
    {
    case TEST_DXT1:
        dwFourCC = FOURCC_DXT1;
        d = squish::kDxt1;
        break;
    case TEST_DXT3:
        dwFourCC = FOURCC_DXT3;
        d = squish::kDxt3;
        break;
    case TEST_DXT5:
        dwFourCC = FOURCC_DXT5;
        d = squish::kDxt5;
        break;
    default:
        return false;
    }

    DDSURFACEDESC2 ddDesc2;
    ZeroMemory(&ddDesc2, sizeof(ddDesc2));
    ddDesc2.dwSize = sizeof(ddDesc2);

    auto hr = g_bitmapSurface->Lock(NULL, &ddDesc2, DDLOCK_READONLY, NULL);
    if (DDFailedCheck(hr, "Cannot lock the surface"))
    {
        return false;
    }

    auto dataSize = squish::GetStorageRequirements(ddDesc2.dwWidth, ddDesc2.dwHeight, d);
    auto data = new BYTE[dataSize];

    if (!data)
    {
        return false;
    }

    DDSURFACEDESC2 kDDSDComp;
    ZeroMemory(&kDDSDComp, sizeof(kDDSDComp));
    kDDSDComp.dwSize = sizeof(kDDSDComp);
    kDDSDComp.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_LPSURFACE | DDSD_LINEARSIZE;
    kDDSDComp.dwWidth = ddDesc2.dwWidth;
    kDDSDComp.dwHeight = ddDesc2.dwHeight;
    kDDSDComp.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
    kDDSDComp.lpSurface = data;
    kDDSDComp.dwLinearSize = dataSize;
    memset(&kDDSDComp.ddpfPixelFormat, 0, sizeof(kDDSDComp.ddpfPixelFormat));
    kDDSDComp.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    kDDSDComp.ddpfPixelFormat.dwFlags = DDPF_FOURCC;
    kDDSDComp.ddpfPixelFormat.dwFourCC = dwFourCC;

    if (ddDesc2.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS)
    {
        // alpha formats
        if (ddDesc2.ddpfPixelFormat.dwRBitMask == 0xFF000000)
            d |= squish::kRGBA;
        else if (ddDesc2.ddpfPixelFormat.dwRBitMask == 0x0000FF00)
            d |= squish::kBGRA;
        else if (ddDesc2.ddpfPixelFormat.dwRBitMask == 0x00FF0000)
            d |= squish::kARGB;
    }
    else
    {
        // no alpha formats
        if (ddDesc2.ddpfPixelFormat.dwRBitMask == 0x00FF0000)
            d |= squish::kRGB;
        else if (ddDesc2.ddpfPixelFormat.dwRBitMask == 0x000000FF)
            d |= squish::kBGR;
    }

    squish::CompressImage((const squish::u8*)ddDesc2.lpSurface, ddDesc2.dwWidth, ddDesc2.dwHeight, ddDesc2.lPitch, ddDesc2.ddpfPixelFormat.dwRGBBitCount, data, d);

#if 1
    FILE* xd = fopen("test1.dds", "wb");
    unsigned int ddsMagic = 0x20534444;
    fwrite(&ddsMagic, sizeof(ddsMagic), 1, xd);
    fwrite(&kDDSDComp, sizeof(kDDSDComp), 1, xd);
    fwrite(data, dataSize, 1, xd);
    fclose(xd);
#endif

    // Create the compressed surface of the correct size.
    hr = g_pDD7->CreateSurface(&kDDSDComp, &g_bitmapTest1, NULL);
    if (DDFailedCheck(hr, "Cannot create surface"))
    {
        g_bitmapSurface->Unlock(NULL);
        delete[] data;
        return false;
    }

    g_bitmapSurface->Unlock(NULL);
    g_data = data;

	return true;
}
