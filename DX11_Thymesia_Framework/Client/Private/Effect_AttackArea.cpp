#include "../Default/stdafx.h"
#include "Effect_AttackArea.h"
#include "Collider.h"
#include "EffectGroup.h"
#include "CustomEffectMesh.h"
#include "SMath.h"
#include "GameManager.h"

GAMECLASS_C(CEffect_AttackArea);
CLONE_C(CEffect_AttackArea, CGameObject);

void CEffect_AttackArea::Init_EffectAttackArea(const EFFECT_ATTACKAREA_DESC& In_Desc, weak_ptr<CEffectGroup> pEffectGroup, weak_ptr<CCustomEffectMesh> pEffectMesh)
{
	m_Desc = In_Desc;
	m_pEffectGroup = pEffectGroup;
	m_pEffectMesh = pEffectMesh;
}

HRESULT CEffect_AttackArea::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_AttackArea::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

HRESULT CEffect_AttackArea::Start()
{
	__super::Start();

	return S_OK;
}

void CEffect_AttackArea::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (Check_AllDisableCollider())
		return;

	_float fTimeScale = GAMEINSTANCE->Get_TimeScale(m_tWeaponDesc.iTimeScaleLayer);

	if (m_fLifeTime > 0.f)
	{
		m_fLifeTime -= fTimeDelta * fTimeScale;
	}

	else
	{
		Disable_Weapon();
		return;
	}

	if (m_tWeaponDesc.fHitFreq > 0.f)
	{
		if (m_fCurrentFreq > 0.f)
		{
			m_fCurrentFreq -= fTimeDelta * fTimeScale;
		}

		else
		{
			// 갱신 시간이 되면 갱신한다.

			m_fCurrentFreq = m_tWeaponDesc.fHitFreq;

			Disable_Weapon();
			Enable_Weapon(m_fLifeTime);
		}
	}
}

void CEffect_AttackArea::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CEffect_AttackArea::Before_Render(_float fTimeDelta)
{
	__super::Before_Render(fTimeDelta);

	if (Check_AllDisableCollider())
		return;

	//부모 게임 오브젝트가 없음.
	if (!m_pParentTransformCom.lock())
	{
		return;
	}

	if (((_flag)ATTACKAREA_FLAG::FOLLOW_TRANSFORM) & m_Desc.AttackAreaFlag)
	{
		Update_TransformWithParent();

	}
	else if (((_flag)ATTACKAREA_FLAG::FOLLOW_EFFECTMESH) & m_Desc.AttackAreaFlag)
	{
		Update_TransformWithEffectMesh();
	}

} 

HRESULT CEffect_AttackArea::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);
	return S_OK;
}

void CEffect_AttackArea::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	if (m_bFirstAttack)
	{
		if (((_flag)ATTACKAREA_FLAG::ONCOLLISION_EFFECTGROUP) & m_Desc.AttackAreaFlag)
		{
			GET_SINGLE(CGameManager)->Use_EffectGroup(m_Desc.szOnCollisionEffectGroupName, m_pParentTransformCom, m_tWeaponDesc.iTimeScaleLayer);
		}

		if (((_flag)ATTACKAREA_FLAG::DISABLE_EFFECTGROUP) & m_Desc.AttackAreaFlag)
		{
			m_pEffectGroup.lock()->UnUse_EffectGroup();
		}
	}

	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CEffect_AttackArea::Update_TransformWithEffectMesh()
{
	if (!m_pEffectMesh.lock())
	{
		DEBUG_ASSERT;
	}

	m_pTransformCom.lock()->Set_WorldMatrix(SMath::Get_MatrixNormalize(m_pEffectMesh.lock()->Get_EffectWorldMatrix()));

	m_pTransformCom.lock()->Go_Right(m_tWeaponDesc.vWeaponOffset.x);
	m_pTransformCom.lock()->Go_Up(m_tWeaponDesc.vWeaponOffset.y);
	m_pTransformCom.lock()->Go_Straight(m_tWeaponDesc.vWeaponOffset.z);

	m_pHitColliderComs.front().lock()->Update(m_pTransformCom.lock()->Get_UnScaledWorldMatrix());

	Print_Vector(m_pTransformCom.lock()->Get_Position());
}


void CEffect_AttackArea::Free()
{
}
