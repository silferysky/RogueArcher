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
	std::vector<HierarchyInfo>& vector = g_engine.m_coordinator.GetActiveObjects();
	bool check = true;
	struct temp
	{
		std::string m_objectName;
		bool m_selected = false;
		Entity m_Entity = 0;
		float m_scaleX = 10.0f;
		float m_scaleY = 0.0f;
		float m_rotateX = 0.0f;
		float m_rotateY = 0.0f;
		float m_translateX = 0.0f;
		float m_translateY = 0.0f;
	};
	public:
		ImGuiInspector();
		~ImGuiInspector();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
};
