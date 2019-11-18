#pragma once
#include "afxdialogex.h"
//#include "EasyStaticCtrlConnect.h"
#include <vector>

class CMyDialogEx :
	public CDialogEx
{
public:
	struct CtrlInfo
	{
		int staticID;
		int ctrlID;
		int CtrlName;
		LONG attribute;
		LONG ExtraData;
	};
	struct SCtrlTDPosition
	{ 
		int id; 
		double x;
		double y;
	};
public:
	void GetWindowWH(POINT & WH){
		CRect rect;
		GetClientRect(&rect);
		WH.x = rect.right - rect.left;
		WH.y = rect.bottom - rect.top;
	}
	CMyDialogEx();
	CMyDialogEx(UINT nIDTemplate, CWnd *pParent = NULL);
	CMyDialogEx(LPCTSTR lpszTemplateName, CWnd *pParentWnd = NULL);
	~CMyDialogEx();
	void OnlyAgreeNo(int nID);
	void OnlyAgreeDecimal(int nID);
	void BanInputMethod(int nID, bool o = true);
	void DecimalCompletion(int nID);
	void PaintGreenBackground();
	//void DrawBorder(CDC *pDC, COLORREF color);
	void DrawBorder(UINT message, COLORREF color);
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void TopMost() {
		::SetWindowPos(this->GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	void NoTopMost() {
		::SetWindowPos(this->GetSafeHwnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	void CheckerboardLayout(const int ExID[] = nullptr, BOOL isAll = TRUE, int = 0);
	void CheckerboardLayout(std::vector<SCtrlTDPosition> & CtrlTDPositionList);
	void RegionLayout(POINT oldWH, std::vector<SCtrlTDPosition> & CtrlTDPositionList);
	std::vector<CMyDialogEx::SCtrlTDPosition> GetCtrlTDPosition(const int ExID[] = nullptr, BOOL isAll = TRUE);
	void OnCtrlInfo(WPARAM wParam, LPARAM lParam);
	CtrlInfo* GetCtrlInfo();
	virtual BOOL OnInitDialog();
};

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

#define BEGIN_CTRLINFO_MAP(class)								\
	class::CtrlInfo * class::GetCtrlsInfo()const				\
	{															\
		static const CtrlInfo ctrlsInfo[] = {					\

#define ON_CTRLINFO(staticID, ctrlID, ctrlName, attribute)		\
			{staticID, ctrlID, ctrlName, attribute},

#define END_CTRLINFO_MAP				\
			{NULL, NULL, NULL, NULL}	\
		};								\
		return (CtrlInfo*)ctrlsInfo;	\
	}									\

#define EDITINPUTNO				0x0000000000000100L
#define EDITINPUTDECIMAL		0x0000000000000200L
#define EDITINPUTNUMBER			0x0000000000000400L
#define EDITLIMITTEXT16			0x0000000000010000L
#define EDITLIMITTEXT64			0x0000000000040000L
#define EDITLIMITTEXT255		0x00000000000FF000L

//最大值是255
#define EDITLIMITTEXT(n)		((n % 256) << 3)
