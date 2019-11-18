
// BMPReaderDlg.h : 头文件
//

#pragma once
#include "EasySize.h"
#include "DragDrop.h"

//中间图像的存放地址
#define _TEMP_IMG_PATH_ "./temp.bmp"
//默认图像的宽度
#define _DEFAULT_IMG_WIDTH_ 152
//默认图像的高度
#define _DEFAULT_IMG_HEIGHT_ 200
//配置文件对应的参数
#define _P_PATH_ L"./Setting.ini"
#define _P_APP_NAME_ L"BMPReader"
#define _P_IMG_WIDTH_ L"WIDTH"
#define _DEFAULT_IMG_WIDTH_STR_ L"152"
#define _P_IMG_HEIGHT_ L"HEIGHT"
#define _DEFAULT_IMG_HEIGHT_STR_ L"200"
#define _P_DEFAULT_PATH_ L"DEFAULTFILEPATH"
#define _P_DEFAULT_SAVE_FILE_NAME L"DEFAULTSAVEFILENAME"
//标题
#define _T_APP_NAME_	L"BMP"

// CBMPReaderDlg 对话框
class CBMPReaderDlg : public CDialogEx
{
	DECLARE_SHORTKEY(CBMPReaderDlg)
	DECLARE_EASYSIZE
// 构造
public:
	CBMPReaderDlg(CWnd* pParent = NULL);	// 标准构造函数
	
// 对话框数据
	enum { IDD = IDD_BMPREADER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	void ShowSettingControl();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//根据结果图像的大小, 对对话框的大小进行调整
	void ResizeWindow(int width, int height)
	{
		::SetWindowPos(this->GetSafeHwnd(), HWND_TOP, 0, 0, width + 18, height + GetSystemMetrics(SM_CYSIZE) * 2 - 9, SWP_NOMOVE);
	}
	DECLARE_MESSAGE_MAP()
	CFileDialog m_dlgSaveFile;
	CFileDialog m_dlgOpenFile;
	CString m_szDefaultFilePath;	//打开文件, 保存文件的默认目录
	//字符串处理的长和宽
	int m_nWidth;
	int m_nHeight;
	byte * m_imageTemp;				//由字符串处理出来的位图数据
	CString m_szCopy;				//上一次从剪切板中获取的字符串
	//处理的结果图像
	CImage m_image;
	bool m_hasImage;				//是否有一张图片
	bool m_isMousePress;			//鼠标是否按住
	//COleDropTarget m_oleTarget;

public:
	void SaveFile();
	void OpenFile(CString * szOpenFileName = nullptr);
	bool GetImageData(char * data, byte * image_data);
	virtual void OnOK();
	void LoadImageData(byte* data);
	bool InitalImageTemp();
	bool InitalImage(int width, int height);
	bool LoadImageFromClipboard();
	void GetClipboardImage();
	bool LoadImageFromTXT(CString& szInFileName);
	void SetWidthHeight(int width, int height);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	BOOL m_isOriginalDraw;
};
