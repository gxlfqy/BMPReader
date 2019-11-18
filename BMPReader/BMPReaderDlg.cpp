// BMPReaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BMPReader.h"
#include "BMPReaderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CString g_szCaption = L"BMP";

// CBMPReaderDlg 对话框

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

//快捷键Map
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

// CBMPReaderDlg 消息处理程序

BOOL CBMPReaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CString szWidth, szHeight, szCaption;		//从配置文件中获取到的参数
	::GetPrivateProfileString(_P_APP_NAME_, _P_IMG_WIDTH_, _DEFAULT_IMG_WIDTH_STR_, szWidth.GetBuffer(MAX_PATH), MAX_PATH, _P_PATH_);
	::GetPrivateProfileString(_P_APP_NAME_, _P_IMG_HEIGHT_, _DEFAULT_IMG_HEIGHT_STR_, szHeight.GetBuffer(MAX_PATH), MAX_PATH, _P_PATH_);
	::GetPrivateProfileString(_P_APP_NAME_, _P_DEFAULT_PATH_, _T(""), m_szDefaultFilePath.GetBuffer(MAX_PATH), MAX_PATH, _P_PATH_);
	//如果获取到配置文件中的默认目录参数, 将其设置为打开文件和保存文件窗口的默认目录
	if (m_szDefaultFilePath != "")
	{
		m_dlgSaveFile.m_ofn.lpstrInitialDir = m_szDefaultFilePath;
		m_dlgOpenFile.m_ofn.lpstrInitialDir = m_szDefaultFilePath;
	}
	//调整窗口中的对应参数
	INIT_EASYSIZE;					//控件位置初始化
	//设置控件IDC_WIDTH IDC_HEIGHT中的填充的参数
	GetDlgItem(IDC_WIDTH)->SetWindowText(szWidth);
	GetDlgItem(IDC_HEIGHT)->SetWindowText(szHeight);
	UpdateData(TRUE);			//同步成员变量 m_nWidth m_nHeight
	//初始化m_image
	InitalImage(m_nWidth, m_nHeight);
	InitalImageTemp();
	//调整窗口大小, 使其与图片的长宽适应
	ResizeWindow(m_image.GetWidth(), m_image.GetHeight());
	szCaption.Format(L"%s(%s*%s)", g_szCaption, szWidth, szHeight);
	SetWindowText(szCaption);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBMPReaderDlg::OnPaint()
{
	//绘制对话框的图标
	int cx, cy;
	CRect	rect;
	CWnd *pWnd = NULL;
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	//绘制对话框中的IDC_PIC1图像
	else
	{
		//确认对话框数据中是否有一张完整的图像
		if (m_hasImage && !m_image.IsNull())
		{
			//获取图片的宽 高度
			cx = m_image.GetWidth();
			cy = m_image.GetHeight();
			//ResizeWindow(cx, cy);
			//获取IDC_PIC1的窗口指针
			pWnd = GetDlgItem(IDC_PIC1);
			//获取Picture Control控件的大小
			pWnd->GetWindowRect(&rect);
			//将客户区选中到控件表示的矩形区域内
			ScreenToClient(&rect);
			//窗口移动到控件表示的区域
			pWnd->MoveWindow(rect.left, rect.top, cx, cy, TRUE);
			pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小
			CDC *pDc = NULL;
			pDc = pWnd->GetDC();//获取picture的DC
			m_image.Draw(pDc->m_hDC, rect);//将图片绘制到picture表示的区域内
			ReleaseDC(pDc);
		}
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	static int nWidth, nHeight;			//上一次所输入的长宽
	int w, h;							//当前输入的长宽
	CString szWidth, szHeight, szCaption;
	//获取编辑框当前状态
	BOOL isShow = GetDlgItem(IDC_WIDTH)->IsWindowVisible();
	//如果编辑框是处于显示状态, 获取编辑框中的参数
	if (isShow)
	{
		//获取编辑框中所填的数据
		UpdateData(TRUE);
		GetDlgItemText(IDC_WIDTH, szWidth);
		GetDlgItemText(IDC_HEIGHT, szHeight);
		swscanf_s(szWidth, L"%d", &w);
		swscanf_s(szHeight, L"%d", &h);
		//长宽如果发生改变, 则改变处理长宽和位图缓冲区, 并重置标志
		if (nWidth != w || nHeight != h)
		{
			//通过 m_nWidth 和 m_nHeight 参数初始化位图缓冲区
			m_nWidth = w;
			m_nHeight = h;
			InitalImageTemp();
			m_hasImage = false;
			//根据处理的长宽改变对话框大小, 注意并不删除结果图像
			ResizeWindow(m_nWidth, m_nHeight);
			//改变对话框的标题
			szCaption.Format(L"%s(%s*%s)", g_szCaption, szWidth, szHeight);
			SetWindowText(szCaption);
			//存储配置信息
			WritePrivateProfileString(_P_APP_NAME_, _P_IMG_WIDTH_, szWidth, _P_PATH_);
			WritePrivateProfileString(_P_APP_NAME_, _P_IMG_HEIGHT_, szHeight, _P_PATH_);
		}
		else{}
	}
	//交替切换编辑框的状态(显示, 隐藏)
	for (int id : SettingControlIDs)
	{
		if (isShow)
			GetDlgItem(id)->ShowWindow(SW_HIDE);
		else
			GetDlgItem(id)->ShowWindow(SW_SHOW);
	}
	//如果编辑框是处于隐藏状态, 将光标移动到IDC_WIDTH编辑框中, 并选中全部字符串
	if (!isShow)
	{
		CString strInfo;
		((CEdit*)GetDlgItem(IDC_WIDTH))->GetWindowText(strInfo);
		((CEdit*)GetDlgItem(IDC_WIDTH))->SetSel(0, strInfo.GetLength(), FALSE);
		GetDlgItem(IDC_WIDTH)->SetFocus();
	}
	//记录本次输入的长宽
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
	// TODO:  在此处添加消息处理程序代码
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
	// TODO:  在此处添加消息处理程序代码
}

void CBMPReaderDlg::OpenFile(CString * szOpenFileName /*= nullptr*/)
{
	CString strFile;					//文件的全名(地址)
	CString strDir;						//文件所在的目录
	CString strFileName;				//文件的文件名
	CString strFileExt;					//文件的后缀名
	//打开对话框模式
	if (szOpenFileName == nullptr)
	{
		if (m_dlgOpenFile.DoModal() != IDOK)
			return;
		//获取文件的全名(地址), 文件的文件名, 文件的后缀名
		strFile = m_dlgOpenFile.GetPathName();
		strFileName = m_dlgOpenFile.GetFileName();
		strFileExt = L"." + m_dlgOpenFile.GetFileExt();
		//记录对话框打开的文件所在的目录, 并将其作为默认目录
		//获取目录名
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
		//读取bmp文件
		m_image.Destroy();
		m_image.Load(strFile);
		m_hasImage = true;
	}
	else if (strFileExt == ".txt")
	{
		//设置标志
		m_hasImage = LoadImageFromTXT(strFile);
	}
	else
		MessageBox(L"所选的文件不是.bmp或.txt文件", L"提示");
	if (!m_image.IsNull())
		ResizeWindow(m_image.GetWidth(), m_image.GetHeight());
	//绘制图像
	OnPaint();
	m_szCopy = "";
}

void CBMPReaderDlg::SaveFile()
{
	//判断是否存在结果图像
	if (!m_hasImage)
		return;

	if (m_dlgSaveFile.DoModal() != IDOK)
		return;

	CString strFile;				//文件全名
	CString strDir;					//文件所在的目录
	CString strFileName;			//文件的文件名
	strFile = m_dlgSaveFile.GetPathName();
	strFileName = m_dlgSaveFile.GetFileName();
	//记录对话框打开的文件所在的目录, 并将其作为默认目录
	//获取目录名
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
	//保存结果图片
	m_image.Save(strFile);
	return;
}

void CBMPReaderDlg::OnOK()
{
	//显示, 隐藏设置
	ShowSettingControl();
}

//初始化成员变量 m_image (图像对象)
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
	//写入调色板
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

//将位图数据载入 m_image 成员变量中
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

//初始化位图缓冲区
bool CBMPReaderDlg::InitalImageTemp()
{
	//判断 m_nWidth m_nHeight 是否非法
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

//将图像的字符串数据转化为位图数据
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

//载入剪切板中的图像数据, 设置标记, 并绘制图像
void CBMPReaderDlg::GetClipboardImage()
{
	if ((m_hasImage = LoadImageFromClipboard()) == true)
	{
		if (!m_image.IsNull())
			ResizeWindow(m_image.GetWidth(), m_image.GetHeight());
		OnPaint();
	}
}

//从剪切板中载入图像数据, 返回是否读取到图像的判断
bool CBMPReaderDlg::LoadImageFromClipboard()
{
	char * buffer = NULL;
	bool ret = false;
	if (OpenClipboard())
	{
		HANDLE hData = GetClipboardData(CF_TEXT);
		buffer = (char*)GlobalLock(hData);
		GlobalUnlock(hData);
		//如果所拷贝的字符串与上一次拷贝的字符串不同, 则进行图像的读取
		if (m_szCopy != buffer)
		{
			ret = GetImageData(buffer, m_imageTemp);
			//如果处理长宽和结果图像的长宽不同, 先初始化结果图像, 再载入图像
			if (m_nWidth != m_image.GetWidth() || m_nHeight != m_image.GetHeight())
				InitalImage(m_nWidth, m_nHeight);
			LoadImageData(m_imageTemp);
		}
		//关闭剪切板
		CloseClipboard();
		//存储字符串
		m_szCopy = buffer;
	}
	return ret;
}

//从TXT文件中载入图像数据
bool CBMPReaderDlg::LoadImageFromTXT(CString& szInFileName)
{
	USES_CONVERSION;
	CStdioFile file;
	CString data;
	bool ret;
	try
	{
		//读取出TXT文件中的字符串, 并转化为图像, 在对话框中绘制出来
		if (!file.Open(szInFileName, CFile::modeRead))
		{
			MessageBox(L"读取TXT文件失败");
			ret = false;
			goto End;
		}
		file.ReadString(data);
		//将字符串中的位图数据存放到位图缓冲区中
		char * buffer = W2A((data.GetBuffer()));
		ret = GetImageData(buffer, m_imageTemp);
		file.Close();
		//如果读取成功, 录入图像数据
		if (true == ret)
		{
			//如果结果图像中的尺寸与处理尺寸不同, 则结果图像先初始化, 再载入位图数据
			if (m_nWidth != m_image.GetWidth() || m_nHeight != m_image.GetHeight())
				InitalImage(m_nWidth, m_nHeight);
			LoadImageData(m_imageTemp);
		}
		//如果读取失败, 无操作退出
		else
			goto End;
	}
	catch (CFileException *e)
	{
		CString str;
		str.Format(L"读取数据失败的原因是:%d", e->m_cause);
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

//拖拽的文件操作
void CBMPReaderDlg::OnDropFiles(HDROP hDropInfo)
{
	// 被拖拽的文件的文件名
	TCHAR Path[MAX_PATH] = { 0 };
	CString szFileName;
	if (hDropInfo)
	{
		int nDrag; //拖拽文件的数量
		nDrag = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
		if (nDrag == 1)
		{
			// 得到被拖拽的文件名
			DragQueryFile(hDropInfo, 0, Path, MAX_PATH);
			szFileName = Path;
			//打开文件, 并绘制图像
			OpenFile(&szFileName);
			OnPaint();
		}
		else
			MessageBox(L"只能拖拽一个文件！！");
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
		//检测剪切板中的数据
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

//获取exe的文件目录
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
