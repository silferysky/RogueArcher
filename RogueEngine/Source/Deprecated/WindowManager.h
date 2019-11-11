#pragma once
#include "REEngine.h"

namespace Rogue
{
	class WindowManager
	{
		HWND  m_hWnd;				/* window */
	public:
		WindowManager() = default;
		~WindowManager() = default;

		void SetHWND(const HWND& hWnd)
		{
			m_hWnd = hWnd;
		}

		HWND GetHWND() const
		{
			return m_hWnd;
		}

	};
}