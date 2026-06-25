
// GrafikaLab.h : main header file for the GrafikaLab application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGrafikaLabApp:
// See GrafikaLab.cpp for the implementation of this class
//

class CGrafikaLabApp : public CWinApp
{
public:
	CGrafikaLabApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGrafikaLabApp theApp;
