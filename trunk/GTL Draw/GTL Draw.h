// GTL Draw.h : main header file for the GTL Draw application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CGTLDrawApp:
// See GTL Draw.cpp for the implementation of this class
//

class CGTLDrawApp : public CWinApp
{
public:
	CGTLDrawApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR  m_gdiplusToken;
};

extern CGTLDrawApp theApp;

