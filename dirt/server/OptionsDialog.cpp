//*********************************************************
//** OptionsDialog.h **
// Created By: Vader
// On: 01/27/2004 
// Comments:  implementation of server options dialog 'COptionsDialog'
//*********************************************************
// REVISION by KIRILL, on 1/28/2004 15:03:24
// Comments: Slight modifications done, some ASSERTs added.
//  OnOK() modified.
//
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
  , m_serverOptions( 0 )
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
  ASSERT( serverOptions );
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
	DDV_MinMaxInt(pDX, m_width, 1, MAX_IMAGE_WIDTH);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_height);
	DDV_MinMaxInt(pDX, m_height, 1, MAX_IMAGE_HEIGHT);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDV_MinMaxInt(pDX, m_port, 1, MAX_SERVER_PORT);
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
  int ret = UpdateData(TRUE);
  
  if ( ret ){
    //correct data were entered
    m_serverOptions->SetImageHeight(m_height);
    m_serverOptions->SetImageWidth(m_width);
    m_serverOptions->SetServerPort(m_port);  
    
	  CDialog::OnOK(); //data correct so we may close dialog
  } 
}
