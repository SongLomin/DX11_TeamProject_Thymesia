#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CEasing_Utillity final : public CBase
{
//public:
//	enum EasingTypeID
//	{
//		TYPE_Linear = 0,
//
//		TYPE_QuadIn = 1,
//		TYPE_QuadOut,
//		TYPE_QuadInOut,
//
//		TYPE_CubicIn = 4,
//		TYPE_CubicOut,
//		TYPE_CubicInOut,
//
//		TYPE_QuarticIn = 7,
//		TYPE_QuarticOut,
//		TYPE_QuarticInOut,
//
//		TYPE_QuinticIn = 10,
//		TYPE_QuinticOut,
//		TYPE_QuinticInOut,
//
//		TYPE_SinIn = 13,
//		TYPE_SinOut,
//		TYPE_SinInOut,
//
//		TYPE_ExpoIn = 16,
//		TYPE_ExpoOut,
//		TYPE_ExpoInOut,
//
//		TYPE_CircularIn = 19,
//		TYPE_CircularOut,
//		TYPE_CircularInOut,
//
//		TYPE_ElasticIn = 22,
//		TYPE_ElasticOut,
//		TYPE_ElasticInOut,
//
//		TYPE_BounceIn = 25,
//		TYPE_BounceOut,
//
//
//		TYPE_End
//	};
//

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

	static _vector QuarticIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector QuarticOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector QuarticInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector QuinticIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector QuinticOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector QuinticInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector SinIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector sinfOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector sinfInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector ExpoIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector ExpoOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector ExpoInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector CircularIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector CircularOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector CircularInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector ElasticEaseIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector ElasticEaseOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector ElasticEaseInOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

	static _vector BounceEaseIn(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);
	static _vector BounceEaseOut(_vector vStartPoint, _vector vTargetPoint, _double fPassedTime, _double fTotalTime);

};

END