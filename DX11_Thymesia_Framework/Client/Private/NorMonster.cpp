#include "stdafx.h"
#include "NorMonster.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "MobWeapon.h"
#include "ActorDecor.h"
#include "NorMonStateS.h"
#include "Status_Monster.h"
//#include "MonsterWeapon.h"
//#include "Monster1States/Monster1States.h"
#include "Client_Components.h"
#include "MonsterHPBar_Base.h"
#include "VIBuffer_Trail.h"


GAMECLASS_C(CNorMonster);
CLONE_C(CNorMonster, CGameObject);

HRESULT CNorMonster::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CNorMonster::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pStatus = Add_Component<CStatus_Monster>(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxAnimModel"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	m_pStandState = Add_Component<CNorMonState_Idle>();
	Add_Component<CNorMonState_Stop>();
	Add_Component<CNorMonState_SitToIdle>();
	Add_Component<CNorMonState_Run>();
	Add_Component<CNorMonState_TurnL90>();
	Add_Component<CNorMonState_TurnR90>();
	Add_Component<CNorMonState_LightAttack1>();
	Add_Component<CNorMonState_LightAttack2>();
	Add_Component<CNorMonState_LightAttack3>();
	Add_Component<CNorMonState_HeavyAttack1>();
	Add_Component<CNorMonState_HeavyAttack2>();
	Add_Component<CNorMonState_HeavyAttack3>();
	Add_Component<CNorMonState_Walk_F>();
	Add_Component<CNorMonState_Walk_FR>();
	Add_Component<CNorMonState_Walk_FL>();
	Add_Component<CNorMonState_Walk_L>();
	Add_Component<CNorMonState_Walk_R>();
	Add_Component<CNorMonState_Walk_B>();
	Add_Component<CNorMonState_Walk_BR>();
	Add_Component<CNorMonState_Walk_BL>();
	Add_Component<CNorMonState_Awake>();
	Add_Component<CNorMonState_HurtL>();
	Add_Component<CNorMonState_HurtR>();
	Add_Component<CNorMonState_GroggyStart>();
	Add_Component<CNorMonState_GroggyLoop>();
	Add_Component<CNorMonState_GroggyEnd>();
	Add_Component<CNorMonState_Die>();
	Add_Component<CNorMonState_Parry>();

	return S_OK;
}

HRESULT CNorMonster::Start()
{
	__super::Start();
	Change_State<CNorMonState_Idle>();

	Bind_HPBar();

	return S_OK;
}

void CNorMonster::Init_Desc()
{
	__super::Init_Desc();

	switch (m_tLinkStateDesc.eMonType)
	{
	case  MONSTERTYPE::AXEMAN:
		m_pModelCom.lock()->Init_Model("Mon_AxeMan", "", (_uint)TIMESCALE_LAYER::MONSTER);
		m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
		m_pWeapons.back().lock()->Init_Model("Mon_Weapon_Axe", TIMESCALE_LAYER::MONSTER);
		m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");

		m_pWeapons.back().lock()->Add_Collider({ 0.51f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 0.61f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 0.71f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 0.81f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		break;

		//나중에추가할거미리해둠
	case  MONSTERTYPE::KNIFEWOMAN:
		m_pModelCom.lock()->Init_Model("Mon_KnifeWoman", "", (_uint)TIMESCALE_LAYER::MONSTER);
		m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
		m_pWeapons.back().lock()->Init_Model("Mon_Weapon_Knife", TIMESCALE_LAYER::MONSTER);
		m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");
		m_pWeapons.back().lock()->Add_Collider({ 0.51f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 0.61f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 0.71f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 0.81f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);

		break;
	case MONSTERTYPE::SKULL:
		break;
	case MONSTERTYPE::GARDENER:
		m_pModelCom.lock()->Init_Model("Mon_Gardner", "", (_uint)TIMESCALE_LAYER::MONSTER);
		m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
		m_pWeapons.back().lock()->Init_Model("Mon_Weapon_Scythe", TIMESCALE_LAYER::MONSTER);
		m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");
		m_pWeapons.back().lock()->Add_Collider({ 0.51f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 0.61f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 0.71f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 0.81f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 0.91f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 1.01f,0.f,0.2f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 1.11f,0.f,0.4f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 1.21f,0.f,0.6f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pActorDecor.push_back(GAMEINSTANCE->Add_GameObject<CActorDecor>(m_CreatedLevel));
		m_pActorDecor.back().lock()->Init_Model("Mon_Vain", TIMESCALE_LAYER::MONSTER);
		m_pActorDecor.back().lock()->Init_ActorDecor(m_pModelCom, m_pTransformCom, "spine_03");

		break;
	case MONSTERTYPE::ENHANCE_GARDENER:
		m_pModelCom.lock()->Init_Model("Mon_Gardner", "", (_uint)TIMESCALE_LAYER::MONSTER);
		m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
		m_pWeapons.back().lock()->Init_Model("Mon_Weapon_Scythe", TIMESCALE_LAYER::MONSTER);
		m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");
		m_pWeapons.back().lock()->Add_Collider({ 0.51f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 0.61f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 0.71f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 0.81f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 0.91f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 1.01f,0.f,0.2f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 1.11f,0.f,0.4f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Add_Collider({ 1.21f,0.f,0.6f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pActorDecor.push_back(GAMEINSTANCE->Add_GameObject<CActorDecor>(m_CreatedLevel));
		m_pActorDecor.back().lock()->Init_Model("Mon_Scarf", TIMESCALE_LAYER::MONSTER);
		m_pActorDecor.back().lock()->Init_ActorDecor(m_pModelCom, m_pTransformCom, "head");
		m_pActorDecor.push_back(GAMEINSTANCE->Add_GameObject<CActorDecor>(m_CreatedLevel));
		m_pActorDecor.back().lock()->Init_Model("Mon_Vain", TIMESCALE_LAYER::MONSTER);
		m_pActorDecor.back().lock()->Init_ActorDecor(m_pModelCom, m_pTransformCom, "spine_03");

		break;
	case MONSTERTYPE::SHIELDAXEMAN:
		m_pModelCom.lock()->Init_Model("Mon_AxeMan", "", (_uint)TIMESCALE_LAYER::MONSTER);
		m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
		m_pWeapons.back().lock()->Init_Model("Mon_Weapon_Axe", TIMESCALE_LAYER::MONSTER);
		m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");
		m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
		m_pWeapons.front().lock()->Add_Collider({0.51f,0.f,0.0f,1.f}, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.front().lock()->Add_Collider({ 0.61f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.front().lock()->Add_Collider({ 0.71f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.front().lock()->Add_Collider({ 0.81f,0.f,0.0f,1.f }, 0.3f, COLLISION_LAYER::MONSTER_ATTACK);
		m_pWeapons.back().lock()->Init_Model("Mon_Weapon_Shield", TIMESCALE_LAYER::MONSTER);
		m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_l");

		break;
	case MONSTERTYPE::BALLOON:
		m_pModelCom.lock()->Init_Model("Balloon", "", (_uint)TIMESCALE_LAYER::MONSTER);
		break;
	}


	//TODO 여기서하는 이유는 몬스터가 배치되고 원점에서 우리가 피킹한위치만큼더해지고 난뒤에 그월드포지션값저장하기위해서 여기서함
	switch (m_tLinkStateDesc.eMonType)
	{
	case  MONSTERTYPE::AXEMAN:
		m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Z);
		break;
	case  MONSTERTYPE::KNIFEWOMAN:
		m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Z);
		break;
	case  MONSTERTYPE::GARDENER:
		m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Z);
		break;
	case MONSTERTYPE::ENHANCE_GARDENER:
		m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Z);
		break;
	case MONSTERTYPE::SHIELDAXEMAN:
		m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Z);
		break;
	case MONSTERTYPE::BALLOON:
		m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Z);

	}
	_vector vecStartPositon = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	XMStoreFloat4(&m_tLinkStateDesc.m_fStartPositon, vecStartPositon);

	INIT_STATE(CNorMonState_Idle);
	INIT_STATE(CNorMonState_Stop);
	INIT_STATE(CNorMonState_SitToIdle);
	INIT_STATE(CNorMonState_LightAttack1);
	INIT_STATE(CNorMonState_LightAttack2);
	INIT_STATE(CNorMonState_LightAttack3);
	INIT_STATE(CNorMonState_Run);
	INIT_STATE(CNorMonState_TurnL90);
	INIT_STATE(CNorMonState_TurnR90);
	INIT_STATE(CNorMonState_HeavyAttack1);
	INIT_STATE(CNorMonState_HeavyAttack2);
	INIT_STATE(CNorMonState_HeavyAttack3);
	INIT_STATE(CNorMonState_Walk_F);
	INIT_STATE(CNorMonState_Walk_FR);
	INIT_STATE(CNorMonState_Walk_FL);
	INIT_STATE(CNorMonState_Walk_L);
	INIT_STATE(CNorMonState_Walk_R);
	INIT_STATE(CNorMonState_Walk_B);
	INIT_STATE(CNorMonState_Walk_BR);
	INIT_STATE(CNorMonState_Walk_BL);
	INIT_STATE(CNorMonState_Awake);
	INIT_STATE(CNorMonState_HurtL);
	INIT_STATE(CNorMonState_HurtR);
	INIT_STATE(CNorMonState_GroggyStart);
	INIT_STATE(CNorMonState_GroggyLoop);
	INIT_STATE(CNorMonState_GroggyEnd);
	INIT_STATE(CNorMonState_Die);
	INIT_STATE(CNorMonState_Parry);


	string	strModelKey = Weak_StaticCast<CStatus_Monster>(m_pStatus).lock()->Get_Desc().m_szModelKey;
	Bind_KeyEvent(strModelKey.c_str());

	m_pPhysXControllerCom.lock()->Init_Controller(Preset::PhysXControllerDesc::PlayerSetting(m_pTransformCom), (_uint)PHYSX_COLLISION_LAYER::MONSTER);
}

void CNorMonster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//TestCode
	//if (m_pHPBar.lock())
	//{
	//}

}

void CNorMonster::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CNorMonster::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	_int iPassIndex = 0;
	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();


	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (i == 2 || i == 3)
			continue;

		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			iPassIndex = 0;
		}
		else
		{
			if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture",i, aiTextureType_SPECULAR)))
			{
				iPassIndex = 4;
			}
			else
			{
				iPassIndex = 5;
			}
		}
		if (0 < m_iPassIndex)
			iPassIndex = m_iPassIndex;
		//m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);

		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, iPassIndex, "g_Bones", pDeviceContext);
		//m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

	return S_OK;
}

void CNorMonster::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

#ifndef _USE_THREAD_
	m_pModelCom.lock()->Update_BoneMatrices();
#endif // !_USE_THREAD_

}

void CNorMonster::Move_RootMotion_Internal()
{
	PxControllerFilters Filters = Filters;
	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	switch (m_tLinkStateDesc.eMonType)
	{
	case  MONSTERTYPE::AXEMAN:
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root");
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
		break;
	case  MONSTERTYPE::KNIFEWOMAN:
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root", true, XMMatrixRotationX(XMConvertToRadians(-90.f)));
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
		break;
	case  MONSTERTYPE::GARDENER:
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root", true, XMMatrixRotationX(XMConvertToRadians(-90.f)));
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
		break;
	case MONSTERTYPE::ENHANCE_GARDENER:
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root", true, XMMatrixRotationX(XMConvertToRadians(-90.f)));
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
		break;
	case  MONSTERTYPE::SHIELDAXEMAN:
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root");
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
		break;
	case MONSTERTYPE::BALLOON:
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root");
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
		break;
	}

}

void CNorMonster::Respawn_Monster(_fvector In_vPosition)
{

}

void Init_MonsterDesc(void* In_Arg)
{

}

void CNorMonster::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CNorMonster::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CNorMonster::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CNorMonster::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_GROGGY == iArg)
	{
		Change_State<CNorMonState_GroggyStart>();
	}

	if ((_uint)EVENT_TYPE::ON_ENTER_SECTION == iArg)
	{
		Set_Enable(true);
	}

	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		PxControllerFilters Filters;
		m_pPhysXControllerCom.lock()->Set_Position(XMLoadFloat4(&m_tLinkStateDesc.m_fStartPositon), 0.f, Filters);

		Change_State<CNorMonState_Idle>();
		Set_Enable(false);
	}
}

void CNorMonster::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);
}

void CNorMonster::OnDisable()
{
	__super::OnDisable();
}

void CNorMonster::OnDestroy()
{
	__super::OnDestroy();
}

void CNorMonster::Free()
{
}
