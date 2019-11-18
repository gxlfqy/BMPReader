#include "image.h"

//////////////////////////////////////////////////////////////////////////
//请不要修改这三个全局变量
const int g_xsize = IMAGEWIDE;
const int g_ysize = IMAGEHIGH;
const int g_imgsize = IMAGESIZE;
//////////////////////////////////////////////////////////////////////////
#ifdef __COMPRESS_BINARY_IMGAGE__
void __fastcall b2p(binarray _In_ in, picarray _Out_ out)
{
	int i = 0;
	for (i = 0; i < g_imgsize; ++i)
		((byte*)out)[i] = BD(in, i / IMAGEWIDE, i % IMAGEWIDE);
}

void __fastcall p2b(picarray _In_ in, binarray _Out_ out)
{
	int i = 0;
	for (i = 0; i < g_imgsize; i += 8)
	{
		((byte*)out)[i / 8] = BINARY(
			((byte*)in)[i + 0],
			((byte*)in)[i + 1],
			((byte*)in)[i + 2],
			((byte*)in)[i + 3],
			((byte*)in)[i + 4],
			((byte*)in)[i + 5],
			((byte*)in)[i + 6],
			((byte*)in)[i + 7]);
	}
}
#endif // __COMPRESS_BINARY_IMGAGE__
