/* Start Header ************************************************************************/
/*!
\file           EditorSettings.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorSettings

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
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
			ImGuiEditorSettings::instance().SetCurrentState(Style::Dark);
		}

		if (ImGui::Button("Light Style"))
		{
			ImGui::StyleColorsLight();
			ImGuiEditorSettings::instance().SetCurrentState(Style::Light);
		}

		if (ImGui::Button("Classic Style"))
		{
			ImGui::StyleColorsClassic();
			ImGuiEditorSettings::instance().SetCurrentState(Style::Classic);
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
		if (ImGui::Button("Keybinds"))
		{
			ImGui::OpenPopup("Keybind");
		}
		bool open = true;
		if (ImGui::BeginPopupModal("Keybind", &open))
		{
			ImGui::Text("F1 : Zoom In Camera");
			ImGui::Text("F2 : Zoom Out Camera");
			ImGui::Text("F3 : On/Off Debug Draw");
			ImGui::Text("F5 : Toggle Editor Mode in Game");
			ImGui::Text("F6 : Toggle VSync On/Off");
			ImGui::Text("F11 : Toggle Full Screen On/Off");
			ImGui::Text("Shift : Toggle World Camera");
			ImGui::Text("Ctrl + C : Copy");
			ImGui::Text("Ctrl + V : Paste");
			ImGui::Text("Ctrl + Z : Undo");
			ImGui::Text("Ctrl + Y : Redo");
			if (ImGui::Button("Close"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		ImGui::End();
	}

	void ImGuiEditorSettings::Shutdown()
	{

	}

	Style ImGuiEditorSettings::getcurrentState() const
	{
		return m_currentBackgroundState;
	}

	void ImGuiEditorSettings::SetCurrentState(Style style)
	{
		m_currentBackgroundState = style;
	}
}