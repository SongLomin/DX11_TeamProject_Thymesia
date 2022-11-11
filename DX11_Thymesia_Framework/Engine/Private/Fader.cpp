#include "Fader.h"
#include "GameInstance.h"
#include "GameObject.h"

GAMECLASS_C(CFader)
CLONE_C(CFader, CComponent)

CFader::CFader(const CFader& rhs)
{
	*this = rhs;
}

HRESULT CFader::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CFader::Initialize(void* pArg)
{
	USE_START(CFader);


	m_tFaderDesc.vFadeColor = { 1.f, 1.f, 1.f, 1.f };



	return S_OK;
}

void CFader::Start()
{

	USE_TICK(CFader);
}

void CFader::Tick(_float fTimeDelta)
{
	if (!Get_Enable())
		return;

	switch (m_tFaderDesc.eFaderType)
	{
	case FADER_TYPE::FADER_OUT:
		Fade_Out(fTimeDelta);
		break;

	case FADER_TYPE::FADER_IN:
		Fade_In(fTimeDelta);
		break;

	case FADER_TYPE::FADER_OUTINLOOPING:
	case FADER_TYPE::FADER_INOUTLOOPING:
		Fade_Looping(fTimeDelta);

		break;

	default:
		//FaderType이 초기화되지 않았음.
		//DEBUG_ASSERT;
		m_tFaderDesc.vFadeColor.w = 1.f;
		break;

	}
}

void CFader::Bake_RatioByLinearType(_float& Out_fRatio)
{
	switch (m_tFaderDesc.eLinearType)
	{
	case LINEAR_TYPE::LNIEAR:
		Out_fRatio = (m_tFaderDesc.fFadeMaxTime - m_fFadeTime) / m_tFaderDesc.fFadeMaxTime;

		break;

	case LINEAR_TYPE::POLY:

		break;

	case LINEAR_TYPE::SQRT:
		Out_fRatio = (m_tFaderDesc.fFadeMaxTime - m_fFadeTime) / m_tFaderDesc.fFadeMaxTime;
		Out_fRatio = sqrt(Out_fRatio);
		break;

	case LINEAR_TYPE::POW:
		Out_fRatio = (m_tFaderDesc.fFadeMaxTime - m_fFadeTime) / m_tFaderDesc.fFadeMaxTime;
		Out_fRatio = pow(Out_fRatio, 1.5f);
		break;

	default:
		DEBUG_ASSERT;
	}
}

void CFader::Reset_Fader()
{
	m_fFadeTime = m_tFaderDesc.fFadeMaxTime;
	m_fCurrentDelayTime = m_tFaderDesc.fDelayTime;

}

void CFader::Exit_Fader()
{
	m_fFadeTime = -0.1f;
	m_fCurrentDelayTime = -0.1f;
	m_tFaderDesc.eFaderType = FADER_TYPE::FADER_END;

}

void CFader::Fade_In(_float fTimeDelta)
{
	m_fFadeTime -= fTimeDelta;

	if (m_fFadeTime < 0.f)
	{
		m_fCurrentDelayTime -= fTimeDelta;

		if (m_fCurrentDelayTime < 0.f)
		{
			CallBack_FadeEnd(FADER_TYPE::FADER_IN);
			m_tFaderDesc.vFadeColor.w = 1.f;
		}

		return;
	}
	_float ratio;
	Bake_RatioByLinearType(ratio);

	m_tFaderDesc.vFadeColor.w = ratio;
}

void CFader::Fade_Out(_float fTimeDelta)
{
	m_fFadeTime -= fTimeDelta;

	if (m_fFadeTime < 0.f)
	{
		m_fCurrentDelayTime -= fTimeDelta;

		if (m_fCurrentDelayTime < 0.f)
		{
			CallBack_FadeEnd(FADER_TYPE::FADER_OUT);
			m_tFaderDesc.vFadeColor.w = 0.f;
		}
		return;
	}

	_float ratio;
	Bake_RatioByLinearType(ratio);

	m_tFaderDesc.vFadeColor.w = 1.f - ratio;
}

void CFader::Fade_Looping(_float fTimeDelta)
{
	if (m_fFadeTime <= 0.f)
	{
		Reset_Fader();
		m_isFadeOut = true - m_isFadeOut;
		return;
	}

	m_isFadeOut ? Fade_Out(fTimeDelta) : Fade_In(fTimeDelta);
}



void CFader::Init_Fader(const FaderDesc& In_tFaderDesc)
{
	m_tFaderDesc = In_tFaderDesc;
	m_fFadeTime = m_tFaderDesc.fFadeMaxTime;
	m_fCurrentDelayTime = m_tFaderDesc.fDelayTime;
	
	if (FADER_TYPE::FADER_INOUTLOOPING == m_tFaderDesc.eFaderType)
	{
		m_isFadeOut = false;
	}
	else if(FADER_TYPE::FADER_OUTINLOOPING == m_tFaderDesc.eFaderType)
	{
		m_isFadeOut = true;
	}
}


void CFader::OnDestroy()
{
	UNUSE_START(CFader);
	UNUSE_TICK(CFader);
}

void CFader::Free()
{

}
