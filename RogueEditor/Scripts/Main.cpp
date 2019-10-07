#include <cstdio>
#include <cstdlib>

#include <fstream>
#include "Main.h"
#include "Quad.h"
#include "SOIL.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "ImguiLayer.h"
GLuint texture[1];

double t = 0.0;
double gdt = 1.0;
//SystemManager *SysManager = new SystemManager();

static const int SCREEN_FULLSCREEN = 0;
static const int SCREEN_WIDTH = 960;
static const int SCREEN_HEIGHT = 540;

//Use for console
int main()
{
	ImGuiLayer::ImguiLayer Editor;
	Editor.StartWindow();
	Editor.UpdateWindow();
	Editor.CloseWindow();
	

	return 0;
}

