/*
	st3c (lithtech) surface test
*/
#include "stdafx.h"
#include "dd.h"
#include "test.h"
#include "s3tc.h"
#include "bmp.h"

extern LPDIRECTDRAWSURFACE7 g_bitmapSurface;
extern bmp_info	g_bitmapInfo;
extern LPDIRECTDRAWSURFACE7 g_bitmapTest2;

static BYTE* g_data = NULL;

void DeleteTest2Memory(void)
{
    if (g_data)
        delete[] g_data;

    g_data = NULL;
}

bool CreateTest2(int testMode)
{
    unsigned long dwFourCC;
    unsigned int encodeType;

    S3TCsetAlphaReference(0);

    switch (testMode)
    {
    case TEST_DXT1:
        encodeType = S3TC_ENCODE_RGB_FULL;
        dwFourCC = FOURCC_DXT1;
        break;
    case TEST_DXT3:
        encodeType = S3TC_ENCODE_RGB_FULL | S3TC_ENCODE_ALPHA_EXPLICIT;
        dwFourCC = FOURCC_DXT3;
        break;
    case TEST_DXT5:
        encodeType = S3TC_ENCODE_RGB_FULL | S3TC_ENCODE_ALPHA_INTERPOLATED;
        dwFourCC = FOURCC_DXT5;
        break;
    default:
        return false;
    }

    DDSURFACEDESC2 ddDesc;
    DDSURFACEDESC2 ddDescDst;

    ZeroMemory(&ddDescDst, sizeof(ddDescDst));
    ZeroMemory(&ddDesc, sizeof(ddDesc));

    // fill up ddsurface data
    ddDesc.dwSize = sizeof(ddDesc);
    ddDescDst.dwSize = sizeof(ddDescDst);


    // lock the surface to get pixels and info

    auto hr = g_bitmapSurface->Lock(NULL, &ddDesc, DDLOCK_READONLY, NULL);
    if (DDFailedCheck(hr, "Cannot lock the surface"))
    {
        return false;
    }

    // gets the size of the compression
    auto dataSize = S3TCgetEncodeSize((DDSURFACEDESC*) & ddDesc, encodeType);

    // allocate memory
    auto data = new BYTE[dataSize];

    if (!data)
    {
        g_bitmapSurface->Unlock(NULL);
        return false;
    }

    float weight[3];
    weight[0] = 0.3086f;
    weight[1] = 0.6094f;
    weight[2] = 0.0820f;

    // encode texture

    S3TCencode((DDSURFACEDESC*)&ddDesc, NULL, (DDSURFACEDESC*)&ddDescDst, data, encodeType, weight);

    // create texture based from info
    ddDescDst.dwFlags |= DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_PIXELFORMAT | DDSD_LPSURFACE | DDSD_LINEARSIZE;
    ddDescDst.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY | DDSCAPS_TEXTURE;
    ddDescDst.ddpfPixelFormat.dwFourCC = dwFourCC;
    ddDescDst.dwLinearSize = dataSize;

#if 1
    FILE* xd = fopen("test2.dds", "wb");
    unsigned int ddsMagic = 0x20534444;
    fwrite(&ddsMagic, sizeof(ddsMagic), 1, xd);
    fwrite(&ddDescDst, sizeof(ddDescDst), 1, xd);
    fwrite(data, dataSize, 1, xd);
    fclose(xd);
#endif

    hr = g_pDD7->CreateSurface(&ddDescDst, &g_bitmapTest2, NULL);
    if (DDFailedCheck(hr, "Cannot create dest surface"))
    {
        delete[] data;
        g_bitmapSurface->Unlock(NULL);
        return false;
    }

    g_data = data;
    g_bitmapSurface->Unlock(NULL);

	return true;
}
