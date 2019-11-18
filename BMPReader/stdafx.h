
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

//�ֲ��ȼ�Map

//ʹ�� SHORTKEYINFO_MAP ���оֲ���ݼ���Ϣ�ĳ�ʼ������PreTranslateMessage�е���PartShortKey
//ע�⣺ֻ֧��Ctrl��Shirt + (һ����) ����ϼ�����֧�� Alt ��
#define DECLARE_SHORTKEY(class)																					\
protected:																										\
	typedef void(class::*ShortKeyFunc)(void);																	\
	struct ShortKeyInfo																							\
	{																											\
		int nSysKey;																							\
		int nKey;																								\
		ShortKeyFunc pfunc;																						\
	};																											\
	ShortKeyInfo* GetShortkeysInfo()const;																		\
	BOOL PartShortKey(MSG* pMsg)																				\
	{																											\
		ShortKeyInfo * shortkeys = const_cast<ShortKeyInfo*>(GetShortkeysInfo());								\
		if (nullptr == shortkeys)																				\
			return FALSE;																						\
		ShortKeyInfo * p = shortkeys;																			\
		if (WM_KEYDOWN != pMsg->message)																		\
			return FALSE;																						\
		int i = 0;																								\
		while (NULL != p->pfunc)																				\
		{																										\
			p = shortkeys + i;																					\
			if (NULL == p->nSysKey)																				\
			{																									\
				if (pMsg->wParam == p->nKey && !GetAsyncKeyState(VK_CONTROL) && !GetAsyncKeyState(VK_SHIFT))	\
					goto Success;																				\
			}																									\
			else																								\
			{																									\
				if (pMsg->wParam == p->nKey)																	\
				{																								\
					switch (p->nSysKey)																			\
					{																							\
					case VK_CONTROL:																			\
						if (GetAsyncKeyState(VK_CONTROL) && !GetAsyncKeyState(VK_SHIFT))						\
							goto Success;																		\
					case VK_SHIFT:																				\
						if (!GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_SHIFT))						\
							goto Success;																		\
					}																							\
				}																								\
			}																									\
			++i;																								\
		}																										\
		return FALSE;																							\
	Success:																									\
		(this->*p->pfunc)();																					\
		return TRUE;																							\
	}																											\

//ע���Ǵ�д
#define SHORTKEY_LETTER(letter) (0x41 + letter - 'A')

//ע�������������ϵ�
#define SHORTKEY_NUMBER(num) (0x30 + num)

#define SHORTKEY_PreTranslateMessage(pMsg)				\
	{													\
		if (TRUE == PartShortKey(pMsg))					\
			return TRUE;								\
	}													\

//�ֲ���ݼ�Map
#define BEGIN_SHORTKEYINFO_MAP(class)					\
	class::ShortKeyInfo* class::GetShortkeysInfo()const	\
	{													\
		static const ShortKeyInfo shortKeys[] = {		\

#define ON_SHORTKEY(syskey, key, pfunc)					\
			{syskey, key, pfunc},						\

#define END_SHORTKEYINFO_MAP							\
			{ NULL, NULL, NULL }						\
		};												\
		return (ShortKeyInfo*)shortKeys;				\
	}													\

