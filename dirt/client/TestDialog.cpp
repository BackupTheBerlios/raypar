//*********************************************************
//** TestDialog.cpp **
// Created By: KIRILL
// Comments: Test dialog is used to show rendered image
//
//*********************************************************
// REVISION by KIRILL, on 1/30/2004 05:50:40
// Comments: Isn't used anymore
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************


//
#include "stdafx.h"
#include "client.h"
#include "TestDialog.h"
#include "common/msg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CTestDialog dialog

CTestDialog::CTestDialog( int width, int height, COLORREF* img)
: CDialog(CTestDialog::IDD, NULL)
, m_width( width )
, m_height( height )
, m_img( img )
{
  ASSERT( AfxIsValidAddress( img, sizeof(COLORREF)*width*height ) );
  
  //{{AFX_DATA_INIT(CTestDialog)
		// NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CTestDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTestDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDialog, CDialog)
//{{AFX_MSG_MAP(CTestDialog)
ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDialog message handlers

void CTestDialog::OnPaint() 
{
  CPaintDC dc(this); // device context for painting
  
  CBitmap bmp;
  int ret = bmp.CreateCompatibleBitmap( &dc, m_width, m_height );
  ASSERT( ret );
  ASSERT( m_width % 2 == 0 ); //Width must be EVEN for my realization!!!
  
  ret = bmp.SetBitmapBits( m_width*m_height*sizeof(COLORREF), m_img );
  ASSERT( ret == m_width*m_height*(int)sizeof(COLORREF) );
  
  CDC memDC;
  memDC.CreateCompatibleDC( &dc );
  
  CBitmap* prev_bmp = memDC.SelectObject( &bmp );
  
  ret = dc.BitBlt(0, 0, m_width, m_height, &memDC, 0, 0, SRCCOPY );
  if ( !ret ){  
    ErrorMessage( "BitBlt: %s",  
      (LPCSTR)  GetErrorMessageByErrorCode(GetLastError()) );
  }
  // ASSERT( ret );
  
  memDC.SelectObject( prev_bmp  );
  
  // Do not call CDialog::OnPaint() for painting messages
}
