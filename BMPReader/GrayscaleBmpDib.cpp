#include "stdafx.h"
#include "GrayscaleBmpDib.h"

GrayscaleBmpDib::GrayscaleBmpDib(const char* pszFileName) :GrayscaleBmpDib()
{
	open(pszFileName);
}

GrayscaleBmpDib::GrayscaleBmpDib(int wide, int height, byte * data) :GrayscaleBmpDib()
{
	m_width = wide;
	m_height = height;
	m_data = new byte[m_height * m_width];
	if (nullptr == m_data)
		return;
	if (nullptr != data)
		memcpy(m_data, data, m_width * m_height);
}

void GrayscaleBmpDib::open(const char* pszFileName)
{
	FILE *fp;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	int linebyte, i;
	if (m_data != nullptr)
	{
		delete[] m_data;
		m_data = nullptr;
	}

	fopen_s(&fp, pszFileName, "rb");
	if (!fp)
		goto End;
	fread(&fileHeader, sizeof(fileHeader), 1, fp);
	fread(&infoHeader, sizeof(infoHeader), 1, fp);

	//判断文件是不是灰度图
	if (*((WORD *)"BM") != fileHeader.bfType || 8 != infoHeader.biBitCount)
		goto Failure;

	//初始化成员变量
	m_width = infoHeader.biWidth;
	m_height = infoHeader.biHeight;
	linebyte = (m_width + 3) / 4 * 4;
	m_data = new byte[m_height * m_width];
	if (nullptr == m_data)
		goto Failure;

	//读取灰度图的图像数据
	fseek(fp, sizeof(RGBQUAD) * 256, SEEK_CUR);
	if (linebyte == m_width)
		fread(m_data, sizeof(byte), m_height * m_width, fp);
	else
	{
		for (i = 0; i < m_height; ++i)
		{
			fread(m_data, sizeof(byte), m_width, fp);
			fseek(fp, linebyte - m_width, SEEK_CUR);
		}
	}
Failure:
	fclose(fp);
End:
	return;
}

void GrayscaleBmpDib::write(const char* pszFileName)
{
	FILE * fp;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	RGBQUAD rgb[256];
	int i;
	const int zero = 0;
	int linebyte = (m_width + 3) / 4 * 4;
	fileHeader.bfType = *((WORD*)"BM");
	//文件头和消息头参数
	fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + m_height * linebyte;
	fileHeader.bfReserved1 = fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
	infoHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoHeader.biHeight = m_height;
	infoHeader.biWidth = m_width;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 8;
	infoHeader.biCompression = 0;
	infoHeader.biSizeImage = linebyte * m_height;
	fopen_s(&fp, pszFileName, "wb+");
	if (!fp)
		goto End;
	fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	for (int i = 0, j = 0; i < 256; i++)
	{
		rgb[i].rgbBlue = i;
		rgb[i].rgbGreen = i;
		rgb[i].rgbRed = i;
		rgb[i].rgbReserved = 0;
	}
	fwrite(&rgb, sizeof(rgb), 1, fp);
	if (linebyte == m_width)
		fwrite(m_data, sizeof(byte), m_height * m_width, fp);
	else
	{
		for (i = 0; i < m_height; ++i)
		{
			fwrite(m_data, sizeof(byte), m_width, fp);
			fseek(fp, linebyte - m_width, SEEK_CUR);
		}
	}
	fclose(fp);
End:
	return;
}

void GrayscaleBmpDib::set(int width, int height, byte * data)
{
	if (width <= 0 || height <= 0)
		return;
	bool isSizeChanged = width != m_width || height != m_height;
	if (isSizeChanged)
	{
		m_width = width;
		m_height = height;
		if (nullptr != m_data)
		{
			delete[] m_data;
			m_data = nullptr;
		}
		m_data = new byte[m_height * m_width];
 		if (nullptr == m_data)
			return;
	}
	set(data);
}

void GrayscaleBmpDib::set(byte * data)
{
	int i;
	if (nullptr != data)
	for (i = 0; i < m_height * m_width; ++i)
		m_data[i] = data[i];
}
