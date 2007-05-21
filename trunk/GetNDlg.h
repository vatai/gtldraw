#pragma once
#include "afxwin.h"
#include "Resource.h"
// GetNDlg dialog

class GetNDlg : public CDialog
{
	DECLARE_DYNAMIC(GetNDlg)

public:
	GetNDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GetNDlg();

// Dialog Data
	enum { IDD = IDD_GETN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_N;
	int m_Nval;
};
