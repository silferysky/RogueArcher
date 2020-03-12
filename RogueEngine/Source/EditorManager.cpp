/* Start Header ************************************************************************/
/*!
\file           EditorManager.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorManager

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "REEditor.h"
#include "EditorManager.h"

namespace Rogue
{
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	EditorManager::EditorManager() :
		m_pickedEntity{ MAX_ENTITIES },m_isTileSetOpen(0), m_tileInstanceCount(0)
	{}

	void EditorManager::Init()
	{
		const char* glsl_version = "#version 130";

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGuizmo::SetOrthographic(true);
		RECT rect;
		::GetClientRect(g_engine.GetWindowHandler(), &rect);
		io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
		ImGui::StyleColorsDark();
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
		ImGuizmo::BeginFrame();
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
		RECT rect;
		::GetClientRect(g_engine.GetWindowHandler(), &rect);
		io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
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

		if (m_isTileSetOpen)
		{
			ImGuiTileSet::instance().Update();
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
	}

	int EditorManager::GetTileInstanceCount() const
	{
		return m_tileInstanceCount;
	}
	void EditorManager::ReduceTileInstanceCount()
	{
		--m_tileInstanceCount;
	}
	void EditorManager::IncreaseTileInstanceCount()
	{
		++m_tileInstanceCount;
	}
}
	

	
