//****************************************
//** LogBox.cpp **
// Created By: KIRILL
// On :11/2/2003 01:29:51
// Comments: implementation of CLogBox
//
//***********************************
// REVISION by ..., on ...
// Comments: ...
//
//***********************************

#include "stdafx.h"
#include "..\CLIENT\CLIENT.h"
#include "LogBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////
//Types of messages (values must be > 0! ) 
#define LOG_MSG_ERROR  1
#define LOG_MSG_NORMAL 2

/////////////////////////////////////////////////////////////////////////////
// CLogBox

#define DEF_ERR_BKG_COLOR     RGB( 255, 190, 200 ) //KIRILL: I like this colors. 
#define DEF_ERR_TEXT_COLOR    RGB( 140,   0,   0 ) 
#define DEF_MSG_BKG_COLOR     RGB( 150, 250, 150 ) 
#define DEF_MSG_TEXT_COLOR    RGB(   0, 100,   0 )
#define DEF_FOCUS_FRAME_COLOR RGB(   0,  0,  255 )
#define DEF_MAX_MSG_COUNT     500


CLogBox::CLogBox()
//initial value of log colors
{
  SetErrBkgColor ( DEF_ERR_BKG_COLOR );
  SetErrTextColor( DEF_ERR_TEXT_COLOR );
  SetMsgBkgColor ( DEF_MSG_BKG_COLOR );
  SetMsgTextColor( DEF_MSG_TEXT_COLOR );
  SetFocusFrameColor( DEF_FOCUS_FRAME_COLOR );
  SetMaxMessageCount( DEF_MAX_MSG_COUNT );
}

CLogBox::~CLogBox()
{
}

//////////////////////////////////////////////////////////////////
//Attaches 'hwnd' to a CLogBox object
int CLogBox::Attach( HWND hwnd )
{
  int ret = CListBox::Attach( hwnd );
  ASSERT( ret ); //Have we attached successfully?
  ASSERT( GetStyle() & LBS_OWNERDRAWFIXED || 
    GetStyle() & LBS_OWNERDRAWVARIABLE  ); //Set the OWNERDRAW style !!!

  ASSERT( GetStyle() & LBS_HASSTRINGS ); //Set the LBS_HASSTRINGS style !!!

  return ret;
}

//Detaches CLogBox object from a window
HWND CLogBox::Detach()
{
  return CListBox::Detach();
}


//////////////////////////////////////////////////////////////////
// AddError and AddMessage methods set item's data member to 
// LOG_MSG_ERROR and LOG_MSG_NORMAL  respectively to let drawing 
//algorithm know what it draws.
//

//Adds line 'text' and sets up item's data member.
//Removes excess lines in the beginnig of the log list.
//Used by AddMessage and AddError
void CLogBox::_AddLine( LPCSTR text, int msg_type )
{
  int max_count = GetMaxMessageCount();
  while ( GetCount() > max_count )
    DeleteString( 0 ); //we left max_count only -1 lines 
  
  ASSERT( msg_type == LOG_MSG_ERROR || msg_type == LOG_MSG_NORMAL );
  
  int id = AddString( text );
  ASSERT( id>=0 );    
  SetItemData( id, msg_type );
}


//adds error message text
void CLogBox::AddError( LPCSTR text )
{
  _AddLine( text, LOG_MSG_ERROR );
}


//adds error message text
void CLogBox::AddMessage( LPCSTR text )
{
  _AddLine( text, LOG_MSG_NORMAL );
}
  
//////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CLogBox, CListBox)
	//{{AFX_MSG_MAP(CLogBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogBox message handlers

const int c_vertical_margin = 1; //vertical item box margin
const int c_left_margin = 2; //left item box margin


void CLogBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
  ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);  
  const char sText[] = "Zg"; //this text is used to determine required height 
                             //of the item's draw box

  CSize   sz;
  CDC*    pDC = GetDC();
  sz = pDC->GetTextExtent( sText );

  ReleaseDC(pDC);

  //set the height of the item 
  lpMeasureItemStruct->itemHeight = sz.cy + 2*c_vertical_margin; 
}

void CLogBox::DrawItem(LPDRAWITEMSTRUCT lp_draw) 
{
  ASSERT(lp_draw->CtlType == ODT_LISTBOX);
    
  if ( ( lp_draw->itemID == DWORD(-1) ) && (lp_draw->itemAction | ODA_FOCUS ) ) {
    //We have an empty list box. We may draw focus frame and return.
    //We won't draw anything
    return;
  }

  CString msg_text;
  GetText( lp_draw->itemID, msg_text );

  int msg_type = GetItemData( lp_draw->itemID );
  ASSERT( msg_type== LOG_MSG_NORMAL || msg_type == LOG_MSG_ERROR ); 
  

  CDC dc;
  dc.Attach(lp_draw->hDC);

  // Save these value to restore them when done drawing.
  COLORREF crOldTextColor = dc.GetTextColor();
  COLORREF crOldBkColor = dc.GetBkColor();

    
  //setup colors    
  COLORREF bkg_color  = ( msg_type == LOG_MSG_ERROR ) ? m_err_bkg_color : m_msg_bkg_color;
  COLORREF text_color = ( msg_type == LOG_MSG_ERROR ) ? m_err_text_color : m_msg_text_color;

  dc.SetBkColor( bkg_color );
  dc.SetTextColor( text_color );

  //fill item box
  dc.FillSolidRect(&lp_draw->rcItem, bkg_color );

  // If this item has the focus, draw a frame around the
  // item's rect.
  if ( lp_draw->itemState & ODS_FOCUS )      
   {
      CBrush br( GetFocusFrameColor() );
      dc.FrameRect(&lp_draw->rcItem, &br);
   }

   // Draw the text.
   CRect r( lp_draw->rcItem.left + c_left_margin, 
            lp_draw->rcItem.top, 
            lp_draw->rcItem.right, 
            lp_draw->rcItem.bottom );

   dc.DrawText( msg_text, msg_text.GetLength(), &r,
      DT_LEFT|DT_SINGLELINE|DT_VCENTER);

   // Reset the background color and the text color back to their
   // original values.
   dc.SetTextColor(crOldTextColor);
   dc.SetBkColor(crOldBkColor);

   dc.Detach();
}

