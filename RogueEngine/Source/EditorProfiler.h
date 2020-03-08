/* Start Header ************************************************************************/
/*!
\file           EditorProfiler.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorProfiler

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "IEditable.h"
#include "Timer.h"

namespace Rogue
{
	class ImGuiProfiler : public IEditable
	{
	private:
		std::vector<float> m_vecTimeSystem; // For histogram
		const std::map<const char*, float>& m_timeSystemRef;
		float m_profileInterval;
		float m_profileAge;

	public:
		ImGuiProfiler();
		~ImGuiProfiler() = default;
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}