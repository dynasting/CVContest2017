
// MFCtest1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCtest1.h"
#include "MFCtest1Dlg.h"
#include "afxdialogex.h"
#include "first_match.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CvCapture *capture;
CRect rect;
CDC *pDC;
HDC hDC;
CWnd *pwnd;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCtest1Dlg 对话框



CMFCtest1Dlg::CMFCtest1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCTEST1_DIALOG, pParent)
	, m_editCURX(0)
	, m_editCURY(0)
	, m_editLong(0)
	, m_editWide(0)
	, m_editLUX(0)
	, m_editLUY(0)
	, m_editLDX(0)
	, m_editLDY(0)
	, m_editRUX(0)
	, m_editRUY(0)
	, m_editRDX(0)
	, m_editRDY(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCtest1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CUR_X_EDIT, m_editCURX);
	DDX_Text(pDX, IDC_CUR_Y_EDIT, m_editCURY);
	DDX_Text(pDX, IDC_CUR_Y_EDIT, m_editCURY);
	DDX_Text(pDX, IDC_LONG_EDIT, m_editLong);
	DDX_Text(pDX, IDC_WIDE_EDIT, m_editWide);
	DDX_Text(pDX, IDC_LU_X_EDIT, m_editLUX);
	DDX_Text(pDX, IDC_LU_Y_EDIT, m_editLUY);
	DDX_Text(pDX, IDC_LD_X_EDIT, m_editLDX);
	DDX_Text(pDX, IDC_LD_Y_EDIT, m_editLDY);
	DDX_Text(pDX, IDC_RU_X_EDIT, m_editRUX);
	DDX_Text(pDX, IDC_RU_Y_EDIT, m_editRUY);
	DDX_Text(pDX, IDC_RD_X_EDIT, m_editRDX);
	DDX_Text(pDX, IDC_RD_Y_EDIT, m_editRDY);
}

BEGIN_MESSAGE_MAP(CMFCtest1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CAL_STOP_BUTTON, &CMFCtest1Dlg::OnBnClickedCalStopButton)
	ON_BN_CLICKED(IDC_VIDEO_BUTTON, &CMFCtest1Dlg::OnBnClickedVideoButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_FINISH_BUTTON, &CMFCtest1Dlg::OnBnClickedFinishButton)
END_MESSAGE_MAP()


// CMFCtest1Dlg 消息处理程序

BOOL CMFCtest1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


									// TODO: 在此添加额外的初始化代码
	pwnd = GetDlgItem(IDC_VIDEO);
	//pwnd->MoveWindow(35,30,352,288);  
	pDC = pwnd->GetDC();
	//pDC =GetDC();  
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCtest1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCtest1Dlg::OnPaint()
{
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
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCtest1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CMFCtest1Dlg::OnBnClickedCalStopButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CShowDlg *m_pShowDlg;

	m_pShowDlg = new CShowDlg();
	m_pShowDlg->Create(IDD_SHOW_DIALOG, this);
	m_pShowDlg->ShowWindow(SW_SHOW);

	//将标定编辑框数据读入，之后可以处理
	m_pShowDlg->editLong = m_editLong;
	m_pShowDlg->editWide = m_editWide;
	m_pShowDlg->editLUX = m_editLUX;
	m_pShowDlg->editLUY = m_editLUY;
	m_pShowDlg->editLDX = m_editLDX;
	m_pShowDlg->editLDY = m_editLDY;
	m_pShowDlg->editRUX = m_editRUX;
	m_pShowDlg->editRUY = m_editRUX;
	m_pShowDlg->editRDX = m_editRDX;
	m_pShowDlg->editRDY = m_editRDY;

	//未经矫正
	Mat rawImage;
	rawImage = denoised();
	//imshow("22", rawImage);
	//wirte in denoised is true

	//即将传递到后续操作的信息（通过全局变量）
	/*calibrationInfo cali;*/

	vector<Point2f> points;
	points.push_back(Point2f(m_editLUX, m_editLUY));
	points.push_back(Point2f(m_editRUX, m_editRUY));
	points.push_back(Point2f(m_editLDX, m_editLDY));
	points.push_back(Point2f(m_editRDX, m_editRDY));


	//传入的点的顺序应该是，左上，右上，左下，右下
	cali = AffineTrans(points, m_editWide, m_editLong, rawImage);



	//cali 将作为全局变量

	/*头文件：state.h 源文件：state.cpp

	其它源文件：t1.cpp t2.cpp t3.cpp, 这些源文件都包含头文件state.h。
	需要定义一个全局变量供这些源文件中使用：方法如下
	1、在 state.h声明全局变量： extern int a;
	2、在state.cpp中定义该全局变量：int a = 10;
	这样其它源文件就可以使用该变量啦*/
}





void CMFCtest1Dlg::OnBnClickedVideoButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!capture)
	{
		capture = cvCaptureFromCAM(0);
		//AfxMessageBox("OK");  
	}

	if (!capture)
	{
		AfxMessageBox(_T("无法打开摄像头"));
		return;
	}
	// 测试  
	IplImage *m_Frame;
	m_Frame = cvQueryFrame(capture);
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_Frame, 1);
	if (true)
	{
		m_CvvImage.DrawToHDC(hDC, &rect);
		//cvWaitKey(10);  
	}

	// 设置计时器,每10ms触发一次事件  
	SetTimer(1, 10, NULL);
}



void CMFCtest1Dlg::OnTimer(UINT_PTR nIDEvent)
{


	// TODO: 在此添加消息处理程序代码和/或调用默认值
	IplImage* m_Frame;
	m_Frame = cvQueryFrame(capture);
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_Frame, 1);
	if (true)
	{
		m_CvvImage.DrawToHDC(hDC, &rect);
		//cvWaitKey(10);  
	}
	
	CPoint point;
	GetCursorPos(&point);
	// HWND hwnd=::GetForegroundWindow();
	HDC hDC = ::GetDC(NULL);

	// 再获取当前鼠标位置像素值
	COLORREF color = ::GetPixel(hDC, point.x, point.y);
	// m_colorState.SetBkColor(color);
	//int posx, posy, red, green, blue;

	CRect IRect;
	GetDlgItem(IDC_VIDEO)->GetWindowRect(&IRect);//获取控件相对于屏幕的位置
	if ((point.x >= IRect.left&&point.x <= IRect.right) && (point.y >= IRect.top&&point.y <= IRect.bottom))
	{
		//m_editX = IRect.left;
		//m_editY = IRect.top;
		m_editCURX = point.x - IRect.left;
		m_editCURY = point.y - IRect.top;
		//m_editRED = GetRValue(color);
		//m_editGREEN = GetGValue(color);
		//m_editBLUE = GetBValue(color);
		UpdateData(FALSE);
	}
	CDialogEx::OnTimer(nIDEvent);

}


void CMFCtest1Dlg::OnBnClickedFinishButton()
{
	

	// TODO: 在此添加控件通知处理程序代码
	cvReleaseCapture(&capture);
	/*CDC MemDC;
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BITMAP1);
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);*/

}
CMFCtest1Dlg::~CMFCtest1Dlg()
{
	cvReleaseCapture(&capture);
}
