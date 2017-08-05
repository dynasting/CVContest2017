
// MFCtest1Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCtest1.h"
#include "MFCtest1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CvCapture *capture;
CRect rect;
CDC *pDC;
HDC hDC;
CWnd *pwnd;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFCtest1Dlg �Ի���



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


// CMFCtest1Dlg ��Ϣ�������

BOOL CMFCtest1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	pwnd = GetDlgItem(IDC_VIDEO);
	//pwnd->MoveWindow(35,30,352,288);??
	pDC = pwnd->GetDC();
	//pDC?=GetDC();??
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCtest1Dlg::OnPaint()
{
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
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCtest1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CMFCtest1Dlg::OnBnClickedCalStopButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CShowDlg *m_pShowDlg;

	m_pShowDlg = new CShowDlg();
	m_pShowDlg->Create(IDD_SHOW_DIALOG, this);
	m_pShowDlg->ShowWindow(SW_SHOW);

	//���궨�༭�����ݶ��룬֮����Դ���
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
}





void CMFCtest1Dlg::OnBnClickedVideoButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!capture)
	{
		capture = cvCaptureFromCAM(0);
		//AfxMessageBox("OK");??
	}

	if (!capture)
	{
		AfxMessageBox(_T("�޷�������ͷ"));
		return;
	}
	//?����??
	IplImage *m_Frame;
	m_Frame = cvQueryFrame(capture);
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_Frame, 1);
	if (true)
	{
		m_CvvImage.DrawToHDC(hDC, &rect);
		//cvWaitKey(10);??
	}

	//?���ü�ʱ��,ÿ10ms����һ���¼�??
	SetTimer(1, 10, NULL);
}


void CMFCtest1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	IplImage* m_Frame;
	m_Frame = cvQueryFrame(capture);
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_Frame, 1);
	if (true)
	{
		m_CvvImage.DrawToHDC(hDC, &rect);
		//cvWaitKey(10);  
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CMFCtest1Dlg::OnBnClickedFinishButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cvReleaseCapture(&capture);
	CDC MemDC;
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BITMAP1);
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);

}
