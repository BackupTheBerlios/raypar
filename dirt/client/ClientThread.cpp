//*********************************************************
//** ClientThread.cpp **
// Created By: KIRILL
// On: 1/29/2004 08:08:39
// Comments: client worker thread function realization.
//    client worker thread reners the image line
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//*********************************************************

#include "stdafx.h"
#include "client.h"
#include "ClientThread.h"
#include "ClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT ClientThreadFunction(void* param)
{
  SClientThreadParam * cp = (SClientThreadParam*) param; 
  ASSERT( AfxIsValidAddress(param, sizeof (SClientThreadParam) ) );
  
  CEvent& p_process_params_event = cp->process_params_event; //wait 
  CCriticalSection& params_cs = cp->params_cs; 

  TRY{
    while(1){
      if ( cp->bShouldExit )
        return 0; //we should stop

      BOOL ret = p_process_params_event.Lock(); //we wait here for access to thread params

      if ( cp->bShouldExit )
        return 0; //we should stop

      if( !ret ){
        ASSERT( 0 ); //very strange
        break; //we terminate this thread
      }

      params_cs.Lock(); //we enter critical section and don't allow
                        //main thread modify most params data members

      COLORREF* data = cp->data;
      CEnvironment* scene = cp->scene;
      CCamera* camera = cp->camera;
      int line_number = cp->line_number;
      HWND hwnd_main = cp->hwnd_main;

      ASSERT(data);
      ASSERT(scene);
      ASSERT(camera);
  
      int  imgWidth, imgHeight;
      camera->GetWidth( imgWidth );
      camera->GetHeight( imgHeight );

      ASSERT( line_number >=0 && line_number < imgHeight );

      SimpleTracer	tracer( 5, 0.08, 0.08, 0.08, 1,1,1);

      Medium			medium; //base medium - vacuum
      medium.Betta = 0;
      medium.nRefr = 1;
      CVector	color;

      for( int i = 0; i < imgWidth; i ++){      
        if ( cp->bShouldStop || cp->bShouldExit ) //if the main thread have set
                              //this variable to true we should stop rendering
          break;

        RenderPixel( *scene, medium, *camera, tracer, i, line_number, color);

        BYTE c_red =   (BYTE) (color.x*255.0);
        BYTE c_green = (BYTE) (color.y*255.0);
        BYTE c_blue =  (BYTE) (color.z*255.0);

        data[i] = RGB(c_blue, c_green, c_red); //Exactly this order!
      
      };

      params_cs.Unlock(); //leave critical section and allow for params modifications
      
      if(!cp->bShouldStop){ //main thread said us to stop, so we 
                        //probably haven't finished the image line
        ::PostMessage(hwnd_main, WM_CLIENT_LINE_RENDERED, 0, 0 );
      }
    }
  }
  CATCH_ALL(e){
    ErrorMessageFromException( e, TRUE ); //Show message box with error description
    AfxAbort(); //don't know how to recover, close the application
  }
  END_CATCH_ALL

  return 0;
}

CWinThread* StartClientThread( SClientThreadParam * param )
{  
  return AfxBeginThread( ClientThreadFunction, param );
}
