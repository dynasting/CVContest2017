
// MFCtest1Dlg.h : ͷ�ļ�
//

#pragma once
#include "ShowDlg.h"


// CMFCtest1Dlg �Ի���
class CMFCtest1Dlg : public CDialogEx
{
// ����
public:
	CMFCtest1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCTEST1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
