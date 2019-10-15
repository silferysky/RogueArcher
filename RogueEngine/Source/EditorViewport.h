#pragma once
#include "REEditor.h"
#include "IEditable.h"

namespace Rogue
{
	class ImGuiViewport : public IEditable
	{
	public:
		ImGuiViewport();
		~ImGuiViewport();
	};
}