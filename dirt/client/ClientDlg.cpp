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
// REVISION by KIRILL, on 1/30/2004 22:40:08
// Comments: Test dialog removed. Client thread creted.
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
//#include "TestDialog.h"
#include "ClientInfoExchange.h"
#include "ClientThread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//timer event id for reconnection timer
const int reconnect_timer_event_id = 17;

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
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
: CDialog(CClientDlg::IDD, pParent)
, m_settings(dialogSection, DIALOG_DEFAULT_LEFT, DIALOG_DEFAULT_TOP, 0, 0)
{
  //{{AFX_DATA_INIT(CClientDlg)
  m_edit_addr = _T("127.0.0.1");
  m_edit_port = 8700;
	m_connect_period = 5;
	//}}AFX_DATA_INIT
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  m_bWannaClose = false;
  m_line_to_render = -1; //negative means that nothing was rendered yet
  m_line_data = 0;
  m_b_standalone = TRUE;
  m_bWorking = false;

  m_scene.SetSceneUID( -1 ); //negative means that there is no available scene
  m_thread_params.bShouldExit = false;
  m_client_thread = StartClientThread( &m_thread_params ); //start client thread
};

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CClientDlg)
	DDX_Control(pDX, IDC_CHECK_STANDALONE, m_standalone_check);
  DDX_Text(pDX, IDC_EDIT_SERVER_ADDR, m_edit_addr);
  DDV_MaxChars(pDX, m_edit_addr, 512);
  DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_edit_port);
  DDV_MinMaxInt(pDX, m_edit_port, 1, 65535);
	DDX_Text(pDX, IDC_EDIT_PERIOD, m_connect_period);
  if ( m_b_standalone )
	  DDV_MinMaxInt(pDX, m_connect_period, 1, 3600);
	//}}AFX_DATA_MAP
};

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
  //{{AFX_MSG_MAP(CClientDlg)
  ON_WM_SYSCOMMAND()
  ON_WM_DESTROY()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_WM_TIMER()
  ON_WM_KEYDOWN()
  ON_MESSAGE( WM_CLIENT_LINE_RENDERED, OnLineRendered )
	ON_BN_CLICKED(IDC_CHECK_STANDALONE, OnCheckStandalone)
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


  m_options.GetDataFromReg();
  m_edit_port = m_options.GetServerPort();
  m_connect_period = m_options.GetConnectPeriod();
  m_edit_addr = m_options.GetServerAddress();
  m_b_standalone = m_options.GetMode(); //we should sinchronize the values of this variable
                                        //with m_standalone_check.
  m_standalone_check.SetCheck( m_b_standalone );
  UpdateOnCheckStandAlone();

  UpdateData(FALSE);

  //Loading info from reg on create
  m_settings.LoadNonResizablePosition(this);
  SetWindowText( "DiRT client" );

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
  
  m_settings.SavePosition(this);

  UpdateData(TRUE);
  m_options.SetConnectPeriod(m_connect_period);
  m_options.SetServerPort(m_edit_port);
  m_options.SetMode(m_b_standalone);
  m_options.SetServerAddress(m_edit_addr);
  m_options.SaveDataToReg();
  
  m_thread_params.bShouldExit = true; //notidy the client thread that it 
  m_thread_params.process_params_event.PulseEvent(); //

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


//void CClientDlg::OnButtonTest() 
//{
//  //KIRILL: Temporal button and function for testing of some features
//
//  //KIRILL: This call will break with memory error every time you run this:)
//
//  int  imgWidth = 300, imgHeight = 300;
//  ASSERT( imgWidth % 2 == 0 ); //Width must be EVEN for my realization to work!!!
//  COLORREF * img = new COLORREF[imgWidth * imgHeight]; 
//
//  CEnvironment		scene;
//  scene.SetAmbientColor( CVector(0.0,0.0,0.0) );
//
//  CVector			sphereCenter(0,0,2), sphereCenter2(1,0,5), red(1,0,0), green(0,1,0), white(1,1,1);
//  //CSphere     solidObject( sphereCenter, 1, CVector(1.0,0.8,0), 0.2, 2.0, true, 0,1,0.5);
//  //CColorSphere	solidObject2(sphereCenter2, 2,white);
//  //CSphere   solidObject( sphereCenter,2, 0.01, 2, true, 0.0, 1, 0.01);
//  //CSphere   solidObject( sphereCenter,2, 0.01, 2, true, 0.0, 1, 0.01);
//  CBox        solidObject2(CVector (-1,-1,4), CVector(2,0,0),CVector(0,2,0),CVector(0,0,2),
//                 CVector(1,1,1),0.01,2,true,0.0,1,0.5,1.0);
//
//  CVector			a(-1, -2, 0.5), b(1,-2,0.5), c(0,1,0.5);
//  CTriangle		solidObject3( a, b, c, red, 0.1, 1.0, true);
//  Ray         axis( CVector(0,-1,3), CVector(0,1,0) );
//  CCylinder   cylinder( axis, 2, 1, CVector(1.0, 0.8, 0.0), 0.5, 1.0, true, 0.0, 2.0, 0.0, 1.0 );
//  CLight			lightSource( 1.0, 1.0, 1.0, 0, 0, 4);
//  CLight			lightSource2( 1.0, 1.0, 0, 0, 0, 6);
//  CLight			lightSource3( 0, 1.0, 0, 1, -1, 2);
//  CLight			lightSource4( 0, 0, 1.0, 1, 0, 2);
//  CLight            lightSource5(1.0,0,0, 0,0,0); 
//  CVector		origin(0.0,0.0,0.0), zAxis(0,0,1), yAxis(0,1,0);
//  CCamera	  camera( origin, zAxis, yAxis, imgWidth,imgHeight );
//  //CImage    image;
//  //image.Create( imgWidth, imgHeight, 24);
//
//  Ray				rayToTrace(CVector(0,0,0), CVector(0,0,1));
//  SimpleTracer	tracer( 5, 0.08, 0.08, 0.08, 1,1,1);
//  tracer.SetBackgroundColor( CVector(0,0,0.5) );
//  CVector			color;
//  Medium			medium;
//
////  cylinder.SetSmoothness(5);
//  medium.Betta = 0;
//  medium.nRefr = 1;
//  //camera.Yaw(-0.4);
//  camera.Shift(1);
//  //camera.Move(-3);
//  //scene.Add( &solidObject );
//  //solidObject.SetSmoothness( 10 );
//  //scene.Add( &solidObject2 );
//  scene.Add( &solidObject3 );
//  scene.Add( &cylinder );
//
//  //scene.Add( &lightSource );
////  scene.Add( &lightSource2 );
//  //scene.Add( &lightSource3 );
//  //scene.Add( &lightSource4 );
//
//  tracer.trace(medium, rayToTrace, scene, color, true);
//  //int i=56,j=143;
//  //CRenderer::RenderPixel( scene, medium, camera, tracer, 150,149,color);
//
//  for( int i = 0; i < imgWidth; i ++)
//    for (int j = 0; j < imgHeight; j++)
//    {
//      RenderPixel( scene, medium, camera, tracer, i,j,color);
//
//      BYTE c_red = (char) (color.x*255.0);
//      BYTE c_green = (char) (color.y*255.0);
//      BYTE c_blue = (char) (color.z*255.0);
//
//      img[i+j*imgWidth] = RGB(c_blue, c_green, c_red); //Exactly this order!
//      //image.SetPixelRGB( i,j, c_red, c_green, c_blue);
//    };
//
//  CTestDialog test_dlg( imgWidth, imgHeight, img);
//  test_dlg.DoModal();
//  //image.Save("out.bmp");
//  delete[] img;
//};


//this is Start or Stop buton really
void CClientDlg::OnButtonStart() 
{
  if( m_bWorking ){ //shoulld stop
    m_thread_params.bShouldStop = true; //this will cause the client thread to
                                          //stop rendering
    KillTimer( reconnect_timer_event_id );
    SwitchToRelaxedMode();
  } else { //should start
    if ( !UpdateData() )   //invalid input data entered
      return;

    SwitchToWorkingMode();
    _DoRepeatedCommunications(); 
  }
}


void CClientDlg::StartRenderImageThread()
{  
  ASSERT( m_line_to_render >=0 );
  ASSERT( m_scene.IsValid() );
  ASSERT( m_client_thread != NULL );

  m_thread_params.params_cs.Lock(); //we should prevent the client thread 
                     //from reading params when we modify they

  //fill in client thread info structure
  m_thread_params.camera = &m_camera;
  m_thread_params.scene = &m_scene;  
  m_thread_params.line_number = m_line_to_render;
  m_thread_params.hwnd_main = *this;
  m_thread_params.bShouldExit = false;
  m_thread_params.bShouldStop = false;


  //allocate enough memory for image line
  int width;
  m_camera.GetWidth( width );
  ASSERT( width>=0 );
  ASSERT( m_line_data == 0 );
  m_line_data = new COLORREF[width];
  m_thread_params.data = m_line_data;

  m_thread_params.params_cs.Unlock(); //we allow the client thread to read the params

  m_thread_params.process_params_event.SetEvent(); //allow client
                      //thread to process params and render the next line
    
}


//Client thread use WM_CLIENT_LINE_RENDERED message to inform main thread that 
//new line was rendered and client thread finished its work
LRESULT CClientDlg::OnLineRendered(WPARAM wParam, LPARAM lParam)
{
  if( m_bWorking ){ //we are interested in this message only 
                    //if we haven't neen stoped, ie we are in work mode
    Message("Line rendered (line_num = %d)", m_line_to_render );  
    _DoRepeatedCommunications();
  }
  
  return 1; //this means that we've processed the message
}


void CClientDlg::DoCommunications(void)
{
  TRY{
    CSocket socket;
    BOOL res;
    res = socket.Create();

    if (!res){      
      CString err_text = GetErrorMessageByErrorCode();

      if (m_b_standalone) 
        ErrorMessage("%s", err_text);
      else
        ErrorMessageWithBox( err_text );
    }else{
      res = socket.Connect(m_edit_addr, m_edit_port);
      if (!res){        
        CString err_text = GetErrorMessageByErrorCode();
        if (m_b_standalone) 
          ErrorMessage("%s", err_text);
        else
          ErrorMessageWithBox( err_text );
      }else{           
        BOOL bReuseAddr = 1;
        socket.SetSockOpt(SO_REUSEADDR, &bReuseAddr, 4); //because of TIME_WAIT

        int new_line_to_render;
        int ret = ClientInfoExchange(m_line_to_render, m_line_data
             , socket, m_scene, m_camera, new_line_to_render );
        if (m_line_data){
          delete[] m_line_data;
          m_line_data = 0;
        }
        m_line_to_render = new_line_to_render;
          

        socket.ShutDown(2);
        socket.Close();
        if ( ret == CIE_NORMAL_RENDER_RETURN ){
          ASSERT( m_line_to_render >=0 );
          StartRenderImageThread();
        }else
          ResetAllSceneData();       
      }
    }
  }
  CATCH(CArchiveException, pEx){
    //this exception probably means that the connection was closed
    //so we ignore it
  }AND_CATCH(CFileException, pEx){
    //this exception probably means that the connection was closed
    //so we ignore it
  }AND_CATCH_ALL(pEx){
    ErrorMessageFromException(pEx, TRUE);
  }
  END_CATCH_ALL
}

void CClientDlg::ResetAllSceneData(void)
{
  m_scene.Empty();  
  m_line_to_render = -1;//nothing was rendered
  if (m_line_data){
    delete m_line_data;
    m_line_data = 0;
  }  
}

void CClientDlg::SwitchToWorkingMode()
{
  m_bWorking = true;

  CWnd* wnd;
  wnd = GetDlgItem( IDC_EDIT_SERVER_ADDR ); 
  ASSERT( wnd );
  wnd->EnableWindow(FALSE); //disable server address edit

  wnd = GetDlgItem( IDC_EDIT_SERVER_PORT ); 
  ASSERT( wnd );
  wnd->EnableWindow(FALSE); //disable server port edit

  wnd = GetDlgItem( IDC_EDIT_PERIOD ); 
  ASSERT( wnd );
  wnd->EnableWindow(FALSE); //disable edit period

  m_standalone_check.EnableWindow(FALSE);

  wnd = GetDlgItem( IDC_BUTTON_START ); 
  ASSERT( wnd );
  wnd->SetWindowText( "Stop" ); //Set text of "Start" button to "Stop"
}

void CClientDlg::SwitchToRelaxedMode()
{
  m_bWorking = false;

  CWnd* wnd;
  wnd = GetDlgItem( IDC_EDIT_SERVER_ADDR ); 
  ASSERT( wnd );
  wnd->EnableWindow(); //enable server address edit

  wnd = GetDlgItem( IDC_EDIT_SERVER_PORT ); 
  ASSERT( wnd );
  wnd->EnableWindow(); //enable server port edit

  wnd = GetDlgItem( IDC_EDIT_PERIOD ); 
  ASSERT( wnd );
  wnd->EnableWindow(); //enable edit period

  m_standalone_check.EnableWindow();
  UpdateOnCheckStandAlone();

  wnd = GetDlgItem( IDC_BUTTON_START ); 
  ASSERT( wnd );
  wnd->SetWindowText( "Start" ); //Set text of "Stop" button to "Start"
}

void CClientDlg::SetupTimer()
{
  ASSERT( m_connect_period > 0 );
  int ret = SetTimer(reconnect_timer_event_id, m_connect_period*1000, 0);
  ASSERT( ret );
}

void CClientDlg::OnTimer(UINT nIDEvent) 
{
  if ( nIDEvent == reconnect_timer_event_id ){
    ASSERT( m_bWorking ); //timer message can be received only in this mode
    KillTimer( reconnect_timer_event_id );
    _DoRepeatedCommunications();
  }
	
	CDialog::OnTimer(nIDEvent);
}

void CClientDlg::_DoRepeatedCommunications()
{
  DoCommunications();
  if( m_line_to_render <0 ){//means that we don't have work now
    if ( m_b_standalone )
      SetupTimer(); //we'l reconnect in m_connect_period seconds
    else
      SwitchToRelaxedMode();//we'll stop our work and wait for user commands
  } 
}

void CClientDlg::OnCheckStandalone() 
{
  UpdateOnCheckStandAlone();
}

void CClientDlg::UpdateOnCheckStandAlone()
{
  m_b_standalone = m_standalone_check.GetCheck();

  CWnd* wnd_edit_period = GetDlgItem( IDC_EDIT_PERIOD ); 
  CWnd* wnd_try_text = GetDlgItem( IDC_STATIC_TRY_TEXT ); 
  CWnd* wnd_seconds_text = GetDlgItem( IDC_STATIC_SECONDS_TEXT ); 

  ASSERT( wnd_edit_period );
  ASSERT( wnd_try_text );
  ASSERT( wnd_seconds_text );

  wnd_edit_period->EnableWindow( m_b_standalone );
  wnd_try_text->EnableWindow( m_b_standalone );
  wnd_seconds_text->EnableWindow( m_b_standalone );  
}
