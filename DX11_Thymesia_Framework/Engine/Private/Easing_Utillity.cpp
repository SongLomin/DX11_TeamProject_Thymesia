#include "..\Public\Easing_Utillity.h"

#define PI 3.141592f

CEasing_Utillity::CEasing_Utillity()
{
}

CEasing_Utillity::~CEasing_Utillity()
{
}
//
//_vector CEasing_Utillity::Easing(_uint eEasingType, _vector vStartPoint, _vector vTargetPoint, _float dPassedTime, _float dTotalTime)
//{
//	if (eEasingType >= TYPE_End)
//		return _vector();
//
//	switch (eEasingType)
//	{
//	case Engine::CEasing_Utillity::TYPE_Linear:
//		return Linear(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuadIn:
//		return QuadIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuadOut:
//		return  QuadOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuadInOut:
//		return  QuadInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_CubicIn:
//		return  CubicIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_CubicOut:
//		return  CubicOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_CubicInOut:
//		return  CubicInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuarticIn:
//		return  QuarticIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuarticOut:
//		return  QuarticOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuarticInOut:
//		return QuarticInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuinticIn:
//		return  QuinticIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuinticOut:
//		return QuinticOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuinticInOut:
//		return QuinticInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_SinIn:
//		return SinIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_SinOut:
//		return  sinfOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_SinInOut:
//		return sinfInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_ExpoIn:
//		return  ExpoIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_ExpoOut:
//		return  ExpoOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_ExpoInOut:
//		return  ExpoInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_CircularIn:
//		return  CircularIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_CircularOut:
//		return  CircularOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_CircularInOut:
//		return  CircularInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_ElasticIn:
//		return  ElasticEaseIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_ElasticOut:
//		return  ElasticEaseOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_ElasticInOut:
//		return  ElasticEaseInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_BounceIn:
//		return  BounceEaseIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_BounceOut:
//		return  BounceEaseOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	default:
//		break;
//	}
//
//	return _vector();
//}


_vector CEasing_Utillity::LerpToType(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime, EASING_TYPE eType)
{
	switch (eType)
	{
	case EASING_TYPE::LINEAR:
		return Linear(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::QUAD_IN:
		return QuadIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);

		break;
	case EASING_TYPE::QUAD_OUT:
		return QuadInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::QUAD_INOUT:
		return QuadInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::CUBIC_IN:
		return CubicIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::CUBIC_OUT:
		return CubicOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::CUBIC_INOUT:
		return CubicInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::QUART_IN:
		return QuartIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);

		break;
	case EASING_TYPE::QUART_OUT:
		return QuartOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);

		break;
	case EASING_TYPE::QUART_INOUT:
		return QuartInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);

		break;
	case EASING_TYPE::QUINT_IN:
		return QuintIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);

		break;
	case EASING_TYPE::QUINT_OUT:
		return QuintOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);

		break;
	case EASING_TYPE::QUINT_INOUT:
		return QuintInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);

		break;
	case EASING_TYPE::SINE_IN:
		return SineIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);

		break;
	case EASING_TYPE::SINE_OUT:
		return SineOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);

		break;
	case EASING_TYPE::SINE_INOUT:
		return SineInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::EXPO_IN:
		return ExpoIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);

		break;
	case EASING_TYPE::EXPO_OUT:
		return ExpoOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);

		break;
	case EASING_TYPE::EXPO_INOUT:
		return ExpoInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::CIRC_IN:
		return CircIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::CIRC_OUT:
		return CircOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::CIRC_INOUT:
		return CircInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::ELASTIC_IN:
		return ElasticIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::ELASTIC_OUT:
		return ElasticOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::ELASTIC_INOUT:
		return ElasticInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::BOUNCE_IN:
		return BounceIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::BOUNCE_OUT:
		return BounceOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
		break;
	case EASING_TYPE::TYPE_END:
		break;
	default:
		break;
	}

	return Linear(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_vector CEasing_Utillity::Linear(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _vector((vTargetPoint - vStartPoint) * fPassedTime / fTotalTime + vStartPoint);
}

_vector CEasing_Utillity::QuadIn(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _vector((vTargetPoint - vStartPoint) * fPassedTime*fPassedTime + vStartPoint);
}

_vector CEasing_Utillity::QuadIn(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	_vector	vStartPoint = XMVectorSet(fStartPoint, fStartPoint, fStartPoint, fStartPoint);
	_vector	vTargetPoint = XMVectorSet(fTargetPoint, fTargetPoint, fTargetPoint, fTargetPoint);

	return QuadIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_vector CEasing_Utillity::QuadOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _vector(-(vTargetPoint - vStartPoint) * fPassedTime*(fPassedTime - 2.f) + vStartPoint);
}

_vector CEasing_Utillity::QuadInOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2.f;
	if (fPassedTime < 1.f)
		return _vector((vTargetPoint - vStartPoint) / 2.f * fPassedTime*fPassedTime + vStartPoint);

	fPassedTime--;
	return _vector(-(vTargetPoint - vStartPoint) / 2.f * (fPassedTime*(fPassedTime - 2.f) - 1.f) + vStartPoint);
}

_vector CEasing_Utillity::CubicIn(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _vector((vTargetPoint - vStartPoint) * fPassedTime*fPassedTime*fPassedTime + vStartPoint);
}

_vector CEasing_Utillity::CubicOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _vector((vTargetPoint - vStartPoint) * (fPassedTime*fPassedTime*fPassedTime + 1.f) + vStartPoint);
}

_vector CEasing_Utillity::CubicInOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2.f;
	if (fPassedTime < 1.f) return (vTargetPoint - vStartPoint) / 2.f * fPassedTime*fPassedTime*fPassedTime + vStartPoint;
	fPassedTime -= 2.f;
	return _vector((vTargetPoint - vStartPoint) / 2.f * (fPassedTime*fPassedTime*fPassedTime + 2.f) + vStartPoint);
}

_vector CEasing_Utillity::QuartIn(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _vector((vTargetPoint - vStartPoint) * fPassedTime*fPassedTime*fPassedTime*fPassedTime + vStartPoint);
}

_vector CEasing_Utillity::QuartOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _vector(-(vTargetPoint - vStartPoint) * (fPassedTime*fPassedTime*fPassedTime*fPassedTime - 1.f) + vStartPoint);
}

_vector CEasing_Utillity::QuartInOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2.f;
	if (fPassedTime < 1.f) return _vector((vTargetPoint - vStartPoint) / 2.f * fPassedTime*fPassedTime*fPassedTime*fPassedTime + vStartPoint);
	fPassedTime -= 2.f;
	return _vector(-(vTargetPoint - vStartPoint) / 2.f * (fPassedTime*fPassedTime*fPassedTime*fPassedTime - 2.f) + vStartPoint);
}

_vector CEasing_Utillity::QuintIn(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _vector((vTargetPoint - vStartPoint) * fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + vStartPoint);
}

_vector CEasing_Utillity::QuintOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _vector((vTargetPoint - vStartPoint) * (fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + 1.f) + vStartPoint);
}

_vector CEasing_Utillity::QuintInOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2.f;
	if (fPassedTime < 1.f) return _vector((vTargetPoint - vStartPoint) / 2.f * fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + vStartPoint);
	fPassedTime -= 2.f;
	return _vector((vTargetPoint - vStartPoint) / 2.f * (fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + 2.f) + vStartPoint);
}

_vector CEasing_Utillity::SineIn(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _vector(-(vTargetPoint - vStartPoint) * cosf(fPassedTime / fTotalTime * (PI / 2.f)) + (vTargetPoint - vStartPoint) + vStartPoint);
}

_vector CEasing_Utillity::SineOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _vector((vTargetPoint - vStartPoint) * sinf(fPassedTime / fTotalTime * (PI / 2.f)) + vStartPoint);
}

_vector CEasing_Utillity::SineInOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _vector(-(vTargetPoint - vStartPoint) / 2.f * (cosf(PI*fPassedTime / fTotalTime) - 1.f) + vStartPoint);
}

_vector CEasing_Utillity::ExpoIn(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _vector((vTargetPoint - vStartPoint) * pow(2.f, 10.0f * (fPassedTime / fTotalTime - 1.f)) + vStartPoint);
}

_vector CEasing_Utillity::ExpoOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _vector((vTargetPoint - vStartPoint) * (-pow(2.f, -10.f * fPassedTime / fTotalTime) + 1.f) + vStartPoint);
}

_vector CEasing_Utillity::ExpoInOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2.f;
	if (fPassedTime < 1.f) return _vector((vTargetPoint - vStartPoint) / 2.f * pow(2.f, 10.f * (fPassedTime - 1.f)) + vStartPoint);
	fPassedTime--;
	return _vector((vTargetPoint - vStartPoint) / 2.f * (-pow(2.f, -10.f * fPassedTime) + 2.f) + vStartPoint);
}

_vector CEasing_Utillity::CircIn(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _vector(-(vTargetPoint - vStartPoint) * (sqrtf(1.f - fPassedTime * fPassedTime) - 1.f) + vStartPoint);
}

_vector CEasing_Utillity::CircOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _vector((vTargetPoint - vStartPoint) * sqrtf(1.f - fPassedTime * fPassedTime) + vStartPoint);
}

_vector CEasing_Utillity::CircInOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2.f;
	if (fPassedTime < 1.f) return _vector(-(vTargetPoint - vStartPoint) / 2.f * (sqrtf(1.f - fPassedTime * fPassedTime) - 1.f) + vStartPoint);
	fPassedTime -= 2.f;
	return _vector((vTargetPoint - vStartPoint) / 2.f * (sqrtf(1.f - fPassedTime * fPassedTime) + 1.f) + vStartPoint);
}

_vector CEasing_Utillity::ElasticOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{

	if ((fPassedTime /= fTotalTime) == 1.f)
		return vStartPoint + (vTargetPoint - vStartPoint);

	_float p = fTotalTime * .3f;
	_float s = p / 4.f;

	return _vector((vTargetPoint - vStartPoint) * pow(2.f, -10.f * fPassedTime) * sinf((fPassedTime * fTotalTime - s) * (2.f * PI) / p) + (vTargetPoint - vStartPoint) + vStartPoint);
}

_vector CEasing_Utillity::ElasticIn(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	if ((fPassedTime /= fTotalTime) == 1.f)
		return vStartPoint + (vTargetPoint - vStartPoint);

	_float p = fTotalTime * .3f;
	_float s = p / 4.f;

	return _vector(-((vTargetPoint - vStartPoint) * pow(2.f, 10.f * (fPassedTime -= 1.f)) * sinf((fPassedTime * fTotalTime - s) * (2.f * PI) / p)) + vStartPoint);
}

_vector CEasing_Utillity::ElasticInOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	if ((fPassedTime /= fTotalTime / 2.f) == 2.f)
		return _vector(vStartPoint + (vTargetPoint - vStartPoint));

	_float p = fTotalTime * (.3f * 1.5f);
	_float s = p / 4.f;

	if (fPassedTime < 1.f)
		return _vector(-.5f * ((vTargetPoint - vStartPoint) * pow(2.f, 10.f * (fPassedTime -= 1.f)) * sinf((fPassedTime * fTotalTime - s) * (2.f * PI) / p)) + vStartPoint);
	return _vector((vTargetPoint - vStartPoint) * pow(2.f, -10.f * (fPassedTime -= 1.f)) * sinf((fPassedTime * fTotalTime - s) * (2.f * PI) / p) * .5f + (vTargetPoint - vStartPoint) + vStartPoint);
}

_vector CEasing_Utillity::BounceOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	if ((fPassedTime /= fTotalTime) < (1 / 2.75f))
		return _vector((vTargetPoint - vStartPoint) * (7.5625f * fPassedTime * fPassedTime) + vStartPoint);
	else if (fPassedTime < (2 / 2.75f))
		return _vector((vTargetPoint - vStartPoint) * (7.5625f * (fPassedTime -= (1.5f / 2.75f)) * fPassedTime + .75f) + vStartPoint);
	else if (fPassedTime < (2.5f / 2.75f))
		return _vector((vTargetPoint - vStartPoint) * (7.5625f * (fPassedTime -= (2.25f / 2.75f)) * fPassedTime + .9375f) + vStartPoint);
	else
		return _vector((vTargetPoint - vStartPoint) * (7.5625f * (fPassedTime -= (2.625f / 2.75f)) * fPassedTime + .984375f) + vStartPoint);
}


_vector CEasing_Utillity::BounceIn(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _vector(BounceOut(vTargetPoint, vStartPoint, fTotalTime - fPassedTime, fTotalTime));
}
//_float CEasing_Utillity::BounceEaseInOut(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
//{
//	if (dPassedTime < dTotalTime * 0.5f)
//		return BounceEaseIn(vStartPoint, vTargetPoint * 0.5f, dPassedTime , dTotalTime * 0.5f);
//	else
//		return BounceEaseOut(vTargetPoint * 0.5f, vTargetPoint , dPassedTime-(dTotalTime * 0.5f) , dTotalTime * 0.5f);
//}
//
//_float CEasing_Utillity::BounceEaseOutIn(_vector vStartPoint, _vector vTargetPoint, _float fPassedTime, _float fTotalTime)
//{
//
//	if (dPassedTime < dTotalTime * 0.5f )
//		return BounceEaseOut(vStartPoint, vTargetPoint * 0.5f, dPassedTime * 0.5f, dTotalTime * 0.5f);
//	return BounceEaseIn(vTargetPoint * 0.5f, vTargetPoint, dPassedTime * 0.5f, dTotalTime * 0.5f);
//}

