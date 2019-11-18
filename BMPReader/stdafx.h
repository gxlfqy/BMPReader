
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

//局部热键Map

//使用 SHORTKEYINFO_MAP 进行局部快捷键信息的初始化，在PreTranslateMessage中调用PartShortKey
//注意：只支持Ctrl、Shirt + (一个键) 的组合键。不支持 Alt 键
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

//注意是大写
#define SHORTKEY_LETTER(letter) (0x41 + letter - 'A')

//注意是主键盘区上的
#define SHORTKEY_NUMBER(num) (0x30 + num)

#define SHORTKEY_PreTranslateMessage(pMsg)				\
	{													\
		if (TRUE == PartShortKey(pMsg))					\
			return TRUE;								\
	}													\

//局部快捷键Map
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

