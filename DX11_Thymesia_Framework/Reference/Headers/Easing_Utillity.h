#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CEasing_Utillity final
{
public:
	CEasing_Utillity();
	~CEasing_Utillity();

//public:
//	_vector	Easing(_uint eEasingType, _vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);


public:
	static _vector Linear(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector QuadIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector QuadOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector QuadInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector CubicIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector CubicOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector CubicInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector QuartIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector QuartOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector QuartInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector QuintIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector QuintOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector QuintInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector SineIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector SineOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector SineInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector ExpoIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector ExpoOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector ExpoInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector CircIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector CircOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector CircInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector ElasticIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector ElasticOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector ElasticInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector BounceIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector BounceOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
};

END