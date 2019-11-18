#pragma once
#include "stdafx.h"

//字符串解析BMP类
class CStringBMP
{
protected:
	typedef unsigned char byte;

public:
	class charstring
	{
	public:
		char * m_data;
		charstring(const char * str) :m_data(const_cast<char*>(str)){}
		charstring(CString str){ USES_CONVERSION; m_data = W2A(str.GetBuffer()); }
		operator const char*(){ return m_data; }
		operator char*(){ return m_data; }
		operator CString(){ USES_CONVERSION; CString result = A2W(m_data); return result; }
		void operator=(CString data){ USES_CONVERSION; m_data = const_cast<char*>(W2A(data.GetBuffer())); }
		void operator=(const char * data){ m_data = const_cast<char*>(data); }
	};
	int m_nWidth;					//字符串处理的宽
	int m_nHeight;					//字符串处理的高
	bool m_isReverse;				//是否上下颠倒
	byte m_fill;					//补充像素值
	int m_o;						//误差阈值
	bool m_hasImage;				//是否有一张图片
	CString m_szString;				//所处理的字符串
	byte * m_imageTemp;				//由字符串处理出来的位图数据
	CImage m_image;					//处理的结果图像

protected:
	bool InitalImage(int width, int height);
	void LoadBitsMapToImage(byte* data, bool isReverse);

public:
	enum expection{
		AllocFail = 1,
		IsNotBMPImage = 2,
		UnkownException = 3
	};
	enum mode{
		modeFillAll = -1,
		modeNoFillAndCut = 0,
		modeFillBlack = 0,
		modeFillWhite = 255,
	};
	CStringBMP(int w, int h, bool isReverse = true, byte fill = CStringBMP::modeFillWhite, int o = 3) throw(int);
	~CStringBMP(){ reset(); }
	int wide() const { return m_nWidth; }
	int height() const { return m_nHeight; }
	bool isValid() const { return m_nWidth > 0 && m_nHeight > 0; }
	bool hasImage() const { return m_hasImage; }
	//return the new bitsmap in heap
	operator unsigned char*() const throw(int);
	operator CImage() const throw(int);
	bool GetImage(CImage &out){ return (out.Destroy(), hasImage()) ? (out = m_image, true) : false; }
	void reset();
	bool set(int w, int h, bool isReverse = true, byte fill = CStringBMP::modeFillWhite, int o = 3) throw(int);
	bool set(charstring str);
	bool set(unsigned char* data) throw(int);
	static bool LoadStringToBitsMap(const CStringBMP::charstring in, unsigned char * const out, int w, int h, CStringBMP::byte fill = CStringBMP::modeFillWhite, int o = 3);
	static bool LoadBitsMapToString(unsigned char * const in, CString &out, const int w, const int h);
	static bool LoadImageToString(CImage image, CString &out, bool isReverse) throw(int);

};
