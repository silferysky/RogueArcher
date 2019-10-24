
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
		/* Initialize the library */
		//if (!glfwInit())
		//{
		//	std::cout << "help" << std::endl;
		//}
		//window = glfwCreateWindow(1640, 1480, "Rogue Editor", NULL, NULL);
		//if (!window)
		//{
		//	glfwTerminate();
		//}
		//glfwMakeContextCurrent(window);
		//
		//if (glewInit() != GLEW_OK)
		//	std::cout << "Oh damn" << std::endl;
		//

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsClassic();
		io.ConfigDockingWithShift = false;
		io.ConfigFlags = ImGuiConfigFlags_DockingEnable;

		ImGui_ImplWin32_Init(gEngine.GetWindowHandler());
		ImGui_ImplOpenGL3_Init(glsl_version);

		for (std::shared_ptr<IEditable> i : m_WindowsVector)
		{
			i->Init();
		}
	}

	void EditorManager::Update()
	{
		gEngine.m_coordinator.InitTimeSystem("Editor");
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		bool i = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("MainWindow", &i, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse);
		ImGui::PopStyleVar(2);

		ImGui::SetWindowSize(viewport->Size);
		ImGui::SetWindowPos({ 0.f,0.f });
		ImGuiIO& io = ImGui::GetIO();
		ImGuiID dockspace_id = ImGui::GetID("MainWindow");
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);

		for (auto& i : m_WindowsVector)
		{
			i->Update();
		}
		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		/*if (show)
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)& clear_color); // Edit 3 floats representing a color
			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}*/
		ImGui::End();
		ImGui::Render();
		//int display_w, display_h;
		//glfwGetFramebufferSize(window, &display_w, &display_h);
		//glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGui::EndFrame();
		ImGui::UpdatePlatformWindows();
		gEngine.m_coordinator.EndTimeSystem("Editor");
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
	

	
