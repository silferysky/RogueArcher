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
	class ImGuiGameObject : public IEditable
	{
	std::vector<HierarchyInfo>& m_currentVector = g_engine.m_coordinator.GetActiveObjects();
	public:
		ImGuiGameObject();
		~ImGuiGameObject();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}