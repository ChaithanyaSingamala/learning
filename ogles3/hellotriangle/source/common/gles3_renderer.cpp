#include "gles3_renderer.h"
#include "egl_win32_window.h"

GLES3Renderer::GLES3Renderer()
{
	eglWindow = new EGLWindow("GLWindow", 960, 360);
}
GLES3Renderer::~GLES3Renderer()
{

}