#pragma once
#include "Vector2D.h"
#include "BaseComponent.h"

namespace Rogue
{
	class ChildComponent
		: public BaseComponent
	{
	public:
	
		ChildComponent() = default;
		~ChildComponent() = default;

		//From BaseComponent
		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
		void DisplayOnInspector();
	
		//For dirty parts
		void SetLocalDirty();
		void SetGlobalDirty();
		void ResetLocalDirty();
		void ResetGlobalDirty();
		bool IsLocalDirty() const;
		bool IsGlobalDirty() const;

		//Transform
		void SetTransform(Vec2 transform);
		void SetTransform(float x, float y);
		Vec2 GetTransform() const;
	
		void SetTransformZ(float z);
		float GetTransformZ() const;
	
		//Scale
		void SetScale(Vec2 scale);
		void SetScale(float x, float y);
		Vec2 GetScale() const;
	
		//Rotate
		void SetRotate(float rotate);
		float GetRotate() const;
	
	private:
		bool m_globalDirty;
		bool m_localDirty;

		Vec2 m_transform;
		float m_transformZ;
		Vec2 m_scale;
		float m_rotate;
	};
}
