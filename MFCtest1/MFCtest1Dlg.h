
// MFCtest1Dlg.h : 头文件
//

#pragma once
#include "ShowDlg.h"


// CMFCtest1Dlg 对话框
class CMFCtest1Dlg : public CDialogEx
{
// 构造
public:
	CMFCtest1Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCTEST1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	friend class CShowDlg;
	double m_editCURX;
	double m_editCURY;
	double m_editLong;
	double m_editWide;
	double m_editLUX;
	double m_editLUY;
	double m_editLDX;
	double m_editLDY;
	double m_editRUX;
	double m_editRUY;
	double m_editRDX;
	double m_editRDY;
	afx_msg void OnBnClickedCalStopButton();
	//~CMFCtest1Dlg();
	afx_msg void OnBnClickedVideoButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedFinishButton();
};
