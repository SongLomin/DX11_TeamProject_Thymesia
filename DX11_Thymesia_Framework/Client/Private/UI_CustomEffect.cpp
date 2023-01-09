#include "stdafx.h"
#include "UI_CustomEffect.h"
#include "Shader.h"
#include "Texture.h"
#include "UI_Utils.h"
#include "EasingComponent_Transform.h"
#include "EasingComponent_Alpha.h"
#include "EasingComponent_Bezier.h"
#include "EasingComponent_Float.h"

GAMECLASS_C(CUI_CustomEffect)
CLONE_C(CUI_CustomEffect, CGameObject)

HRESULT CUI_CustomEffect::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUI_CustomEffect::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

HRESULT CUI_CustomEffect::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_CustomEffect::Tick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();

	__super::Tick(fTimeDelta);

}

void CUI_CustomEffect::LateTick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();

	__super::LateTick(fTimeDelta);
}

void CUI_CustomEffect::Set_LerpAlpha(_float fStart, _float fTarget, _float fTime, EASING_TYPE eType)
{
	m_pEasingAlpha.lock()->Set_Lerp_once(fStart, fTarget, fTime, eType, true);
}

void CUI_CustomEffect::Set_LerpSize(_float2 fStart, _float2 fTarget, _float fTime, EASING_TYPE eType)
{
	m_pEasingSizing.lock()->Set_Lerp(fStart, fTarget, fTime, eType, CEasingComponent::ONCE, true, false);
}

void CUI_CustomEffect::Set_LerpTransform(_float2 fStart, _float2 fTarget, _float fTime, EASING_TYPE eType)
{
	m_pEasingTransform.lock()->Set_Lerp(fStart, fTarget, fTime, eType, CEasingComponent::ONCE, true, false);
}

void CUI_CustomEffect::Free()
{
}

