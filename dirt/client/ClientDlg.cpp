//*********************************************************
//** ClientDlg.cpp **
// Created By: KIRILL
// On :10/29/2003 00:18:20
// Comments: implementation file of ClientDlg
//
//*********************************************************
// REVISION by KIRILL, on 1/9/2004 03:03:37
// Comments: The problem of closing dialog by pressing Enter 
// or ESC corrected. OnSysCommand and OnCommand modified.
//
//*********************************************************
// REVISION by KIRILL, on 1/15/2004 22:42:23
// Comments: Test dialog added, but it seems that it doesn't work...
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#include "stdafx.h"
#include "CLIENT.h"
#include "ClientDlg.h"
#include "common/msg.h"
#include "environment.h"
#include "simpletracer.h"
#include "geometry.h"
#include "TestDialog.h"

//#include <atlimage.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

  // Dialog Data
  //{{AFX_DATA(CAboutDlg)
  enum { IDD = IDD_ABOUTBOX };
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
protected:
  //{{AFX_MSG(CAboutDlg)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
};

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  //{{AFX_MSG_MAP(CAboutDlg)
  // No message handlers
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
: CDialog(CClientDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CClientDlg)
  m_edit_addr = _T("");
  m_edit_port = 0;
  //}}AFX_DATA_INIT
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  m_bWannaClose = false;
};

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CClientDlg)
  DDX_Text(pDX, IDC_EDIT_SERVER_ADDR, m_edit_addr);
  DDV_MaxChars(pDX, m_edit_addr, 512);
  DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_edit_port);
  DDV_MinMaxInt(pDX, m_edit_port, 1, 65535);
  //}}AFX_DATA_MAP
};

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
  //{{AFX_MSG_MAP(CClientDlg)
  ON_WM_SYSCOMMAND()
  ON_WM_DESTROY()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
  ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
  ON_WM_KEYDOWN()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Add "About..." menu item to system menu.
  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL)
  {
    CString strAboutMenu;
    strAboutMenu.LoadString(IDS_ABOUTBOX);
    if (!strAboutMenu.IsEmpty())
    {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);            // Set big icon
  SetIcon(m_hIcon, FALSE);        // Set small icon

  // TODO: Add extra initialization here
  int ret = m_log_box.Attach( GetDlgItem( IDC_LOG_LIST )->m_hWnd ); //We have to attach 
  //m_log_box to the dialog item
  ASSERT( ret ); //Check whether we've attached successfully or not
  return TRUE;  // return TRUE  unless you set the focus to a control
};

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID & 0xFFF0) == IDM_ABOUTBOX)
  {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  }else{
    if (nID == SC_CLOSE)
      m_bWannaClose = true; //we are interested wether we have 
    //received SC_CLOSE or not
    CDialog::OnSysCommand(nID, lParam);
  }
};

void CClientDlg::OnDestroy()
{
  m_log_box.Detach();
  WinHelp(0L, HELP_QUIT);
  CDialog::OnDestroy();
};

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint() 
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialog::OnPaint();
  }
};

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
  return (HCURSOR) m_hIcon;
}


void CClientDlg::OnButtonTest() 
{
  //KIRILL: Temporal button and function for testing of some features

  int  imgWidth = 300, imgHeight = 300;
  ASSERT( imgWidth % 2 == 0 ); //Width must be EVEN for my realization to work!!!
  COLORREF * img = new COLORREF[imgWidth * imgHeight]; 

  Environment		scene;
  scene.SetAmbientColor( CVector(0.0,0.0,0.0) );

  CVector			sphereCenter(0,0,2), sphereCenter2(1,0,5), red(1,0,0), green(0,1,0), white(1,1,1);
  //CSphere     solidObject( sphereCenter, 1, CVector(1.0,0.8,0), 0.2, 2.0, true, 0,1,0.5);
  //CColorSphere	solidObject2(sphereCenter2, 2,white);
  //CSphere   solidObject( sphereCenter,2, 0.01, 2, true, 0.0, 1, 0.01);
  //CSphere   solidObject( sphereCenter,2, 0.01, 2, true, 0.0, 1, 0.01);
  //CBox        solidObject2(CVector (-1,-1,4), CVector(2,0,0),CVector(0,2,0),CVector(0,0,2),
  //            CVector(1,1,1),0.01,2,true,0.0,1,0.5,1.0);

  CVector			a(-1, -2, 2.5), b(1,-2,2.5), c(0,1,2.5);
  //CTriangle		solidObject3(a,b,c, CVector(1,1,1));
  Ray         axis( CVector(0,-1,3), CVector(0,1,0) );
  CCylinder   cylinder( axis, 2, 1, CVector(1.0, 0.8, 0.0), 0.5, 1.0, true, 0.0, 2.0, 0.0, 1.0 );
  CLight			lightSource( 1.0, 1.0, 1.0, 0, 0, 4);
  CLight			lightSource2( 1.0, 1.0, 0, 0, 0, 6);
  CLight			lightSource3( 0, 1.0, 0, 1, -1, 2);
  CLight			lightSource4( 0, 0, 1.0, 1, 0, 2);
  CLight            lightSource5(1.0,0,0, 0,0,0); 
  CVector		origin(0.0,0.0,0.0), zAxis(0,0,1), yAxis(0,1,0);
  CCamera	  camera( origin, zAxis, yAxis, imgWidth,imgHeight );
  //CImage    image;
  //image.Create( imgWidth, imgHeight, 24);

  Ray				rayToTrace(CVector(0,0,0), CVector(0,0,1));
  SimpleTracer	tracer( 5, 0.08, 0.08, 0.08, 1,1,1);
  CVector			color;
  Medium			medium;

  cylinder.SetSmoothness(5);
  medium.Betta = 0;
  medium.nRefr = 1;
  //camera.Yaw(-0.8);
  //camera.Shift(-3);
  //camera.Move(-3);
  //scene.Add( &solidObject );
  //solidObject.SetSmoothness( 10 );
  //scene.Add( &solidObject2 );
  //scene.Add( &solidObject3 );
  scene.Add( &cylinder );
  //scene.Add( &lightSource );
  scene.Add( &lightSource2 );
  //scene.Add( &lightSource3 );
  //scene.Add( &lightSource4 );

  tracer.trace(medium, rayToTrace, scene, color, true);
  //int i=56,j=143;
  //CRenderer::RenderPixel( scene, medium, camera, tracer, 150,149,color);

  for( int i = 0; i < imgWidth; i ++)
    for (int j = 0; j < imgHeight; j++)
    {
      CRenderer::RenderPixel( scene, medium, camera, tracer, i,j,color);

      BYTE c_red = (char) (color.x*255.0);
      BYTE c_green = (char) (color.y*255.0);
      BYTE c_blue = (char) (color.z*255.0);

      img[i+j*imgWidth] = RGB(c_blue, c_green, c_red); //Exactly this order!
      //image.SetPixelRGB( i,j, c_red, c_green, c_blue);
    };

  CTestDialog test_dlg( imgWidth, imgHeight, img);
  test_dlg.DoModal();
  //image.Save("out.bmp");
  delete[] img;
};

void CClientDlg::OnButtonStart() 
{
  if (!UpdateData())   //invalid input data entered
    return;

  TRY{
    CSocket* sock= new CSocket;
    BOOL res;
    res = sock->Create();

    if (!res){
      DWORD err = GetLastError();
      CString err_text = GetErrorMessageByErrorCode(err);
      ErrorMessageWithBox( err_text );
    }else{
      res = sock->Connect(m_edit_addr, m_edit_port);
      if (!res){
        DWORD err = GetLastError();
        CString err_text = GetErrorMessageByErrorCode(err);
        ErrorMessageWithBox( err_text );
      }else{
        sock->Close();
      }
    }
  }
  CATCH(CMemoryException, pEx){
    ErrorMessageFromException(pEx, TRUE);
    AfxAbort(); //can do nothing :)
  }AND_CATCH_ALL(pEx){
    ErrorMessageFromException(pEx, TRUE);
  }
  END_CATCH_ALL
};

BOOL CClientDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
  //to prevent closing by pressing ENTER or ESC we ignore IDOK and IDCANCEL commands
  //but to allow closing by pressing cross button we have to check wether we have
  //received WM_SYSCOMMAND + SC_CLOSE message or not. We do this in OnSysCommand() function
  //and set the value m_bWannaClose there.

  if ( (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) && !m_bWannaClose )
    return TRUE;
  else
    return CDialog::OnCommand(wParam, lParam);
};