// ShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCtest1.h"
#include "ShowDlg.h"
#include "afxdialogex.h"
#include "MFCtest1Dlg.h"
#include "first_match.h"
#include "Resource.h"
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
	, m_double()
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
	//识别程序从这里开始


	//标定数据需要传过来
	//这些数据是为了通过编译用的 应来源于标定
	/*calibrationInfo cali;*/
	cali.lenth = 400;
	cali.width = 400;
	cali.physicalLenth = 72;
	cali.physicalWidth = 72;


	//用于识别的图像
	Mat color_denoised = imread("C:\\Users\\高峰\\Documents\\ready_to_recognize.jpg", 1);

	//ROI set
	//向内挖去5像素
	//参见first_match.h 中 OFFSET=5

	Rect region_of_interest = Rect(OFFSET, OFFSET, color_denoised.cols-2* OFFSET, color_denoised.rows-2* OFFSET);
	Mat image_roi = color_denoised(region_of_interest);


	vector <rawResult> rawresults;
	rawresults = recognize(image_roi);

	//完成识别  进行比例运算
	vector <finalResult> finalresults;
	for (size_t i = 0; i < rawresults.size(); i++) {
		finalresults.push_back(rawresultToFinalResult(rawresults[i], cali));

	}


	//输出
	for (int i = 0; i < finalresults.size(); i++) {
		m_double[i][0] = finalresults[i].shapeComleteID;
		m_double[i][1] = finalresults[i].centreX;
		m_double[i][2] = finalresults[i].centreY;
		m_double[i][3] = finalresults[i].angle;
		m_double[i][4] = finalresults[i].area;

	}

	//将使用一些效率较低的方式完成数据输出
	//或改变相关定义优化

	//m_double[9][4] = 100 ;
	numChange();
	UpdateData(FALSE);
}

void CShowDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnClose();
}


void CShowDlg::numChange()
{
	m_edit1ID = m_double[0][0];
	m_edit1X = m_double[0][1];
	m_edit1Y = m_double[0][2];
	m_edit1TH = m_double[0][3];
	m_edit1S = m_double[0][4];
	m_edit2ID = m_double[1][0];
	m_edit2X = m_double[1][1];
	m_edit2Y = m_double[1][2];
	m_edit2TH = m_double[1][3];
	m_edit2S = m_double[1][4];
	m_edit3ID = m_double[2][0];
	m_edit3X = m_double[2][1];
	m_edit3Y = m_double[2][2];
	m_edit3TH = m_double[2][3];
	m_edit3S = m_double[2][4];
	m_edit4ID = m_double[3][0];
	m_edit4X = m_double[3][1];
	m_edit4Y = m_double[3][2];
	m_edit4TH = m_double[3][3];
	m_edit4S = m_double[3][4];
	m_edit5ID = m_double[4][0];
	m_edit5X = m_double[4][1];
	m_edit5Y = m_double[4][2];
	m_edit5TH = m_double[4][3];
	m_edit5S = m_double[4][4];
	m_edit6ID = m_double[5][0];
	m_edit6X = m_double[5][1];
	m_edit6Y = m_double[5][2];
	m_edit6TH = m_double[5][3];
	m_edit6S = m_double[5][4];
	m_edit7ID = m_double[6][0];
	m_edit7X = m_double[6][1];
	m_edit7Y = m_double[6][2];
	m_edit7TH = m_double[6][3];
	m_edit7S = m_double[6][4];
	m_edit8ID = m_double[7][0];
	m_edit8X = m_double[7][1];
	m_edit8Y = m_double[7][2];
	m_edit8TH = m_double[7][3];
	m_edit8S = m_double[7][4];
	m_edit9ID = m_double[8][0];
	m_edit9X = m_double[8][1];
	m_edit9Y = m_double[8][2];
	m_edit9TH = m_double[8][3];
	m_edit9S = m_double[8][4];
	m_edit10ID = m_double[9][0];
	m_edit10X = m_double[9][1];
	m_edit10Y = m_double[9][2];
	m_edit10TH = m_double[9][3];
	m_edit10S = m_double[9][4];
}
