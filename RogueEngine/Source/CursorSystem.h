#pragma once
#include "BaseSystem.h"
#include "EventListener.h"
#include "Vector2D.h"
#include "CursorManager.h"

namespace Rogue
{
	class CursorComponent;
	class TransformComponent;

	class CursorSystem : public System, public EventListener
	{
	public:
		CursorSystem();
		~CursorSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;
	};
}