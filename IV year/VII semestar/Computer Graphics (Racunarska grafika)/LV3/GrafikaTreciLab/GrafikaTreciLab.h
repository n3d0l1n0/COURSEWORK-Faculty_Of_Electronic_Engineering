
// GrafikaTreciLab.h : main header file for the GrafikaTreciLab application
//
#pragma once
#include "DImage.h"


#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"   // main symbols

// CGrafikaTreciLabApp:
// See GrafikaTreciLab.cpp for the implementation of this class
//

class CGrafikaTreciLabApp : public CWinApp
{
public:

	CGrafikaTreciLabApp() noexcept;
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGrafikaTreciLabApp theApp;
