#pragma once
#include "IEditable.h"
#include "REEngine.h"
#include "Main.h"
namespace Rogue
{
	class ImGuiGameObject : public IEditable
	{
	std::vector<Entity>& m_currentVector = g_engine.m_coordinator.GetActiveObjects();
	public:
		ImGuiGameObject();
		~ImGuiGameObject();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}