//****************************************
//** ChildView.cpp **
// Created By: KIRILL
// On :10/27/2003 01:49:34
// Comments: implementation of the CChildView class
//
//***********************************
// REVISION by ..., on ...
// Comments: ...
//***********************************

#include "stdafx.h"
#include "SERVER.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
: m_bitmap_bits( 0 )
{
}

CChildView::~CChildView()
{
  delete[] m_bitmap_bits;
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
  if( m_bitmap_bits ){
//    CBitmap bmp;
//    int ret = bmp.CreateCompatibleBitmap( &dc, m_bitmap_width, m_bitmap_height );
//    ASSERT( ret );
//  
//    ret = bmp.SetBitmapBits( m_bitmap_width*m_bitmap_height*sizeof(COLORREF), m_bitmap_bits );
//    ASSERT( ret == m_bitmap_width*m_bitmap_height*(int)sizeof(COLORREF) );
  
      BITMAPINFO lpbmi;
      lpbmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      lpbmi.bmiHeader.biWidth = m_bitmap_width;
      lpbmi.bmiHeader.biHeight = m_bitmap_height;
      lpbmi.bmiHeader.biPlanes = 1;
      lpbmi.bmiHeader.biBitCount = 32;
      lpbmi.bmiHeader.biCompression = BI_RGB;
      lpbmi.bmiHeader.biSizeImage = m_bitmap_width * m_bitmap_height * 4;
      lpbmi.bmiHeader.biXPelsPerMeter = 0;
      lpbmi.bmiHeader.biYPelsPerMeter = 0;
      lpbmi.bmiHeader.biClrImportant = 0;
      lpbmi.bmiHeader.biClrUsed = 0;

      SetDIBitsToDevice(dc,0,0,m_bitmap_width, m_bitmap_height,0 ,0 ,0 ,m_bitmap_height ,m_bitmap_bits, &lpbmi ,DIB_PAL_COLORS);
    
//    CDC memDC;
//    memDC.CreateCompatibleDC( &dc );
//  
//    CBitmap* prev_bmp = memDC.SelectObject( &bmp );
//  
//    ret = dc.BitBlt(0, 0, m_bitmap_width, m_bitmap_height, &memDC, 0, 0, SRCCOPY );
//    if ( !ret ){  
//      ErrorMessage( "BitBlt: %s",  
//        (LPCSTR)  GetErrorMessageByErrorCode(GetLastError()) );
//   }
//    // ASSERT( ret );
//  
//    memDC.SelectObject( prev_bmp  );
  }
	
	// Do not call CWnd::OnPaint() for painting messages
}

//bitmap_bits may be 0 that means the absence of picture
void CChildView::SetBitmapParams(int width, int height, void* bitmap_bits)
{
  ASSERT( AfxIsValidAddress(bitmap_bits, width*height*sizeof(COLORREF) ) );

  m_bitmap_height = height;
  m_bitmap_width  = width;
  if( m_bitmap_bits ){
    delete m_bitmap_bits;
    m_bitmap_bits = 0;
  }
  if(  bitmap_bits){
    m_bitmap_bits = new COLORREF[width*height];
    memcpy(m_bitmap_bits, bitmap_bits, width*height*sizeof(COLORREF) );
  }  
}