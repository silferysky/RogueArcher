#pragma once
#include "ILogic.h"
#include "LogicComponent.h"
#include <memory>

namespace Rogue
{
	class BaseAI
		: public ILogic
	{
	public:

		BaseAI(LogicComponent& logicComp);
		virtual ~BaseAI() = default;

		//From ILogic
		void logicInit() override;
		void logicUpdate() override;
		void AIDetect() override;
		void AIActiveStateUpdate() override;

		//Getter/Setter
		std::shared_ptr<LogicComponent> getLogicComponent();
		void setLogicComponent(LogicComponent& logicComp);

	private:

		std::shared_ptr<LogicComponent> m_logicComponent;
	};
}