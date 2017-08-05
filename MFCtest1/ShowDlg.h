#pragma once

// CShowDlg 对话框


class CShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowDlg)

public:
	CShowDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOW_DIALOG };
#endif
//private:
	//virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double editLong;
	double editWide;
	double editLUX;
	double editLUY;
	double editLDX;
	double editLDY;
	double editRUX;
	double editRUY;
	double editRDX;
	double editRDY;

	int m_editResuNum;
	int m_edit1ID;
	double m_edit1X;
	double m_edit1Y;
	double m_edit1TH;
	double m_edit1S;
	int m_edit2ID;
	double m_edit2X;
	double m_edit2Y;
	double m_edit2TH;
	double m_edit2S;
	int m_edit3ID;
	double m_edit3X;
	double m_edit3Y;
	double m_edit3TH;
	double m_edit3S;
	int m_edit4ID;
	double m_edit4X;
	double m_edit4Y;
	double m_edit4TH;
	double m_edit4S;
	int m_edit5ID;
	double m_edit5X;
	double m_edit5Y;
	double m_edit5TH;
	double m_edit5S;
	int m_edit6ID;
	double m_edit6X;
	double m_edit6Y;
	double m_edit6TH;
	double m_edit6S;
	int m_edit7ID;
	double m_edit7X;
	double m_edit7Y;
	double m_edit7TH;
	double m_edit7S;
	int m_edit8ID;
	double m_edit8X;
	double m_edit8Y;
	double m_edit8TH;
	double m_edit8S;
	int m_edit9ID;
	double m_edit9X;
	double m_edit9Y;
	double m_edit9TH;
	double m_edit9S;
	int m_edit10ID;
	double m_edit10X;
	double m_edit10Y;
	double m_edit10TH;
	double m_edit10S;
	afx_msg void OnBnClickedRecBeginButton();
	afx_msg void OnClose();
	double m_double[10][5];
	void numChange();
};
