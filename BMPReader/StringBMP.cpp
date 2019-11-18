#include "stdafx.h"
#include "StringBMP.h"

CStringBMP::CStringBMP(int w, int h, bool isReverse, byte fill, int o) throw(int)
	: m_nWidth(w)
	, m_nHeight(h)
	, m_isReverse(isReverse)
	, m_fill(fill)
	, m_o(o)
	, m_hasImage(false)
	, m_szString(L"")
	, m_imageTemp(nullptr)
{
	ASSERT(w > 0 && h > 0);
	if (w <= 0 || h <= 0)
		m_nHeight = m_nWidth = 0;
	else
	{
		InitalImage(m_nWidth, m_nHeight);
		m_imageTemp = new byte[m_nWidth*m_nHeight];
		if (nullptr == m_imageTemp)
			throw int(CStringBMP::AllocFail);
	}
}

CStringBMP::operator unsigned char*() const throw(int)
{
	if (!hasImage())
		return nullptr;
	byte * out = nullptr;
	out = new byte[m_nWidth*m_nHeight];
	if (nullptr == out)
		throw int(CStringBMP::AllocFail);

	for (int i = 0; i < m_nWidth*m_nHeight; ++i)
		out[i] = m_imageTemp[i];
	return (unsigned char*)out;
}

CStringBMP::operator CImage() const throw(int)
{
	if (!hasImage())
		throw int(IsNotBMPImage);
	return m_image;
}

void CStringBMP::reset()
{
	m_nHeight = m_nHeight = 0;
	m_hasImage = false;
	if (nullptr != m_imageTemp)
	{
		delete[] m_imageTemp;
		m_imageTemp = nullptr;
	}
	m_image.Destroy();
}

bool CStringBMP::set(int w, int h, bool isReverse /*= true*/, byte fill /*= modeFillWhite*/, int o /*= 3*/) throw(int)
{
	if (w <= 0 || h <= 0 || -1 != o && 0 > o)
		return false;

	m_hasImage = false;
	if (w != m_nWidth || h != m_nHeight)
	{
		m_nWidth = w;
		m_nHeight = h;
		if (nullptr == m_imageTemp)
		{
			delete[] m_imageTemp;
			m_imageTemp = nullptr;
		}
		m_imageTemp = new CStringBMP::byte[m_nWidth*m_nHeight];
		if (nullptr == m_imageTemp)
			throw int(CStringBMP::AllocFail);
	}
	m_isReverse = isReverse;
	m_fill = fill;
	m_o = o;
	return true;
}

bool CStringBMP::set(charstring str)
{
	m_hasImage = LoadStringToBitsMap(str, (CStringBMP::byte*)m_imageTemp, m_nWidth, m_nHeight, m_fill, m_o);
	return m_hasImage ? (m_szString = (CString)str, LoadBitsMapToImage(m_imageTemp, m_isReverse), true) : false;
}



bool CStringBMP::set(unsigned char* data) throw(int)
{
	bool bRet = false;
	CString result;
	bRet = LoadBitsMapToString(data, result, m_nWidth, m_nHeight);
	if (false == bRet)
		return false;
	m_szString = result;
	const int wide = m_nWidth;
	auto bit = [&wide](unsigned char * data, int index)->unsigned char &{ return *(data + index); };
	if (nullptr == m_imageTemp)
	{
		m_imageTemp = new byte[m_nWidth*m_nHeight];
		if (nullptr == m_imageTemp)
			throw int(CStringBMP::AllocFail);
	}
	int i;
	for (i = 0; i < m_nWidth*m_nHeight; ++i)
		bit(m_imageTemp, i) = bit(data, i);
	LoadBitsMapToImage(data, m_isReverse);
	return true;
}

bool CStringBMP::InitalImage(int width, int height)
{
	if (m_image.IsNull())
		m_image.Create(width, height, 8);
	else
	{
		if (width <= 0 || height <= 0)
			return false;
		else if (m_image.GetHeight() == width && m_image.GetWidth() == height)
			return true;
		else
		{
			m_image.Destroy();
			m_image.Create(width, height, 8);
		}
	}
	//写入调色板
	RGBQUAD ColorTable[256];
	m_image.GetColorTable(0, 256, ColorTable);
	for (int i = 0; i < 256; i++)
	{
		ColorTable[i].rgbBlue = (CStringBMP::byte)i;
		ColorTable[i].rgbGreen = (CStringBMP::byte)i;
		ColorTable[i].rgbRed = (CStringBMP::byte)i;
	}
	m_image.SetColorTable(0, 256, ColorTable);
	return true;
}

void CStringBMP::LoadBitsMapToImage(CStringBMP::byte * data, bool isReverse)
{
	if (data == nullptr)
		return;
	CStringBMP::byte *pS;
	CStringBMP::byte *pImg = (CStringBMP::byte *)m_image.GetBits();
	int step = m_image.GetPitch();
	int height = m_image.GetHeight();
	for (int i = 0; i < m_image.GetHeight(); ++i)
	{
		if (isReverse)
			pS = data + (height - 1 - i)*m_nWidth;
		else
			pS = data + i*m_nWidth;
		for (int j = 0; j < m_image.GetWidth(); ++j)
		{
			*(pImg + i*step + j) = pS[j];
		}
	}
}

bool CStringBMP::LoadStringToBitsMap(CStringBMP::charstring in, unsigned char * const out, int w, int h, CStringBMP::byte fill, int o)
{
	if (w <= 0 || h <= 0 || nullptr == out)
		return false;

	char * data = in;
	CStringBMP::byte * image_data = (CStringBMP::byte*)out;
	if (data == NULL || -1 != o && o < 0 || nullptr == out)
		return false;

	long long index = 0;
	int result;
	while (sscanf_s(data + index, "%x%*c", &result) != 0)
	{
		if (index - 1 >= w * h)
			break;
		else if (result >= 0 && result <= 255)
			*(image_data + index++) = (CStringBMP::byte)result;
		else
			break;
	}
	if (-1 != o && index < w * h - o)
		return false;
	while (index - 1 < w * h)
		*(image_data + index++) = fill;
	return true;
}

bool CStringBMP::LoadBitsMapToString(unsigned char * const in, CString &out, const int w, const int h)
{
	out = L"";
	if (w <= 0 || h <= 0)
		return false;
	CString term = L"", result = L"";
	int i;
	for (i = 0; i < w * h; ++i)
	{
		term.Format(L"%x ", *(in + i));
		result += term;
	}
	out = result;
	return true;
}

// bool CStringBMP::LoadImageToString(CImage image, CString &out, bool isReverse) throw(int)
// {
// 	//如果图像为空或参数不合法, 直接返回
// 	if (image.IsNull() || w <= 0 || h <= 0)
// 		return false;
// 	CStringBMP::byte * data = new CStringBMP::byte[w*h];
// 	if (nullptr == data)
// 		throw int(CStringBMP::AllocFail);
// 	CStringBMP::byte *pS;
// 	CStringBMP::byte *pImg = (CStringBMP::byte *)image.GetBits();
// 	int step = image.GetPitch();
// 	int height = image.GetHeight();
// 	for (int i = 0; i < image.GetHeight(); ++i)
// 	{
// 		if (isReverse)
// 			pS = data + (height - 1 - i)*m_nWidth;
// 		else
// 			pS = data + i*m_nWidth;
// 		for (int j = 0; j < m_image.GetWidth(); ++j)
// 		{
// 			*(pImg + i*step + j) = pS[j];
// 		}
// 	}
// }
