#include "ImguiLayer.h"
#include "Quad.h"
#include "SOIL.h"
#include "REEditor.h"
#include "glew.h"
namespace ImGuiLayer
{
	ImguiLayer::ImguiLayer():window(nullptr)
	{

	}

	void ImguiLayer::StartWindow()
	{
		const char* glsl_version = "#version 130";
		/* Initialize the library */
		if (!glfwInit())
		{
			std::cout << "help" << std::endl;
		}
		window = glfwCreateWindow(1640, 1480, "Terence Dad Gay", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
		}
		glfwMakeContextCurrent(window);
		if (glewInit() != GLEW_OK)
			std::cout << "Oh fuck" << std::endl;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		/*io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   */      // Enable Multi-Viewport / Platform Windows
		ImGui::StyleColorsClassic();

		/*ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}*/



		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}
	void ImguiLayer::UpdateWindow()
	{
		ImGuiEditorFile::ImGuiEditorFile ImGuiFile;
		ImGuiInspector::ImGuiInspector ImGuiInspector;
		ImGuiEditorEdit::ImGuiEditorEdit ImGuiMenu;
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();


			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);
			if (show)
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
				ImGui::Checkbox("Another Window", &show_another_window);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float*)& clear_color); // Edit 3 floats representing a color
				if (ImGui::BeginMainMenuBar())
				{
					ImGuiFile.ImGuiEditorFileMenuInit();
					ImGuiMenu.ImGuiEditorEditInit();
					
					if (ImGui::BeginMenu("Assets"))
					{
						if (ImGui::BeginMenu("Create"))
						{
							if (ImGui::MenuItem("Folder"))
							{

							}

							if (ImGui::MenuItem("PreFab"))
							{

							}
							ImGui::EndMenu();
						}
						if (ImGui::MenuItem("Open", "CTRL + O", false, false))
						{

						}
						if (ImGui::MenuItem("Delete", "", false, false))
						{

						}
						if (ImGui::MenuItem("Rename", "", false, false))
						{

						}
						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu("GameObject"))
					{
						if (ImGui::MenuItem("Create Empty"))
						{

						}
						if (ImGui::MenuItem("Create Empty Child"))
						{

						}
						if (ImGui::BeginMenu("2D Object"))
						{
							if (ImGui::MenuItem("Sprite"))
							{

							}
							ImGui::Separator();

							if (ImGui::MenuItem("Sprite Mask"))
							{

							}

							if (ImGui::MenuItem("Tilemap"))
							{

							}
							ImGui::EndMenu();
						}
						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("Component"))
					{
						if (ImGui::MenuItem("Add"))
						{

						}
						if (ImGui::BeginMenu("Mesh"))
						{
							if (ImGui::MenuItem("Mesh Filter"))
							{

							}
							if (ImGui::MenuItem("TextMesh"))
							{

							}
							ImGui::Separator();
							if (ImGui::MenuItem("Mesh Renderer"))
							{

							}
							if (ImGui::MenuItem("Skinned Mesh Renderer"))
							{

							}
							ImGui::EndMenu();
						}
						if (ImGui::BeginMenu("Physics"))
						{
							if (ImGui::MenuItem("Rigid Body"))
							{

							}
							ImGui::Separator();
							if (ImGui::MenuItem("Box Collider"))
							{

							}

							if (ImGui::MenuItem("Circle Collider"))
							{

							}

							if (ImGui::MenuItem("Edge Collider"))
							{

							}

							if (ImGui::MenuItem("Polygon Collider"))
							{

							}

							ImGui::Separator();
							if (ImGui::MenuItem("Distant Joint"))
							{

							}
							if (ImGui::MenuItem("Friction Joint"))
							{

							}
							ImGui::EndMenu();
						}
						if (ImGui::BeginMenu("Audio"))
						{

							ImGui::EndMenu();
						}
						ImGui::EndMenu();
					}
					ImGui::EndMainMenuBar();
				}
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
			}
			if (Inspector)
			{

				ImGuiInspector.InitInspector();
			}

			if (Dropbox)
			{
				static ImGuiComboFlags flags = 0;
				ImGui::CheckboxFlags("ImGuiComboFlags_PopupAlignLeft", (unsigned int*)& flags, ImGuiComboFlags_PopupAlignLeft);
				if (ImGui::CheckboxFlags("ImGuiComboFlags_NoArrowButton", (unsigned int*)& flags, ImGuiComboFlags_NoArrowButton))
					flags &= ~ImGuiComboFlags_NoPreview;     // Clear the other flag, as we cannot combine both
				if (ImGui::CheckboxFlags("ImGuiComboFlags_NoPreview", (unsigned int*)& flags, ImGuiComboFlags_NoPreview))
					flags &= ~ImGuiComboFlags_NoArrowButton; // Clear the other flag, as we cannot combine both
				const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
				static const char* item_current = items[0];
				if (ImGui::BeginCombo("combo 1", item_current, flags)) // The second parameter is the label previewed before opening the combo.
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						bool is_selected = (item_current == items[n]);
						if (ImGui::Selectable(items[n], is_selected))
							item_current = items[n];
						if (is_selected)
							ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
					}
					ImGui::EndCombo();
				}
				static int item_current_2 = 0;
				ImGui::Combo("combo 2 (one-liner)", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

				// Simplified one-liner Combo() using an array of const char*
				static int item_current_3 = -1; // If the selection isn't within 0..count, Combo won't display a preview
				ImGui::Combo("combo 3 (array)", &item_current_3, items, IM_ARRAYSIZE(items));

				// Simplified one-liner Combo() using an accessor function
				struct FuncHolder { static bool ItemGetter(void* data, int idx, const char** out_str) { *out_str = ((const char**)data)[idx]; return true; } };
				static int item_current_4 = 0;
				ImGui::Combo("combo 4 (function)", &item_current_4, &FuncHolder::ItemGetter, items, IM_ARRAYSIZE(items));
			}
			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			ImGui::EndFrame();
			ImGui::UpdatePlatformWindows();
		}
	}
	void ImguiLayer::CloseWindow()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
	}
}
	
