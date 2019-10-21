#pragma once
#include "Coordinator.h"

namespace Rogue
{
	class REEngine
	{
		using ChronoClock = std::chrono::time_point <std::chrono::steady_clock>;

		const float MICRO_TO_SECONDS = 1000000.0f;
		HWND m_hwnd;
		float m_accumulatedTime;
		int	m_stepCount;
		bool m_gameIsRunning;

		ChronoClock m_loopStart;
		ChronoClock m_loopEnd;

		bool InitializeOpenGL();
		void RegisterSystems();
		void RegisterComponents();

	public:
		REEngine();

		Coordinator m_coordinator;

		void init(HWND hWnd);
		void update(HDC hDC);
		void shutdown();

		float GetAccumulatedTime() const;
		int GetStepCount() const;
		HWND GetWindowHandle() const;
		void SetGameIsRunning(bool set);
	};
}