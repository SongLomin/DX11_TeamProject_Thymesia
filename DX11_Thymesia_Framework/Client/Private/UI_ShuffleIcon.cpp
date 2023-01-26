#include "stdafx.h"
#include "UI_ShuffleIcon.h"
#include "Item.h"
#include "Preset_UIDesc.h"
#include "CustomUI.h"
#include "Preset_ItemData.h"
#include "UI_Utils.h"
#include "EasingComponent_Transform.h"
#include "UI_ItemSlot.h"
#include "EasingComponent_Float.h"
#include "EasingComponent_Bezier.h"
GAMECLASS_C(CUI_ShuffleIcon)
CLONE_C(CUI_ShuffleIcon, CGameObject)

HRESULT CUI_ShuffleIcon::Initialize_Prototype()
{
	__super::Initialize_Prototype();



	return S_OK;
}

HRESULT CUI_ShuffleIcon::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;
	Set_Texture("None");
	
	SetUp_Component();

	return S_OK;
}

HRESULT CUI_ShuffleIcon::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_ShuffleIcon::Tick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();

	__super::Tick(fTimeDelta);

	if (m_pEasingTransform.lock()->Is_Lerping())
	{
		_float2	fPos = m_pEasingTransform.lock()->Get_Lerp();

		Set_UIPosition(fPos.x, fPos.y);
	}

	if (m_pEasingBezier.lock()->Is_Lerping())
	{
		_float2	fPos = m_pEasingBezier.lock()->Get_Lerp();

		Set_UIPosition(fPos.x, fPos.y);
	}

	if (m_pEasingScale.lock()->Is_Lerping())
	{
		_float fLerpedValue = m_pEasingScale.lock()->Get_Lerp();
		Set_Size(fLerpedValue, fLerpedValue);
	}

}

void CUI_ShuffleIcon::LateTick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();

	__super::LateTick(fTimeDelta);

}

void CUI_ShuffleIcon::Bind_Item(weak_ptr<CItem> pItem)
{
	m_pBindItem = pItem;

	CPreset_ItemData::SetUITextureFromItemName(Weak_StaticCast<CUI>(m_this), pItem.lock()->Get_Name());
}

void CUI_ShuffleIcon::Unbind_Item()
{
	m_pBindItem = weak_ptr<CItem>();

	Set_Texture("None");

	Set_Enable(false);
}

void CUI_ShuffleIcon::Bind_Target(weak_ptr<CUI_ItemSlot> pItemSlot)
{
	m_pTargetItemSlot = pItemSlot;
}

void CUI_ShuffleIcon::Pull_Lerp()
{
	_float2 PullPos = GetPos();

	PullPos.x -= 97.f;

	m_pEasingTransform.lock()->Set_Lerp(GetPos(),PullPos, 0.5f, EASING_TYPE::QUAD_IN,
		CEasingComponent::ONCE, true);
}

void CUI_ShuffleIcon::Start_Lerp(_float fLerpTime)
{
	m_pEasingTransform.lock()->Set_Lerp(GetPos(), m_pTargetItemSlot.lock()->GetPos(), fLerpTime, EASING_TYPE::QUAD_IN,
		CEasingComponent::ONCE,true);

	m_pEasingTransform.lock()->Callback_LerpEnd +=
		bind(&CUI_ShuffleIcon::Call_TransformLerpEnd, this);
}

void CUI_ShuffleIcon::Start_SwapLerp(_float2 TargetPos, _float fWeight, _float fLerpTime)
{
	_float2	fCurve = CUI_Utils::Get_BezierCurve(GetPos(), TargetPos, fWeight);

	m_pEasingBezier.lock()->Set_Lerp_OneCurve(GetPos(), fCurve, TargetPos, fLerpTime, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE, false);
}

void CUI_ShuffleIcon::Call_TransformLerpEnd()
{
	m_pEasingScale.lock()->Set_Lerp(64.f, 84.f, 0.3f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE);
	m_pEasingScale.lock()->Callback_LerpEnd
		+= bind(&CUI_ShuffleIcon::Call_ScaleUpEnd, this);

}

void CUI_ShuffleIcon::Call_ScaleUpEnd()
{
	m_pEasingScale.lock()->Set_Lerp(84.f, 64.f, 0.3f, EASING_TYPE::QUAD_OUT, CEasingComponent::ONCE);
	m_pEasingScale.lock()->Callback_LerpEnd
		+= bind(&CUI_ShuffleIcon::Call_ScaleDownEnd, this);
}

void CUI_ShuffleIcon::Call_ScaleDownEnd()
{
	m_pTargetItemSlot.lock()->Bind_Item(m_pBindItem);
	m_pBindItem = weak_ptr<CItem>();
	Set_Enable(false);
}

void CUI_ShuffleIcon::SetUp_Component()
{
	m_pEasingTransform = Add_Component<CEasingComponent_Transform>();
	
	m_pEasingBezier = Add_Component<CEasingComponent_Bezier>();

	m_pEasingScale = Add_Component<CEasingComponent_Float>();

}

void CUI_ShuffleIcon::Free()
{
}
