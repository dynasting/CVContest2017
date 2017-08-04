// ShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCtest1.h"
#include "ShowDlg.h"
#include "afxdialogex.h"
#include "MFCtest1Dlg.h"

#include "first_match.h"

// CShowDlg 对话框

IMPLEMENT_DYNAMIC(CShowDlg, CDialogEx)

CShowDlg::CShowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SHOW_DIALOG, pParent)
	, m_editResuNum(0)
	, m_edit1ID(0)
	, m_edit1X(0)
	, m_edit1Y(0)
	, m_edit1TH(0)
	, m_edit1S(0)
	, m_edit2ID(0)
	, m_edit2X(0)
	, m_edit2Y(0)
	, m_edit2TH(0)
	, m_edit2S(0)
	, m_edit3ID(0)
	, m_edit3X(0)
	, m_edit3Y(0)
	, m_edit3TH(0)
	, m_edit3S(0)
	, m_edit4ID(0)
	, m_edit4X(0)
	, m_edit4Y(0)
	, m_edit4TH(0)
	, m_edit4S(0)
	, m_edit5ID(0)
	, m_edit5X(0)
	, m_edit5Y(0)
	, m_edit5TH(0)
	, m_edit5S(0)
	, m_edit6ID(0)
	, m_edit6X(0)
	, m_edit6Y(0)
	, m_edit6TH(0)
	, m_edit6S(0)
	, m_edit7ID(0)
	, m_edit7X(0)
	, m_edit7Y(0)
	, m_edit7TH(0)
	, m_edit7S(0)
	, m_edit8ID(0)
	, m_edit8X(0)
	, m_edit8Y(0)
	, m_edit8TH(0)
	, m_edit8S(0)
	, m_edit9ID(0)
	, m_edit9X(0)
	, m_edit9Y(0)
	, m_edit9TH(0)
	, m_edit9S(0)
	, m_edit10ID(0)
	, m_edit10X(0)
	, m_edit10Y(0)
	, m_edit10TH(0)
	, m_edit10S(0)
{

}

CShowDlg::~CShowDlg()
{
}

void CShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RESU_NUM_EDIT, m_editResuNum);
	DDX_Text(pDX, IDC_1ID_EDIT, m_edit1ID);
	DDX_Text(pDX, IDC_1X_EDIT, m_edit1X);
	DDX_Text(pDX, IDC_1Y_EDIT, m_edit1Y);
	DDX_Text(pDX, IDC_1TH_EDIT, m_edit1TH);
	DDX_Text(pDX, IDC_1S_EDIT, m_edit1S);
	DDX_Text(pDX, IDC_2ID_EDIT, m_edit2ID);
	DDX_Text(pDX, IDC_2X_EDIT, m_edit2X);
	DDX_Text(pDX, IDC_2Y_EDIT, m_edit2Y);
	DDX_Text(pDX, IDC_2TH_EDIT, m_edit2TH);
	DDX_Text(pDX, IDC_2S_EDIT, m_edit2S);
	DDX_Text(pDX, IDC_3ID_EDIT, m_edit3ID);
	DDX_Text(pDX, IDC_3X_EDIT, m_edit3X);
	DDX_Text(pDX, IDC_3Y_EDIT, m_edit3Y);
	DDX_Text(pDX, IDC_3TH_EDIT, m_edit3TH);
	DDX_Text(pDX, IDC_3S_EDIT, m_edit3S);
	DDX_Text(pDX, IDC_4ID_EDIT, m_edit4ID);
	DDX_Text(pDX, IDC_4X_EDIT, m_edit4X);
	DDX_Text(pDX, IDC_4Y_EDIT, m_edit4Y);
	DDX_Text(pDX, IDC_4TH_EDIT, m_edit4TH);
	DDX_Text(pDX, IDC_4S_EDIT, m_edit4S);
	DDX_Text(pDX, IDC_5ID_EDIT, m_edit5ID);
	DDX_Text(pDX, IDC_5X_EDIT, m_edit5X);
	DDX_Text(pDX, IDC_5Y_EDIT, m_edit5Y);
	DDX_Text(pDX, IDC_5TH_EDIT, m_edit5TH);
	DDX_Text(pDX, IDC_5S_EDIT, m_edit5S);
	DDX_Text(pDX, IDC_6ID_EDIT, m_edit6ID);
	DDX_Text(pDX, IDC_6X_EDIT, m_edit6X);
	DDX_Text(pDX, IDC_6Y_EDIT, m_edit6Y);
	DDX_Text(pDX, IDC_6TH_EDIT, m_edit6TH);
	DDX_Text(pDX, IDC_6S_EDIT, m_edit6S);
	DDX_Text(pDX, IDC_7ID_EDIT, m_edit7ID);
	DDX_Text(pDX, IDC_7X_EDIT, m_edit7X);
	DDX_Text(pDX, IDC_7Y_EDIT, m_edit7Y);
	DDX_Text(pDX, IDC_7TH_EDIT, m_edit7TH);
	DDX_Text(pDX, IDC_7S_EDIT, m_edit7S);
	DDX_Text(pDX, IDC_8ID_EDIT, m_edit8ID);
	DDX_Text(pDX, IDC_8X_EDIT, m_edit8X);
	DDX_Text(pDX, IDC_8Y_EDIT, m_edit8Y);
	DDX_Text(pDX, IDC_8TH_EDIT, m_edit8TH);
	DDX_Text(pDX, IDC_8S_EDIT, m_edit8S);
	DDX_Text(pDX, IDC_9ID_EDIT, m_edit9ID);
	DDX_Text(pDX, IDC_9X_EDIT, m_edit9X);
	DDX_Text(pDX, IDC_9Y_EDIT, m_edit9Y);
	DDX_Text(pDX, IDC_9TH_EDIT, m_edit9TH);
	DDX_Text(pDX, IDC_9S_EDIT, m_edit9S);
	DDX_Text(pDX, IDC_10ID_EDIT, m_edit10ID);
	DDX_Text(pDX, IDC_10X_EDIT, m_edit10X);
	DDX_Text(pDX, IDC_10Y_EDIT, m_edit10Y);
	DDX_Text(pDX, IDC_10TH_EDIT, m_edit10TH);
	DDX_Text(pDX, IDC_10S_EDIT, m_edit10S);
}


BEGIN_MESSAGE_MAP(CShowDlg, CDialogEx)
	ON_BN_CLICKED(IDC_REC_BEGIN_BUTTON, &CShowDlg::OnBnClickedRecBeginButton)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CShowDlg 消息处理程序

/*BOOL CShowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CMFCtest1Dlg *pDlg = (CMFCtest1Dlg*)this->GetParent();
	m_edit2S = pDlg->m_editLUX;
	m_edit2X = 200;
	//UpdateData(FALSE);
	return TRUE;
}*/
void CShowDlg::OnBnClickedRecBeginButton()
{
	// TODO: 在此添加控件通知处理程序代码
	//CMFCtest1Dlg *pDlg1 = (CMFCtest1Dlg*)this->GetParent();
	//m_edit2S = pDlg1->m_editLDX;

	//m_edit2S = editLUX;
	m_edit2TH = 100;



	//识别程序从这里开始

	//用于识别的图像
	Mat color_denoised = imread("C:\\Users\\高峰\\Desktop\\比赛图像\\测试图片\\小图\\ready_to_recognize", 1);

	//ROI set
	//即将进行





	//to do
	UpdateData(FALSE);
}

void CShowDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnClose();
}
