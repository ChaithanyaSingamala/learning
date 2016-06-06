
#include "egl_win32_window.h"
#include "stdio.h"

#include <assert.h>
#include <tchar.h>
#include <dwmapi.h>


EGLWindow::EGLWindow(char *_windowName, int _width, int _height, bool _isInverted, bool _isTransprent)
{
	sprintf(windowName,"%s",_windowName);
	hDC=NULL;	
	hRC=NULL;	
	hWnd=NULL;	
	hInstance = NULL;
	width = _width;
	height = _height;
	isInverted = _isInverted;
	isTransprent = _isTransprent;

	CreateEGLWindow();

}
EGLWindow::~EGLWindow()
{

}
LRESULT CALLBACK WndProc(	HWND	hWnd, UINT	uMsg, WPARAM	wParam, LPARAM	lParam)			
{
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


EGLBoolean EGLWindow::CreateEGLContext ( EGLNativeWindowType hWnd)
{
   EGLint numConfigs;
   EGLint majorVersion;
   EGLint minorVersion;
   EGLDisplay display;
   EGLContext context;
   EGLSurface surface;
   EGLConfig config;
   EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

   EGLint attribList[] =
   {
       EGL_RED_SIZE,       8,
       EGL_GREEN_SIZE,     8,
       EGL_BLUE_SIZE,      8,
       EGL_ALPHA_SIZE,     8,
       EGL_DEPTH_SIZE,     24,
       EGL_STENCIL_SIZE,   EGL_DONT_CARE,
       EGL_SAMPLE_BUFFERS, 0,
	    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, 
       EGL_NONE
   };

   // Get Display
   display = eglGetDisplay(GetDC(hWnd));
   if ( display == EGL_NO_DISPLAY )
   {
      return EGL_FALSE;
   }

   // Initialize EGL
   if ( !eglInitialize(display, &majorVersion, &minorVersion) )
   {
      return EGL_FALSE;
   }

   // Get configs
   if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
   {
      return EGL_FALSE;
   }

   // Choose config
   if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) )
   {
      return EGL_FALSE;
   }

   // Create a surface
   surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
   if ( surface == EGL_NO_SURFACE )
   {
      return EGL_FALSE;
   }

   // Create a GL context
   context = eglCreateContext(display, config, NULL, contextAttribs );
   if ( context == EGL_NO_CONTEXT )
   {
	   int error = eglGetError();
	 //  EGL_BAD_ALLOC
      return EGL_FALSE;
   }   
   
   // Make the context current
   if ( !eglMakeCurrent(display, surface, surface, context) )
   {
      return EGL_FALSE;
   }
   
   eglDisplay = display;
   eglSurface = surface;
   eglContext = context;
   return EGL_TRUE;
}

bool EGLWindow::CreateEGLWindow() 
{
	DWORD		dwExStyle;				
	DWORD		dwStyle;	
	RECT		WindowRect;				
	WindowRect.left=(long)0;			
	WindowRect.right=(long)width;		
	WindowRect.top=(long)0;				
	WindowRect.bottom=(long)height;	

    WNDCLASSEX wc;
    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.cbClsExtra  = 0;
    wc.cbWndExtra  = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(0x00000000);
    wc.lpszClassName = (LPCSTR)windowName;

	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
	dwStyle=WS_OVERLAPPEDWINDOW;							

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, _T("RegisterClassEx - failed"), _T("Error"), MB_OK | MB_ICONERROR);
        return FALSE;
    }

    hWnd = CreateWindowEx(dwExStyle, (LPCSTR)windowName, (LPCSTR)windowName,
                     WS_VISIBLE | dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top,
                    NULL, NULL, wc.hInstance, NULL);

    if(!hWnd) {
        MessageBox(NULL, _T("CreateWindowEx - failed"), _T("Error"), MB_OK | MB_ICONERROR);
        return FALSE;
    }

	//if(isTransprent)
	//{
	//	DWM_BLURBEHIND bb = {0};
	//	HRGN hRgn = CreateRectRgn(0, 0, -1, -1);
	//	bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
	//	bb.hRgnBlur = hRgn;
	//	bb.fEnable = TRUE;
	//	DwmEnableBlurBehindWindow(hWnd, &bb);
	//}

	CreateEGLContext(hWnd);

    HDC hdc = GetDC(hWnd);

   // ReleaseDC(hWnd, hdc);

	ShowWindow(hWnd,SW_SHOW);	
    SetForegroundWindow(hWnd);					
	SetFocus(hWnd);		

    return (FALSE); 
}

void EGLWindow::KillEGLWindow()							
{
	if (hRC)										
	{
		hRC=NULL;					
	}

	if (hDC && !ReleaseDC(hWnd,hDC))
	{
		MessageBox(NULL,(LPCSTR)"Release Device Context Failed.",(LPCSTR)"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;					
	}

	

	if (hWnd && !DestroyWindow(hWnd))	
	{
		MessageBox(NULL,(LPCSTR)"Could Not Release hWnd.",(LPCSTR)"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;						
	}

	if (!UnregisterClass((LPCSTR)"OpenGL",hInstance))	
	{
		MessageBox(NULL,(LPCSTR)"Could Not Unregister Class.",(LPCSTR)"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}
