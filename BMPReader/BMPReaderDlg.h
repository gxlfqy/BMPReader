
// BMPReaderDlg.h : ͷ�ļ�
//

#pragma once
#include "EasySize.h"
#include "DragDrop.h"

//�м�ͼ��Ĵ�ŵ�ַ
#define _TEMP_IMG_PATH_ "./temp.bmp"
//Ĭ��ͼ��Ŀ��
#define _DEFAULT_IMG_WIDTH_ 152
//Ĭ��ͼ��ĸ߶�
#define _DEFAULT_IMG_HEIGHT_ 200
//�����ļ���Ӧ�Ĳ���
#define _P_PATH_ L"./Setting.ini"
#define _P_APP_NAME_ L"BMPReader"
#define _P_IMG_WIDTH_ L"WIDTH"
#define _DEFAULT_IMG_WIDTH_STR_ L"152"
#define _P_IMG_HEIGHT_ L"HEIGHT"
#define _DEFAULT_IMG_HEIGHT_STR_ L"200"
#define _P_DEFAULT_PATH_ L"DEFAULTFILEPATH"
#define _P_DEFAULT_SAVE_FILE_NAME L"DEFAULTSAVEFILENAME"
//����
#define _T_APP_NAME_	L"BMP"

// CBMPReaderDlg �Ի���
class CBMPReaderDlg : public CDialogEx
{
	DECLARE_SHORTKEY(CBMPReaderDlg)
	DECLARE_EASYSIZE
// ����
public:
	CBMPReaderDlg(CWnd* pParent = NULL);	// ��׼���캯��
	
// �Ի�������
	enum { IDD = IDD_BMPREADER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	void ShowSettingControl();

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//���ݽ��ͼ��Ĵ�С, �ԶԻ���Ĵ�С���е���
	void ResizeWindow(int width, int height)
	{
		::SetWindowPos(this->GetSafeHwnd(), HWND_TOP, 0, 0, width + 18, height + GetSystemMetrics(SM_CYSIZE) * 2 - 9, SWP_NOMOVE);
	}
	DECLARE_MESSAGE_MAP()
	CFileDialog m_dlgSaveFile;
	CFileDialog m_dlgOpenFile;
	CString m_szDefaultFilePath;	//���ļ�, �����ļ���Ĭ��Ŀ¼
	//�ַ�������ĳ��Ϳ�
	int m_nWidth;
	int m_nHeight;
	byte * m_imageTemp;				//���ַ������������λͼ����
	CString m_szCopy;				//��һ�δӼ��а��л�ȡ���ַ���
	//����Ľ��ͼ��
	CImage m_image;
	bool m_hasImage;				//�Ƿ���һ��ͼƬ
	bool m_isMousePress;			//����Ƿ�ס
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
