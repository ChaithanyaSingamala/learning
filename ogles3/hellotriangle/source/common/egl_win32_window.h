#ifndef __EGLWINDOW__
#define __EGLWINDOW__

#include <windows.h>	

#include <EGL/egl.h>

class EGLWindow
{
private:
	int width;
	int height;
	char windowName[256];

	bool isInverted;
	bool isTransprent;

	HDC			hDC;	
	HGLRC		hRC;	
	HWND		hWnd;	
	HINSTANCE	hInstance;

	bool CreateEGLWindow();
	void KillEGLWindow();

	EGLDisplay eglDisplay;
	EGLContext eglContext;
	EGLSurface eglSurface;
	EGLBoolean CreateEGLContext ( EGLNativeWindowType hWnd);


public:
	EGLWindow(char *windowName, int _width, int _height, bool _isInverted = false, bool _isTransprent = false);
	virtual ~EGLWindow();

	bool isUpdate;

};


#endif // __EGLWINDOW__