#pragma once
namespace Rogue
{
	class ICommandable
	{
	public:
		ICommandable() = default;
		virtual ~ICommandable() = default;

		virtual bool Execute() = 0;
		virtual bool Undo() = 0;
		//virtual void Redo() = 0;
	};

	/*class Model
	{
		bool m_On;
		int m_Count;
	public:
		Model() {}
		void SwitchOn()
		{
			m_On = true;
		}
		void SwitchOff()
		{
			m_On = false;
		}
		void switchCount(int count)
		{
			m_Count = count;
		}

		bool isOn()
		{
			return m_On;
		}

		int getCount()
		{
			return m_Count;
		}
	};

	class OnCommand :public ICommandable
	{
		Model* m_Model;
	public:
		OnCommand(Model& model) :m_Model(&model)
		{
		}
		void Execute() override
		{
			m_Model->SwitchOn();
		}
		void Undo() override
		{
			m_Model->SwitchOff();
		}
		void Redo() override
		{
			m_Model->SwitchOn();
		}
	};

	class OffCommand :public ICommandable
	{
		OnCommand m_OnCommand;
	public:
		OffCommand(Model& model) :m_OnCommand(model)
		{
		}
		void Execute() override
		{
			m_OnCommand.Undo();
		}
		void Undo() override
		{
			m_OnCommand.Execute();
		}
		void Redo() override
		{
			m_OnCommand.Undo();
		}
	};
	
	class SwitchCommand : public ICommandable
	{
		Model* m_Model;
		int m_oldCount;
		int m_newCount;

	public:
		SwitchCommand(Model* model,int count):m_Model(model),m_newCount(count)
		{
		}

		void Execute() override
		{
			m_oldCount = m_Model->getCount();
			m_Model->switchCount(m_newCount);
		}

		void Undo() override
		{
			m_Model->switchCount(m_oldCount);
		}

		void Redo() override
		{
			m_Model->switchCount(m_newCount);
		}
	};*/
}
