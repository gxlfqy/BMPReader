#ifndef __BMPDIB_H_INCLUDED__
#define __BMPDIB_H_INCLUDED__
#include <Windows.h>
#include <stdio.h>
//#include "image.h"

class GrayscaleBmpDib 
{
	friend class CBMPReaderDlg;
public:
	GrayscaleBmpDib() :m_width(0), m_height(0), m_data(nullptr){}
	GrayscaleBmpDib(const char* pszFileName);
	GrayscaleBmpDib(int wide, int height, byte * data = nullptr);
	~GrayscaleBmpDib()
	{
		if (nullptr != m_data)
		{
			delete[] m_data;
			m_data = nullptr;
		}
		m_height = m_width = 0;
	}
	void open(const char* pszFileName);
	void write(const char* pszFileName);
	void set(int width, int height, byte * data = nullptr);
	void set(byte * data);
	int height()
	{
		return m_height;
	}
	int width()
	{
		return m_height;
	}
	byte * data()
	{
		if (nullptr == m_data)
			return nullptr;
		byte * data = new byte[m_height * m_width];
		if (nullptr == data)
			return data;
		memcpy(data, m_data, sizeof(byte) * m_height * m_width);
	}
protected:
	int m_width;
	int m_height;
	byte * m_data;
};

#endif