
// RacunarskaGrafikaLV1GDI_Priprema.h : main header file for the RacunarskaGrafikaLV1GDI_Priprema application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRacunarskaGrafikaLV1GDIPripremaApp:
// See RacunarskaGrafikaLV1GDI_Priprema.cpp for the implementation of this class
//

class CRacunarskaGrafikaLV1GDIPripremaApp : public CWinApp
{
public:
	CRacunarskaGrafikaLV1GDIPripremaApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRacunarskaGrafikaLV1GDIPripremaApp theApp;
