
// MFCtest1.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "cxcore.h"
#include "windows.h"
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"


// CMFCtest1App: 
// �йش����ʵ�֣������ MFCtest1.cpp
//

class CMFCtest1App : public CWinApp
{
public:
	CMFCtest1App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCtest1App theApp;