#include "EditorSettings.h"

namespace Rogue
{
	void ImGuiEditorSettings::Init()
	{
	}
	void ImGuiEditorSettings::Update()
	{
		ImGui::Begin("Settings");

		ImGui::Text("Change Background Style");
		if (ImGui::Button("Dark Style"))
		{
			ImGui::StyleColorsDark();
		}

		if (ImGui::Button("Light Style"))
		{
			ImGui::StyleColorsLight();
		}

		if (ImGui::Button("Classic Style"))
		{
			ImGui::StyleColorsClassic();
		}

		ImGui::Checkbox("Show Demo", &show_demo_window);
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
			//ImGui::ColorEdit3("clear color", (float*)& clear_color); // Edit 3 floats representing a color
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
		ImGui::End();
	}
	void ImGuiEditorSettings::Shutdown()
	{
	}
}