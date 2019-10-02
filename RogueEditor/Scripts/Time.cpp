#pragma once
#include "Time.h"

float dt()
{
	/*while (1)
	{
		deltaTime = clock() - oldTime;
		double fps = (1.0f / deltaTime) * 1000;
		oldTime = clock();
		break;
	}
	return deltaTime;*/
	std::chrono::high_resolution_clock timer;
	auto start = timer.now();
	auto stop = timer.now();
	using ms = std::chrono::duration<float, std::milli>;
	deltaTime = std::chrono::duration_cast<ms>(stop - start).count();
	return deltaTime;
}

float fps(float dt)
{
	return (1.0f / dt) * 1000.0f;
}

void setVSync(bool sync)
{
	// Function pointer for the wgl extention function we need to enable/disable
	// vsync
	typedef BOOL(APIENTRY * PFNWGLSWAPINTERVALPROC)(int);
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


