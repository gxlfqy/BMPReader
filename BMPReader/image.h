#ifndef __IMAGE_H_INCLUDED__
#define __IMAGE_H_INCLUDED__

#include "chip.h"

//ͼ����Ĳ�����
#define IMAGEWIDE 152					//ͼ��Ŀ��
#define IMAGEHIGH 200					//ͼ��ĸ߶�
#define IMAGESIZE (IMAGEWIDE*IMAGEHIGH)	//ͼ������Ĵ�С
//ע�����ͼ��Ŀ�Ȳ���8�ı����Ļ�, ͼ���ֵ�����ͼ���޷�����ѹ��
#if IMAGEWIDE <= 0 || IMAGEHIGH <= 0 || IMAGEWIDE % 2 != 0
#error The size of the painting is wrong
#endif
#if IMAGEWIDE % 2 == 0
#define __ZOOMOUT_HALF__			//ͼ����Сһ��
#endif
#if IMAGEWIDE % 8 == 0
#define __COMPRESS_BINARY_IMGAGE__	//��ֵ��ͼ��ѹ����
#endif
#undef __COMPRESS_BINARY_IMGAGE__	//��ֹ��ֵ��ͼ��ѹ��

//������������
typedef unsigned char uint8;													//unsigned char
typedef uint8 byte;																//unsigned char
//////////////////////////////////////////////////////////////////////////
//ͼ��������������
typedef byte(*picarray)[IMAGEWIDE];												//���ͼ�������ָ��
#ifdef __WINDOW_TEST__
#define alloc_picarray() ((picarray)new byte[IMAGEHIGH * IMAGEWIDE])			//picarray�Ķ�����
#define free_picarray(name) if(name != NULL){delete[] name; name = NULL;}		//picarray���ͷ�
#endif // __WINDOW_TEST__
#define stack_picarry(name) byte name##[IMAGEHIGH][IMAGEWIDE]					//picarray��ջ�ķ���
typedef byte(*halfpicarray)[IMAGEWIDE / 2];										//��ų������һ���ͼ�������
#ifdef __COMPRESS_BINARY_IMGAGE__												
typedef byte(*binarray)[IMAGEWIDE / 8];											//���ѹ����ĵĶ�ֵ��ͼ�������ָ��
#ifdef __WINDOW_TEST__
#define alloc_binarray() ((binarray)new byte[IMAGEHIGH * IMAGEWIDE / 8])		//binarray�Ķ�����
#define free_binarray(name) (if(name != NULL){delete[] name; name = NULL;})		//binarray���ͷ�
#endif // __WINDOW_TEST__
#define stack_binarray(name) (byte name##[IMAGEHIGH][IMAGEWIDE / 8])			//binarray��ջ�ķ���
//��ֵ��ͼ��ѹ��, �����
#define BINARY(a, b, c, d, e, f, g, h) ((int((a) != 0) << 7) + (int((b) != 0) << 6) + (int((c) != 0) << 5) + (int((d) != 0) << 4) + (int((e) != 0) << 3) + (int((f) != 0) << 2) + (int((g) != 0) << 1) + int((h) != 0))
#define _BD(data, i) ((((1 << (7 - (i) % 8)) & (data)) != 0) * 255)
#define BD(barray, i, j) (_BD((barray)[i][(j) / 8], ((j) % 8)))
//��ֵ��ͼ��ѹ��, ���뺯��
void __fastcall b2p(binarray _In_ in, picarray _Out_ out);
void __fastcall p2b(picarray _In_ in, binarray _Out_ out);
#endif // __COMPRESS_BINARY_IMGAGE__

//ͼ����ȫ�ֲ�������
extern const int g_xsize;
extern const int g_ysize;

#endif // !__CHIP_H_INCLUDED__