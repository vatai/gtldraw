// GetNDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GTL Draw.h"
#include "GetNDlg.h"


// GetNDlg dialog

IMPLEMENT_DYNAMIC(GetNDlg, CDialog)
GetNDlg::GetNDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GetNDlg::IDD, pParent)
	, m_Nval(3)
{
}

GetNDlg::~GetNDlg()
{
}

void GetNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_N, m_N);
	DDX_Text(pDX, IDC_N, m_Nval);
}


BEGIN_MESSAGE_MAP(GetNDlg, CDialog)
END_MESSAGE_MAP()


// GetNDlg message handlers
