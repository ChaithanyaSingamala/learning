#ifndef __GLWINDOW__
#define __GLWINDOW__


class EGLWindow;

class GLES3Renderer
{
private:
	EGLWindow *eglWindow;
public:
	GLES3Renderer();
	virtual ~GLES3Renderer();
};



#endif // __EGLWINDOW__