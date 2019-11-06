#pragma once
#include "IEditable.h"
#include <vector>
#include "EditorHierarchyInfo.h"

namespace Rogue
{
	class ImGuiInspector : public IEditable
	{
		std::vector <HierarchyInfo>& activeObjects;
		ImVec4 m_color;
		public:
			ImGuiInspector();
			~ImGuiInspector() = default;
			virtual void Init() override;
			virtual void Update() override;
			virtual void Shutdown() override;
	};
};
