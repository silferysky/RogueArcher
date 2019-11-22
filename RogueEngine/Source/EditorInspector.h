#pragma once
#include "IEditable.h"
#include "EditorHierarchyInfo.h"

namespace Rogue
{
	class ImGuiInspector : public IEditable
	{
		std::vector <Entity>& activeObjects;
		ImVec4 m_color;
		public:
			ImGuiInspector();
			~ImGuiInspector() = default;
			virtual void Init() override;
			virtual void Update() override;
			virtual void Shutdown() override;
	};
};
