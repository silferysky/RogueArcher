#include "Precompiled.h"
#include "REEditor.h"
#include "EditorManager.h"

namespace Rogue
{
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	EditorManager::EditorManager()
	{}

	void EditorManager::Init()
	{
		const char* glsl_version = "#version 130";

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsClassic();
		io.ConfigDockingWithShift = false;
		io.ConfigFlags = ImGuiConfigFlags_DockingEnable;

		ImGui_ImplWin32_Init(g_engine.GetWindowHandler());
		ImGui_ImplOpenGL3_Init(glsl_version);

		for (std::shared_ptr<IEditable> i : m_WindowsVector)
		{
			i->Init();
		}
	}

	void EditorManager::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Editor");
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		bool i = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("MainWindow", &i, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse);
		ImGui::PopStyleVar(2);
		
		ImGui::SetWindowSize(viewport->Size);
		ImGui::SetWindowPos({ 0.f,0.f });
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height, bytes_per_pixel;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);
		ImGuiID dockspace_id = ImGui::GetID("MainWindow");
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);

		for (auto& i : m_WindowsVector)
		{
			i->Update();
		}

		ImGui::End();
		ImGui::Render();

		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGui::EndFrame();
		ImGui::UpdatePlatformWindows();
		g_engine.m_coordinator.EndTimeSystem("Editor");
	}

	void EditorManager::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		//glfwDestroyWindow(window);
		//glfwTerminate();
	}
}
	

	
