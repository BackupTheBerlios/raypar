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

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

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
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDlg)
	DDX_Text(pDX, IDC_EDIT_SERVER_ADDR, m_edit_addr);
	DDV_MaxChars(pDX, m_edit_addr, 512);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_edit_port);
	DDV_MinMaxInt(pDX, m_edit_port, 1, 65535);
	//}}AFX_DATA_MAP
}

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
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
  // TODO: Add extra initialization here
	int ret = m_log_box.Attach( GetDlgItem( IDC_LOG_LIST )->m_hWnd ); //We have to attach 
                                                   //m_log_box to the dialog item
  ASSERT( ret ); //Check whether we've attached successfully or not
  	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

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
}

void CClientDlg::OnDestroy()
{
  m_log_box.Detach();
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

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
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CClientDlg::OnButtonTest() 
{
  //KIRILL: Temporal button and function for testing of some features
  
  int  imgWidth = 200, imgHeight = 200;
	
  ASSERT( imgWidth % 2 == 0 ); //Width must be EVEN for my realization to work!!!

  COLORREF * img = new COLORREF[imgWidth * imgHeight]; 

  Environment		scene;
	SimpleTracer	testedTracer;

	CVector			sphereCenter(0,0,5);
	Solid			* solidObject = new CSphere(&sphereCenter, 1);
	Light			* lightSource = new Light( 1.0, 1.0, 1.0, 500, 500 ,500);
	CVector			planePoint;
	CVector			origin(0,0,0);
	Medium			medium;
	Tracer			* tracer = new SimpleTracer();

	medium.Betta = 1;
	medium.nRefr = 1;

	planePoint.z = 1;

	scene.Add( solidObject );
	scene.Add( lightSource );

  for( int i = 0; i < imgWidth; i ++)
		for (int j = 0; j < imgHeight; j++)
		{
			CVector color;
			
			planePoint.x = ((double) i)/imgWidth/2 - 1.0;  
			planePoint.y = ((double) j)/imgHeight/2 - 1.0;

			Ray ray( &origin, &planePoint );
			tracer->trace( &medium, &ray, &scene, 1, &color);
			
      //It's time to create global EPS :)))
			#define EPS (1E-7)      
      ASSERT( ( -EPS < color.x ) && (color.x < 1+EPS ) );//must hold 
      ASSERT( ( -EPS < color.y ) && ( color.y < 1+EPS ) );
      ASSERT( ( -EPS < color.z ) && ( color.z < 1+EPS ) );

      ASSERT ( color.x < 0.001 );

			BYTE c_red = BYTE (color.x*255.0);
			BYTE c_green = (char) (color.y*255.0);
			BYTE c_blue = (char) (color.z*255.0);
			
      img[i+j*imgWidth] = RGB(c_blue, c_green, c_red); //Exactly this order!
		};

  for (int j=0; j<imgHeight-1; j++ )
    img[imgWidth*j]=RGB(255,0,0);
  
  CTestDialog test_dlg( imgWidth, imgHeight, img);
  test_dlg.DoModal();

	delete[] img;
	delete solidObject;
	delete lightSource;
	delete tracer;
}

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
}
