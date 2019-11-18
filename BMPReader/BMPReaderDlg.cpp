// BMPReaderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BMPReader.h"
#include "BMPReaderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CString g_szCaption = L"BMP";

// CBMPReaderDlg �Ի���

CBMPReaderDlg::CBMPReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBMPReaderDlg::IDD, pParent)
	, m_nWidth(_DEFAULT_IMG_WIDTH_)
	, m_nHeight(_DEFAULT_IMG_HEIGHT_)
	, m_hasImage(false)
	, m_imageTemp(nullptr)
	, m_dlgSaveFile(FALSE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.bmp)|*.bmp||"), this)
	, m_dlgOpenFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.txt)|*.txt|Describe Files (*.bmp)|*.bmp||"), this)
	, m_isMousePress(false)
	, m_isOriginalDraw(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CBMPReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 1, 1000);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
	DDV_MinMaxInt(pDX, m_nHeight, 1, 1000);
	DDX_Check(pDX, IDC_CHECK1, m_isOriginalDraw);
}

BEGIN_MESSAGE_MAP(CBMPReaderDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DROPFILES()
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//��ݼ�Map
BEGIN_SHORTKEYINFO_MAP(CBMPReaderDlg)
	ON_SHORTKEY(VK_CONTROL, SHORTKEY_LETTER('S'), &CBMPReaderDlg::SaveFile)
	ON_SHORTKEY(VK_CONTROL, SHORTKEY_LETTER('V'), &CBMPReaderDlg::GetClipboardImage)
END_SHORTKEYINFO_MAP

BEGIN_EASYSIZE_MAP(CBMPReaderDlg)
	EASYSIZE(IDC_WIDTH_STATIC, ES_BORDER, ES_KEEPSIZE, ES_KEEPSIZE, ES_BORDER, NULL)
	EASYSIZE(IDC_WIDTH, ES_BORDER, ES_KEEPSIZE, ES_KEEPSIZE, ES_BORDER, NULL)
	EASYSIZE(IDC_HEIGHT_STATIC, ES_BORDER, ES_KEEPSIZE, ES_KEEPSIZE, ES_BORDER, NULL)
	EASYSIZE(IDC_HEIGHT, ES_BORDER, ES_KEEPSIZE, ES_KEEPSIZE, ES_BORDER, NULL)
	EASYSIZE(IDC_CHECK1, ES_BORDER, ES_KEEPSIZE, ES_KEEPSIZE, ES_BORDER, NULL)
	EASYSIZE(IDC_FILE_NAME, ES_BORDER, ES_KEEPSIZE, ES_KEEPSIZE, ES_BORDER, NULL)
	EASYSIZE(IDC_PIC1, ES_BORDER, ES_BORDER, ES_BORDER, ES_BORDER, NULL)
END_EASYSIZE_MAP

// CBMPReaderDlg ��Ϣ�������

BOOL CBMPReaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	CString szWidth, szHeight, szCaption;		//�������ļ��л�ȡ���Ĳ���
	::GetPrivateProfileString(_P_APP_NAME_, _P_IMG_WIDTH_, _DEFAULT_IMG_WIDTH_STR_, szWidth.GetBuffer(MAX_PATH), MAX_PATH, _P_PATH_);
	::GetPrivateProfileString(_P_APP_NAME_, _P_IMG_HEIGHT_, _DEFAULT_IMG_HEIGHT_STR_, szHeight.GetBuffer(MAX_PATH), MAX_PATH, _P_PATH_);
	::GetPrivateProfileString(_P_APP_NAME_, _P_DEFAULT_PATH_, _T(""), m_szDefaultFilePath.GetBuffer(MAX_PATH), MAX_PATH, _P_PATH_);
	//�����ȡ�������ļ��е�Ĭ��Ŀ¼����, ��������Ϊ���ļ��ͱ����ļ����ڵ�Ĭ��Ŀ¼
	if (m_szDefaultFilePath != "")
	{
		m_dlgSaveFile.m_ofn.lpstrInitialDir = m_szDefaultFilePath;
		m_dlgOpenFile.m_ofn.lpstrInitialDir = m_szDefaultFilePath;
	}
	//���������еĶ�Ӧ����
	INIT_EASYSIZE;					//�ؼ�λ�ó�ʼ��
	//���ÿؼ�IDC_WIDTH IDC_HEIGHT�е����Ĳ���
	GetDlgItem(IDC_WIDTH)->SetWindowText(szWidth);
	GetDlgItem(IDC_HEIGHT)->SetWindowText(szHeight);
	UpdateData(TRUE);			//ͬ����Ա���� m_nWidth m_nHeight
	//��ʼ��m_image
	InitalImage(m_nWidth, m_nHeight);
	InitalImageTemp();
	//�������ڴ�С, ʹ����ͼƬ�ĳ�����Ӧ
	ResizeWindow(m_image.GetWidth(), m_image.GetHeight());
	szCaption.Format(L"%s(%s*%s)", g_szCaption, szWidth, szHeight);
	SetWindowText(szCaption);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBMPReaderDlg::OnPaint()
{
	//���ƶԻ����ͼ��
	int cx, cy;
	CRect	rect;
	CWnd *pWnd = NULL;
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	//���ƶԻ����е�IDC_PIC1ͼ��
	else
	{
		//ȷ�϶Ի����������Ƿ���һ��������ͼ��
		if (m_hasImage && !m_image.IsNull())
		{
			//��ȡͼƬ�Ŀ� �߶�
			cx = m_image.GetWidth();
			cy = m_image.GetHeight();
			//ResizeWindow(cx, cy);
			//��ȡIDC_PIC1�Ĵ���ָ��
			pWnd = GetDlgItem(IDC_PIC1);
			//��ȡPicture Control�ؼ��Ĵ�С
			pWnd->GetWindowRect(&rect);
			//���ͻ���ѡ�е��ؼ���ʾ�ľ���������
			ScreenToClient(&rect);
			//�����ƶ����ؼ���ʾ������
			pWnd->MoveWindow(rect.left, rect.top, cx, cy, TRUE);
			pWnd->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С
			CDC *pDc = NULL;
			pDc = pWnd->GetDC();//��ȡpicture��DC
			m_image.Draw(pDc->m_hDC, rect);//��ͼƬ���Ƶ�picture��ʾ��������
			ReleaseDC(pDc);
		}
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBMPReaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBMPReaderDlg::ShowSettingControl()
{
	static const int SettingControlIDs[] = {
		IDC_WIDTH_STATIC, IDC_WIDTH,
		IDC_HEIGHT_STATIC, IDC_HEIGHT,
		IDC_CHECK1, IDC_FILE_NAME
	};
	static int nWidth, nHeight;			//��һ��������ĳ���
	int w, h;							//��ǰ����ĳ���
	CString szWidth, szHeight, szCaption;
	//��ȡ�༭��ǰ״̬
	BOOL isShow = GetDlgItem(IDC_WIDTH)->IsWindowVisible();
	//����༭���Ǵ�����ʾ״̬, ��ȡ�༭���еĲ���
	if (isShow)
	{
		//��ȡ�༭�������������
		UpdateData(TRUE);
		GetDlgItemText(IDC_WIDTH, szWidth);
		GetDlgItemText(IDC_HEIGHT, szHeight);
		swscanf_s(szWidth, L"%d", &w);
		swscanf_s(szHeight, L"%d", &h);
		//������������ı�, ��ı䴦�����λͼ������, �����ñ�־
		if (nWidth != w || nHeight != h)
		{
			//ͨ�� m_nWidth �� m_nHeight ������ʼ��λͼ������
			m_nWidth = w;
			m_nHeight = h;
			InitalImageTemp();
			m_hasImage = false;
			//���ݴ���ĳ���ı�Ի����С, ע�Ⲣ��ɾ�����ͼ��
			ResizeWindow(m_nWidth, m_nHeight);
			//�ı�Ի���ı���
			szCaption.Format(L"%s(%s*%s)", g_szCaption, szWidth, szHeight);
			SetWindowText(szCaption);
			//�洢������Ϣ
			WritePrivateProfileString(_P_APP_NAME_, _P_IMG_WIDTH_, szWidth, _P_PATH_);
			WritePrivateProfileString(_P_APP_NAME_, _P_IMG_HEIGHT_, szHeight, _P_PATH_);
		}
		else{}
	}
	//�����л��༭���״̬(��ʾ, ����)
	for (int id : SettingControlIDs)
	{
		if (isShow)
			GetDlgItem(id)->ShowWindow(SW_HIDE);
		else
			GetDlgItem(id)->ShowWindow(SW_SHOW);
	}
	//����༭���Ǵ�������״̬, ������ƶ���IDC_WIDTH�༭����, ��ѡ��ȫ���ַ���
	if (!isShow)
	{
		CString strInfo;
		((CEdit*)GetDlgItem(IDC_WIDTH))->GetWindowText(strInfo);
		((CEdit*)GetDlgItem(IDC_WIDTH))->SetSel(0, strInfo.GetLength(), FALSE);
		GetDlgItem(IDC_WIDTH)->SetFocus();
	}
	//��¼��������ĳ���
	nWidth = m_nWidth;
	nHeight = m_nHeight;
}

BOOL CBMPReaderDlg::PreTranslateMessage(MSG* pMsg)
{
	PartShortKey(pMsg);
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CBMPReaderDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	UPDATE_EASYSIZE;
	// TODO:  �ڴ˴������Ϣ����������
}

void CBMPReaderDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	static int nCounter = 0;
	static int width = 0, height = 0;
	if (nCounter == 0)
	{
		width = pRect->right - pRect->left;
		height = pRect->bottom - pRect->top;
		nCounter++;
	}
	EASYSIZE_MINSIZE(width, height, fwSide, pRect);
	// TODO:  �ڴ˴������Ϣ����������
}

void CBMPReaderDlg::OpenFile(CString * szOpenFileName /*= nullptr*/)
{
	CString strFile;					//�ļ���ȫ��(��ַ)
	CString strDir;						//�ļ����ڵ�Ŀ¼
	CString strFileName;				//�ļ����ļ���
	CString strFileExt;					//�ļ��ĺ�׺��
	//�򿪶Ի���ģʽ
	if (szOpenFileName == nullptr)
	{
		if (m_dlgOpenFile.DoModal() != IDOK)
			return;
		//��ȡ�ļ���ȫ��(��ַ), �ļ����ļ���, �ļ��ĺ�׺��
		strFile = m_dlgOpenFile.GetPathName();
		strFileName = m_dlgOpenFile.GetFileName();
		strFileExt = L"." + m_dlgOpenFile.GetFileExt();
		//��¼�Ի���򿪵��ļ����ڵ�Ŀ¼, ��������ΪĬ��Ŀ¼
		//��ȡĿ¼��
		strDir = strFile;
		strDir.TrimRight(strFileName);
		if (strDir != m_szDefaultFilePath)
		{
			m_szDefaultFilePath = strDir;
			m_dlgOpenFile.m_ofn.lpstrInitialDir = m_szDefaultFilePath;
			m_dlgSaveFile.m_ofn.lpstrInitialDir = m_szDefaultFilePath;
			WritePrivateProfileString(_P_APP_NAME_, _P_DEFAULT_PATH_, strDir, _P_PATH_);
		}
		else{}
	}
	else
	{
		strFileExt = szOpenFileName->Right(4);
		strFile = *szOpenFileName;
	}

	if (strFileExt == ".bmp")
	{
		//��ȡbmp�ļ�
		m_image.Destroy();
		m_image.Load(strFile);
		m_hasImage = true;
	}
	else if (strFileExt == ".txt")
	{
		//���ñ�־
		m_hasImage = LoadImageFromTXT(strFile);
	}
	else
		MessageBox(L"��ѡ���ļ�����.bmp��.txt�ļ�", L"��ʾ");
	if (!m_image.IsNull())
		ResizeWindow(m_image.GetWidth(), m_image.GetHeight());
	//����ͼ��
	OnPaint();
	m_szCopy = "";
}

void CBMPReaderDlg::SaveFile()
{
	//�ж��Ƿ���ڽ��ͼ��
	if (!m_hasImage)
		return;

	if (m_dlgSaveFile.DoModal() != IDOK)
		return;

	CString strFile;				//�ļ�ȫ��
	CString strDir;					//�ļ����ڵ�Ŀ¼
	CString strFileName;			//�ļ����ļ���
	strFile = m_dlgSaveFile.GetPathName();
	strFileName = m_dlgSaveFile.GetFileName();
	//��¼�Ի���򿪵��ļ����ڵ�Ŀ¼, ��������ΪĬ��Ŀ¼
	//��ȡĿ¼��
	strDir = strFileName;
	strDir.TrimRight(strFileName);
	if (strDir != m_szDefaultFilePath)
	{
		m_szDefaultFilePath = strDir;
		m_dlgOpenFile.m_ofn.lpstrInitialDir = m_szDefaultFilePath;
		m_dlgSaveFile.m_ofn.lpstrInitialDir = m_szDefaultFilePath;
		WritePrivateProfileString(_P_APP_NAME_, _P_DEFAULT_PATH_, strDir, _P_PATH_);
	}
	strFile.TrimRight(L".bmp");
	strFile += L".bmp";
	strFileName.TrimRight(L".bmp");
	strFileName += L".bmp";
	//������ͼƬ
	m_image.Save(strFile);
	return;
}

void CBMPReaderDlg::OnOK()
{
	//��ʾ, ��������
	ShowSettingControl();
}

//��ʼ����Ա���� m_image (ͼ�����)
bool CBMPReaderDlg::InitalImage(int width, int height)
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
	//д���ɫ��
	RGBQUAD ColorTable[256];
	m_image.GetColorTable(0, 256, ColorTable);
	for (int i = 0; i < 256; i++)
	{
		ColorTable[i].rgbBlue = (BYTE)i;
		ColorTable[i].rgbGreen = (BYTE)i;
		ColorTable[i].rgbRed = (BYTE)i;
	}
	m_image.SetColorTable(0, 256, ColorTable);
	return true;
}

//��λͼ�������� m_image ��Ա������
void CBMPReaderDlg::LoadImageData(byte* data)
{
	if (data == nullptr)
		return;
	byte *pS;
	byte *pImg = (byte *)m_image.GetBits();
	int step = m_image.GetPitch();
	int height = m_image.GetHeight();
	for (int i = 0; i < m_image.GetHeight(); ++i)
	{
		//pS = data + (height - 1 - i) * m_nWidth;
		pS = data + i * m_nWidth;
		for (int j = 0; j < m_image.GetWidth(); ++j)
		{
			*(pImg + i*step + j) = pS[j];
		}
	}
}

//��ʼ��λͼ������
bool CBMPReaderDlg::InitalImageTemp()
{
	//�ж� m_nWidth m_nHeight �Ƿ�Ƿ�
	if (m_nWidth <= 0 || m_nHeight <= 0)
		return false;
	if (nullptr != m_imageTemp)
	{
		delete[] m_imageTemp;
		m_imageTemp = nullptr;
	}
	m_imageTemp = new byte[m_nWidth * m_nHeight];
	return nullptr != m_imageTemp;
}

//��ͼ����ַ�������ת��Ϊλͼ����
bool CBMPReaderDlg::GetImageData(char * data, byte * image_data)
{
	if (NULL == data)
		return false;

	size_t len = strlen(data);
	if (len < (m_nWidth * m_nHeight - 3) * 3 || len > (m_nWidth * m_nHeight + 3) * 3)
		return false;

	int index = 0;
	int result;
	
	if (sscanf_s(data, "%x%*c", &result) == 0)
		return false;

	while (true)
	{
		if (index > m_nWidth * m_nHeight - 1)
			break;
		if (sscanf_s(data + index * 3, "%x%*c", &result) == 0)
			*(image_data + index++) = 255;
		else
			*(image_data + index++) = (BYTE)result;
	}
	if (index < m_nWidth * m_nHeight)
		return false;
	return true;
}

//������а��е�ͼ������, ���ñ��, ������ͼ��
void CBMPReaderDlg::GetClipboardImage()
{
	if ((m_hasImage = LoadImageFromClipboard()) == true)
	{
		if (!m_image.IsNull())
			ResizeWindow(m_image.GetWidth(), m_image.GetHeight());
		OnPaint();
	}
}

//�Ӽ��а�������ͼ������, �����Ƿ��ȡ��ͼ����ж�
bool CBMPReaderDlg::LoadImageFromClipboard()
{
	char * buffer = NULL;
	bool ret = false;
	if (OpenClipboard())
	{
		HANDLE hData = GetClipboardData(CF_TEXT);
		buffer = (char*)GlobalLock(hData);
		GlobalUnlock(hData);
		//������������ַ�������һ�ο������ַ�����ͬ, �����ͼ��Ķ�ȡ
		if (m_szCopy != buffer)
		{
			ret = GetImageData(buffer, m_imageTemp);
			//���������ͽ��ͼ��ĳ���ͬ, �ȳ�ʼ�����ͼ��, ������ͼ��
			if (m_nWidth != m_image.GetWidth() || m_nHeight != m_image.GetHeight())
				InitalImage(m_nWidth, m_nHeight);
			LoadImageData(m_imageTemp);
		}
		//�رռ��а�
		CloseClipboard();
		//�洢�ַ���
		m_szCopy = buffer;
	}
	return ret;
}

//��TXT�ļ�������ͼ������
bool CBMPReaderDlg::LoadImageFromTXT(CString& szInFileName)
{
	USES_CONVERSION;
	CStdioFile file;
	CString data;
	bool ret;
	try
	{
		//��ȡ��TXT�ļ��е��ַ���, ��ת��Ϊͼ��, �ڶԻ����л��Ƴ���
		if (!file.Open(szInFileName, CFile::modeRead))
		{
			MessageBox(L"��ȡTXT�ļ�ʧ��");
			ret = false;
			goto End;
		}
		file.ReadString(data);
		//���ַ����е�λͼ���ݴ�ŵ�λͼ��������
		char * buffer = W2A((data.GetBuffer()));
		ret = GetImageData(buffer, m_imageTemp);
		file.Close();
		//�����ȡ�ɹ�, ¼��ͼ������
		if (true == ret)
		{
			//������ͼ���еĳߴ��봦��ߴ粻ͬ, ����ͼ���ȳ�ʼ��, ������λͼ����
			if (m_nWidth != m_image.GetWidth() || m_nHeight != m_image.GetHeight())
				InitalImage(m_nWidth, m_nHeight);
			LoadImageData(m_imageTemp);
		}
		//�����ȡʧ��, �޲����˳�
		else
			goto End;
	}
	catch (CFileException *e)
	{
		CString str;
		str.Format(L"��ȡ����ʧ�ܵ�ԭ����:%d", e->m_cause);
		MessageBox(L"str");
		file.Abort();
		e->Delete();
		ret = false;
		goto End;
	}
End:
	return ret;
}

void CBMPReaderDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OpenFile();
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

//��ק���ļ�����
void CBMPReaderDlg::OnDropFiles(HDROP hDropInfo)
{
	// ����ק���ļ����ļ���
	TCHAR Path[MAX_PATH] = { 0 };
	CString szFileName;
	if (hDropInfo)
	{
		int nDrag; //��ק�ļ�������
		nDrag = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
		if (nDrag == 1)
		{
			// �õ�����ק���ļ���
			DragQueryFile(hDropInfo, 0, Path, MAX_PATH);
			szFileName = Path;
			//���ļ�, ������ͼ��
			OpenFile(&szFileName);
			OnPaint();
		}
		else
			MessageBox(L"ֻ����קһ���ļ�����");
	}
	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}


void CBMPReaderDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
	static int i = 0;
	if (0 == i)
	{
		//�����а��е�����
		GetClipboardImage();
		++i;
	}
}

void CBMPReaderDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_isMousePress = true;
	CDialogEx::OnLButtonDown(nFlags, point);
}


int CBMPReaderDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	OleInitialize(NULL);
	return 0;
}


void CBMPReaderDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	OleUninitialize();
}

void CBMPReaderDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_isMousePress = false;
	CDialogEx::OnLButtonUp(nFlags, point);
}

//��ȡexe���ļ�Ŀ¼
CString GetExePath(void)
{
	CString strExePath;
	GetModuleFileName(NULL, strExePath.GetBuffer(MAX_PATH), MAX_PATH);
	strExePath.ReleaseBuffer();
	strExePath = strExePath.Left(strExePath.ReverseFind(_T('\\')));
	return strExePath;
}

void CBMPReaderDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	USES_CONVERSION;
	if (m_isMousePress)
	{
		if (true == m_hasImage)
		{
			m_image.Save(L"test.bmp");
			CString path = GetExePath();
			path += L"\\test.bmp";
			THDROP_Files filelist;
			filelist.push_back(W2A(path.GetBuffer()));
			DoDragDrop(filelist, DROPEFFECT_COPY);
			remove("test.bmp");
		}
		m_isMousePress = false;
	}
	CDialogEx::OnMouseMove(nFlags, point);
}
