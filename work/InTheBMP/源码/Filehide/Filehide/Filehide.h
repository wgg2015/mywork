
// Filehide.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFilehideApp:
// �йش����ʵ�֣������ Filehide.cpp
//

class CFilehideApp : public CWinApp
{
public:
	CFilehideApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFilehideApp theApp;