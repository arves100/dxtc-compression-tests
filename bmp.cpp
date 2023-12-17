#include "stdafx.h"
#include "bmp.h"
#include "dd.h"

/*--------------------------------------------------------------------------*/
bool load_bmp_with_gdi(LPTSTR bmp, bmp_info& out, HANDLE& obj)
{
	obj = LoadImage(NULL, bmp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (!obj)
		return false;

	BITMAP bm;
	GetObject(obj, sizeof(bm), &bm);

	out.w = bm.bmWidth;
	out.h = bm.bmHeight;
	out.bpp = bm.bmBitsPixel;
	return true;
}
/*--------------------------------------------------------------------------*/
LPDIRECTDRAWSURFACE7 make_surf_bmp(const bmp_info& info)
{
	DDSURFACEDESC2 ddsd;
	LPDIRECTDRAWSURFACE7 bmpDD = NULL;



	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
	ddsd.dwWidth = info.w;
	ddsd.dwHeight = info.h;

	// pad texture
	if ((ddsd.dwWidth % 4) != 0)
		ddsd.dwWidth += 4 - (ddsd.dwWidth % 4);
	if ((ddsd.dwHeight % 4) != 0)
		ddsd.dwHeight += 4 - (ddsd.dwHeight % 4);

	auto hr = g_pDD7->CreateSurface(&ddsd, &bmpDD, NULL);
	if (FAILED(hr))
		return NULL;

	return bmpDD;
}
/*--------------------------------------------------------------------------*/
bool gdi_bmp_to_ddr(HANDLE bmp, LONG w, LONG h, LPDIRECTDRAWSURFACE7 dd)
{
	HDC hdcsrc = CreateCompatibleDC(NULL);
	if (!hdcsrc)
		return false;

	SelectObject(hdcsrc, bmp);

	HDC hdcdst;
	auto hr = dd->GetDC(&hdcdst);
	if (FAILED(hr))
	{
		DeleteDC(hdcsrc);
		return false;
	}

	BitBlt(hdcdst, 0, 0, w, h, hdcsrc, 0, 0, SRCCOPY);
	dd->ReleaseDC(hdcdst);
	DeleteDC(hdcsrc);
	return true;
}
/*--------------------------------------------------------------------------*/
LPDIRECTDRAWSURFACE7 LoadBitmapToDirectDraw(LPTSTR path, bmp_info& info)
{
	HANDLE obj;
	LPDIRECTDRAWSURFACE7 ddr;
	if (!load_bmp_with_gdi(path, info, obj))
	{
		DeleteObject(obj);
		return false;
	}

	ddr = make_surf_bmp(info);
	if (!ddr)
	{
		DeleteObject(obj);
		return false;
	}

	if (!gdi_bmp_to_ddr(obj, info.w, info.h, ddr))
	{
		ddr->Release();
		DeleteObject(obj);
		return false;
	}

	DeleteObject(obj);
	return ddr;
}
/*--------------------------------------------------------------------------*/
