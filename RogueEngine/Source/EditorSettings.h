/* Start Header ************************************************************************/
/*!
\file           EditorSettings.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorSettings

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "IEditable.h"
#include "glew.h"

namespace Rogue
{
	enum Style
	{
		Classic = 0,
		Dark,
		Light
	};

	class ImGuiEditorSettings:public IEditable
	{

	public:
		static ImGuiEditorSettings& instance()
		{
			static ImGuiEditorSettings instance;
			return instance;
		}
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
		Style getcurrentState() const;
		void SetCurrentState(Style style);
	private:
		bool show_demo_window = false;
		bool show_another_window = false;
		bool show = false;
		Style m_currentBackgroundState = Dark;

			
	};
}