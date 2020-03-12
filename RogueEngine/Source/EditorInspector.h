/* Start Header ************************************************************************/
/*!
\file           EditorInspector.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (70%)
				Lim Jia Hao, l.jiahao, 390006518 (10%)
				Chan Wai Kit Terence, c.terence, 440005918 (10%)
				Javier Foo, javier.foo, 440002318 (10%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorInspector

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "IEditable.h"
#include "EditorHierarchyInfo.h"

namespace Rogue
{
	class ImGuiInspector : public IEditable
	{
		std::vector <Entity>& activeObjects;
		ImVec4 m_color;
		bool m_show = false;
	public:
		ImGuiInspector();
		~ImGuiInspector() = default;
		static ImGuiInspector& instance()
		{
			static ImGuiInspector instance;
			return instance;
		}
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
};
