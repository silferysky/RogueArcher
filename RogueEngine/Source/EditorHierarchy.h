/* Start Header ************************************************************************/
/*!
\file           EditorHierarchy.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorHierarchy

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "IEditable.h"
#include "REEngine.h"
#include "EditorHierarchyInfo.h"
#include "EventDispatcher.h"
#include "KeyEvent.h"

namespace Rogue
{
	class ImGuiEditorHierarchy : public IEditable
	{
	private:
		std::vector<Entity>& m_currentVector;
		bool m_isHierarchyFocused;
		void DisplayHierarchyParent(HierarchyInfo& ent);
		void DisplayHierarchyChildren(HierarchyInfo& ent, size_t numOfParents = 0);
		void AcceptHierarchyPayload(HierarchyInfo& objInfo);
		
	public:
		static ImGuiEditorHierarchy& instance()
		{
			static ImGuiEditorHierarchy instance;
			return instance;
		}
		bool& GetIsHierarchyFocused();
		bool& set(bool test);
		ImGuiEditorHierarchy();
		~ImGuiEditorHierarchy();
		std::vector<Entity>& GetCurrentHierarchyVector();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
		
	};
}