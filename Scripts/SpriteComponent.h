#pragma once
#include "BaseComponent.h"
#include "Matrix33.h"

struct SpriteComponent : public BaseComponent
{
	//Texture2D var
	//VertexList var
	//MeshSize var
	//Color var
	Vec2 _offset;
	Mtx33 _rotMat;
	Mtx33 _transMat;
	Mtx33 _scaleMat;
	int _z;


//public:

	SpriteComponent(Vec2 o = Vec2(), Mtx33 rot = Mtx33(), Mtx33 trans = Mtx33(), Mtx33 sca = Mtx33(), int z = 0)
		: BaseComponent(CMP_SPRITE), _offset{ o }, _rotMat{ rot }, _transMat{ trans }, _scaleMat{ sca }, _z{ z }{}

	/*inline void offset(const Vec2& o) { _offset = o; }
	inline Vec2 offset() const { return _offset; }
	inline void rotMat(const Mtx33& rot) { _rotMat = rot; }
	inline Mtx33 rotMat() const {return _rotMat;}
	inline void transMat(const Mtx33& trans) { _transMat = trans; }
	inline Mtx33 transMat() const { return _transMat; }
	inline void scalMat(const Mtx33& sca) { _scaleMat = sca; }
	inline Mtx33 scalMat() const { return _scaleMat; }*/
};