#include "VSync.h"
#include "BasicIO.h"
#include "GL/glew.h"
#include "Windows.h"

namespace Rogue
{
	void setVSync(bool sync)
	{
		// Function pointer for the wgl extention function we need to enable/disable
		// vsync
		typedef BOOL(APIENTRY* PFNWGLSWAPINTERVALPROC)(int);
		PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

		const char* extensions = (char*)glGetString(GL_EXTENSIONS);

		if (strstr(extensions, "WGL_EXT_swap_control") == 0)
		{
			return;
		}
		else
		{
			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");

			if (wglSwapIntervalEXT)
				wglSwapIntervalEXT(sync);
		}
	}
}