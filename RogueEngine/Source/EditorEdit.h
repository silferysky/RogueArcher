#pragma once
#include "IEditable.h"
#include "REEngine.h"
#include "Main.h"
namespace Rogue
{
	class ImGuiEditorEdit : public IEditable
	{
	std::vector<HierarchyInfo>& m_currentVector = g_engine.m_coordinator.GetActiveObjects();
	public:
		ImGuiEditorEdit();
		~ImGuiEditorEdit();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	

	};
}