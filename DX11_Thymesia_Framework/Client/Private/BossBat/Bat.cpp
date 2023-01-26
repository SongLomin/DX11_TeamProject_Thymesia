#include "stdafx.h"
#include "BossBat/Bat.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "MobWeapon.h"
#include "Client_Components.h"
#include "BossBat/BatStates.h"
#include "MonsterHPBar_Boss.h"
#include "Status_Monster.h"
#include "Status_Boss.h"
#include "MobWeapon.h"

GAMECLASS_C(CBat);
CLONE_C(CBat, CGameObject);

HRESULT CBat::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CBat::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	CollsionContent(14.f);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxAnimModel"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	m_pModelCom.lock()->Init_Model("Boss_Bat", "", (_uint)TIMESCALE_LAYER::MONSTER);

	m_pStandState = Add_Component<CBatBossState_Start>();
	Add_Component<CBatBossState_Idle>();
	Add_Component<CBatBossState_Car>();
	Add_Component<CBatBossState_Atk_L01_1>();
	Add_Component<CBatBossState_Atk_L01_2a>();
	Add_Component<CBatBossState_Atk_L01_2b>();
	Add_Component<CBatBossState_Atk_L01_3a>();
	Add_Component<CBatBossState_Atk_R01_1>();
	Add_Component<CBatBossState_Atk_R01_2a>();
	Add_Component<CBatBossState_Atk_R01_2b>();
	Add_Component<CBatBossState_Bite_1>();
	Add_Component<CBatBossState_Bite_2>();
	Add_Component<CBatBossState_Car>();
	Add_Component<CBatBossState_Charge>();
	Add_Component<CBatBossState_FTurnL>();
	Add_Component<CBatBossState_FTurnR>();
	Add_Component<CBatBossState_Hellscream>();
	Add_Component<CBatBossState_HurtXL_F>();
	Add_Component<CBatBossState_HurtXL_L>();
	Add_Component<CBatBossState_JumpSmash_Chest>();
	Add_Component<CBatBossState_JumpSmash_ForwardL>();
	Add_Component<CBatBossState_JumpSmash_SmarhL>();
	Add_Component<CBatBossState_SonicBoom>();
	Add_Component<CBatBossState_Sp>();
	Add_Component<CBatBossState_Start_Loop>();
	Add_Component<CBatBossState_Storm>();
	Add_Component<CBatBossState_Stun_End>();
	Add_Component<CBatBossState_Stun_Start>();
	Add_Component<CBatBossState_Stun_Loop>();
	Add_Component<CBatBossState_TakeExecution_End>();
	Add_Component<CBatBossState_TakeExecution_Start>();
	Add_Component<CBatBossState_TakeExecution_Loop>();
	Add_Component<CBatBossState_TurnL>();
	Add_Component<CBatBossState_TurnR>();
	Add_Component<CBatBossState_WalkF>();
	Add_Component<CBatBossState_SonicBullet>();
	Add_Component<CBatBossState_HellIdle>();
	Add_Component<CBatBossState_ChargeIdle>();
	Add_Component<CBatBossState_AttackIdle>();
	Add_Component<CBatBossState_BackJump>();

	m_fCullingRange = 999.f;

	USE_START(CBat);
	return S_OK;
}

HRESULT CBat::Start()
{
	__super::Start();


	m_pPhysXControllerCom.lock()->Enable_Gravity(false);
	CBase::Set_Enable(true);

	Change_State<CBatBossState_Start_Loop>();


	//m_EffectIndexList.emplace_back("Character_Target", GET_SINGLE(CGameManager)->Use_EffectGroup("Character_Target", m_pTransformCom));

	return S_OK;
}

void CBat::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


}

void CBat::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CBat::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		_flag BindTextureFlag = 0;
		_uint iPassIndex = 0;

		if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{
			BindTextureFlag |= (1 << aiTextureType_DIFFUSE);
		}

		if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			BindTextureFlag |= (1 << aiTextureType_NORMALS);
		}

		if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
		{
			BindTextureFlag |= (1 << aiTextureType_SPECULAR);
		}

		// DiffuseTexture	NO.
		if (!((1 << aiTextureType_DIFFUSE) & BindTextureFlag))
		{
			continue;
		}

		// NormalTexture	OK.
		// ORMTexture		OK.
		else if ((1 << aiTextureType_NORMALS) & BindTextureFlag &&
			(1 << aiTextureType_SPECULAR) & BindTextureFlag)
		{
			iPassIndex = 5;
		}

		// NormalTexture	OK.
		// ORMTexture		NO.
		else if ((1 << aiTextureType_NORMALS) & BindTextureFlag &&
			!((1 << aiTextureType_SPECULAR) & BindTextureFlag))
		{
			iPassIndex = 4;
		}

		// NormalTexture	NO.
		// ORMTexture		NO.
		else if (
			!((1 << aiTextureType_NORMALS) & BindTextureFlag) &&
			!((1 << aiTextureType_SPECULAR) & BindTextureFlag))
		{
			iPassIndex = 0;
		}

		if(m_iPassIndex > 0)
		{
			iPassIndex = m_iPassIndex;
			m_pShaderCom.lock()->Set_RawValue("g_fDissolveAmount", &m_fDissolveAmount, sizeof(_float));
		}

		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, iPassIndex, "g_Bones", pDeviceContext);
		//m_pModelCom.lock()->Render_Mesh(i);
	}

	return S_OK;
}

void CBat::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}

void CBat::CollsionContent(_float fScale)
{
	m_pHitColliderCom = Add_Component<CCollider>();

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));


	ColliderDesc.vScale = _float3(fScale, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
	ColliderDesc.iLayer = (_uint)COLLISION_LAYER::MONSTER;

	m_pHitColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
}

void CBat::Init_Desc()
{

	__super::Init_Desc();

	m_pModelCom.lock()->Init_Model("Boss_Bat", "", (_uint)TIMESCALE_LAYER::MONSTER);
	m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
	m_pWeapons.back().lock()->Init_Model("Boss_BatWeapon", TIMESCALE_LAYER::MONSTER);
	m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "hand_r");
	m_pWeapons.back().lock()->Add_Collider({ 1.f,0.0f,0.f,1.0f }, 4.5f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
	m_pWeapons.back().lock()->Init_Model("Boss_BatWeapon", TIMESCALE_LAYER::MONSTER);
	m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "hand_l");
	m_pWeapons.back().lock()->Add_Collider({ 1.f,0.0f,0.f,1.0f }, 4.5f, COLLISION_LAYER::MONSTER_ATTACK);



	//TODO 여기서하는 이유는 몬스터가 배치되고 원점에서 우리가 피킹한위치만큼더해지고 난뒤에 그월드포지션값저장하기위해서 여기서함

	m_pModelCom.lock()->Set_RootNode("root_$AssimpFbx$_Translation", (_byte)ROOTNODE_FLAG::X |(_byte)ROOTNODE_FLAG::Z);



	m_pPhysXControllerCom.lock()->Init_Controller(Preset::PhysXControllerDesc::BossBatSetting(m_pTransformCom),
		(_uint)PHYSX_COLLISION_LAYER::MONSTER);

	INIT_STATE(CBatBossState_Start);
	INIT_STATE(CBatBossState_Idle);
	INIT_STATE(CBatBossState_Car);
	INIT_STATE(CBatBossState_Atk_L01_1);
	INIT_STATE(CBatBossState_Atk_L01_2a);
	INIT_STATE(CBatBossState_Atk_L01_2b);
	INIT_STATE(CBatBossState_Atk_L01_3a);
	INIT_STATE(CBatBossState_Atk_R01_1);
	INIT_STATE(CBatBossState_Atk_R01_2a);
	INIT_STATE(CBatBossState_Atk_R01_2b);
	INIT_STATE(CBatBossState_Bite_1);
	INIT_STATE(CBatBossState_Bite_2);
	INIT_STATE(CBatBossState_Car);
	INIT_STATE(CBatBossState_Charge);
	INIT_STATE(CBatBossState_FTurnL);
	INIT_STATE(CBatBossState_FTurnR);
	INIT_STATE(CBatBossState_Hellscream);
	INIT_STATE(CBatBossState_HurtXL_F);
	INIT_STATE(CBatBossState_HurtXL_L);
	INIT_STATE(CBatBossState_JumpSmash_Chest);
	INIT_STATE(CBatBossState_JumpSmash_ForwardL);
	INIT_STATE(CBatBossState_JumpSmash_SmarhL);
	INIT_STATE(CBatBossState_SonicBoom);
	INIT_STATE(CBatBossState_Sp);
	INIT_STATE(CBatBossState_Start_Loop);
	INIT_STATE(CBatBossState_Storm);
	INIT_STATE(CBatBossState_Stun_End);
	INIT_STATE(CBatBossState_Stun_Start);
	INIT_STATE(CBatBossState_Stun_Loop);
	INIT_STATE(CBatBossState_TakeExecution_End);
	INIT_STATE(CBatBossState_TakeExecution_Start);
	INIT_STATE(CBatBossState_TakeExecution_Loop);
	INIT_STATE(CBatBossState_TurnL);
	INIT_STATE(CBatBossState_TurnR);
	INIT_STATE(CBatBossState_WalkF);
	INIT_STATE(CBatBossState_SonicBullet);
	INIT_STATE(CBatBossState_HellIdle);
	INIT_STATE(CBatBossState_AttackIdle);
	INIT_STATE(CBatBossState_ChargeIdle);
	INIT_STATE(CBatBossState_BackJump);

#ifdef _BAT_EFFECT_
	Bind_KeyEvent("Boss_Bat");
#endif // _BAT_EFFECT_
}

//void CBat::Move_RootMotion_Internal()
//{
//	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
//	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root", true, XMMatrixRotationX(XMConvertToRadians(-90.f)));
//
//	PxControllerFilters Filters;
//	m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
//}

void CBat::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CBat::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);

}

void CBat::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CBat::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_GROGGY == iArg)
	{
		Change_State<CBatBossState_Stun_Start>();
	}

	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		if (Weak_Cast<CStatus_Boss>(m_pStatus).lock()->Get_WhiteRatio() > 0.99f)
		{
			return;
		}

		PxControllerFilters Filters;
		m_pPhysXControllerCom.lock()->Set_Position(XMLoadFloat4(&m_tLinkStateDesc.m_fStartPositon), 0.f, Filters);

		Change_State<CBatBossState_Start_Loop>();
		Set_Enable(false);
		m_pPhysXControllerCom.lock()->Enable_Gravity(false);

		m_pStatus.lock()->Full_Recovery();

	}
}

void CBat::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);


}

void CBat::OnDisable()
{
	__super::OnDisable();
}



void CBat::Free()
{
}
