//*********************************************************
//** OptionsDialog.h **
// Created By: Vader
// On: 01/27/2004 
// Comments:  implementation of server options dialog 'COptionsDialog'
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#include "stdafx.h"
#include "server.h"
#include "OptionsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog dialog


COptionsDialog::COptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDialog)
	m_width = 0;
	m_height = 0;
	m_port = 0;
	//}}AFX_DATA_INIT
}

COptionsDialog::COptionsDialog(CWnd* pParent, COptions *serverOptions)
	: CDialog(COptionsDialog::IDD, pParent)
{
	m_serverOptions = serverOptions;
  m_width = serverOptions->GetImageWidth();
	m_height = serverOptions->GetImageHeight();
	m_port = serverOptions->GetServerPort();
}


void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDialog)
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_width);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_height);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDV_MinMaxInt(pDX, m_port, 1, 65535);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDialog, CDialog)
	//{{AFX_MSG_MAP(COptionsDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog message handlers

void COptionsDialog::OnOK() 
{
  UpdateData(TRUE);
  m_serverOptions->SetImageHeight(m_height);
  m_serverOptions->SetImageWidth(m_width);
  m_serverOptions->SetServerPort(m_port);
  
	CDialog::OnOK();
}
