#ifndef __IMAGE_H_INCLUDED__
#define __IMAGE_H_INCLUDED__

#include "chip.h"

//图像处理的参数宏
#define IMAGEWIDE 152					//图像的宽度
#define IMAGEHIGH 200					//图像的高度
#define IMAGESIZE (IMAGEWIDE*IMAGEHIGH)	//图像总体的大小
//注意如果图像的宽度不是8的倍数的话, 图像二值化后的图像无法进行压缩
#if IMAGEWIDE <= 0 || IMAGEHIGH <= 0 || IMAGEWIDE % 2 != 0
#error The size of the painting is wrong
#endif
#if IMAGEWIDE % 2 == 0
#define __ZOOMOUT_HALF__			//图像缩小一半
#endif
#if IMAGEWIDE % 8 == 0
#define __COMPRESS_BINARY_IMGAGE__	//二值化图像压缩宏
#endif
#undef __COMPRESS_BINARY_IMGAGE__	//禁止二值化图像压缩

//基本类型声明
typedef unsigned char uint8;													//unsigned char
typedef uint8 byte;																//unsigned char
//////////////////////////////////////////////////////////////////////////
//图像数组类型声明
typedef byte(*picarray)[IMAGEWIDE];												//存放图像的数组指针
#ifdef __WINDOW_TEST__
#define alloc_picarray() ((picarray)new byte[IMAGEHIGH * IMAGEWIDE])			//picarray的堆申请
#define free_picarray(name) if(name != NULL){delete[] name; name = NULL;}		//picarray的释放
#endif // __WINDOW_TEST__
#define stack_picarry(name) byte name##[IMAGEHIGH][IMAGEWIDE]					//picarray的栈的分配
typedef byte(*halfpicarray)[IMAGEWIDE / 2];										//存放长宽减少一半的图像的数组
#ifdef __COMPRESS_BINARY_IMGAGE__												
typedef byte(*binarray)[IMAGEWIDE / 8];											//存放压缩后的的二值化图像的数组指针
#ifdef __WINDOW_TEST__
#define alloc_binarray() ((binarray)new byte[IMAGEHIGH * IMAGEWIDE / 8])		//binarray的堆申请
#define free_binarray(name) (if(name != NULL){delete[] name; name = NULL;})		//binarray的释放
#endif // __WINDOW_TEST__
#define stack_binarray(name) (byte name##[IMAGEHIGH][IMAGEWIDE / 8])			//binarray的栈的分配
//二值化图像压缩, 解码宏
#define BINARY(a, b, c, d, e, f, g, h) ((int((a) != 0) << 7) + (int((b) != 0) << 6) + (int((c) != 0) << 5) + (int((d) != 0) << 4) + (int((e) != 0) << 3) + (int((f) != 0) << 2) + (int((g) != 0) << 1) + int((h) != 0))
#define _BD(data, i) ((((1 << (7 - (i) % 8)) & (data)) != 0) * 255)
#define BD(barray, i, j) (_BD((barray)[i][(j) / 8], ((j) % 8)))
//二值化图像压缩, 解码函数
void __fastcall b2p(binarray _In_ in, picarray _Out_ out);
void __fastcall p2b(picarray _In_ in, binarray _Out_ out);
#endif // __COMPRESS_BINARY_IMGAGE__

//图像处理全局参数声明
extern const int g_xsize;
extern const int g_ysize;

#endif // !__CHIP_H_INCLUDED__