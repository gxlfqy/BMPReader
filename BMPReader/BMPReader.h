
// BMPReader.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBMPReaderApp: 
// �йش����ʵ�֣������ BMPReader.cpp
//

class CBMPReaderApp : public CWinApp
{
public:
	CBMPReaderApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBMPReaderApp theApp;