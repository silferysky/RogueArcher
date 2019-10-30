#pragma once
#include "IEditable.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "glew.h"
#include "REEngine.h"
#include "Main.h"

namespace Rogue
{
	class ImGuiInspector : public IEditable
	{
		std::vector <HierarchyInfo>& activeObjects = g_engine.m_coordinator.GetEntityManager().m_getActiveObjects();
		ImVec4 m_color = { 0.0f,0.0f,0.0f,0.0f };
		public:
			ImGuiInspector();
			~ImGuiInspector();
			virtual void Init() override;
			virtual void Update() override;
			virtual void Shutdown() override;
	};
};
