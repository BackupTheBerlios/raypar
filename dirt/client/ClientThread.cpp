#include "stdafx.h"
#include "client.h"
#include "ClientThread.h"
#include "ClientDlg.h"

UINT ClientThreadFunction(void* param)
{
  SClientThreadParam * cp = (SClientThreadParam*) param;
  ASSERT( AfxIsValidAddress(param, sizeof (SClientThreadParam) ) );
  
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
  //tracer.SetBackgroundColor(  );

  Medium			medium;
  medium.Betta = 0;
  medium.nRefr = 1;
  CVector	color;

  for( int i = 0; i < imgWidth; i ++){      
    RenderPixel( *scene, medium, *camera, tracer, i, line_number, color);

    BYTE c_red =   (BYTE) (color.x*255.0);
    BYTE c_green = (BYTE) (color.y*255.0);
    BYTE c_blue =  (BYTE) (color.z*255.0);

    data[i] = RGB(c_blue, c_green, c_red); //Exactly this order!
      
  };

  ::PostMessage(hwnd_main, WM_CLIENT_LINE_RENDERED, 0, 0 );

  return 0;
}

CWinThread* StartClientThread( SClientThreadParam * param )
{  
  return AfxBeginThread( ClientThreadFunction, param );
}
