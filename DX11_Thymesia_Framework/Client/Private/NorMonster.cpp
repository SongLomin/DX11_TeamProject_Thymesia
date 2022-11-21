#include "stdafx.h"
#include "NorMonster.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "MobWeapon/MobWeapon.h"
#include "NorMonStateS.h"
#include "Status_Monster.h"
//#include "MonsterWeapon.h"
//#include "Monster1States/Monster1States.h"
#include "Client_Components.h"
#include "MonsterHPBar_Base.h"


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

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxAnimModel"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	memcpy(&m_tLinkStateDesc, pArg, sizeof(STATE_LINK_DESC));
	
	weak_ptr<CMonsterHPBar_Base> pHPBar = GAMEINSTANCE->Add_GameObject<CMonsterHPBar_Base>(LEVEL_STATIC);

	pHPBar.lock()->Set_Owner(Weak_Cast<CMonster>(m_this));

	switch (m_tLinkStateDesc.eMonType)
	{
	case  MONSTERTYPE::AXEMAN:
		m_pModelCom.lock()->Init_Model("Mon_AxeMan", "", (_uint)TIMESCALE_LAYER::MONSTER);
		m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
		m_pWeapons.back().lock()->Set_WeaponType(MONSTERWEAPONTYPE::WEAPON_AXE);
		m_pWeapons.back().lock()->Init_DefaultWeapon(m_pModelCom, Weak_Cast<CGameObject>(m_this), "weapon_r");
		//TODO 야매에요 ㅎ
		m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_tLinkStateDesc.vYame.x, 0.f, m_tLinkStateDesc.vYame.z, 1.f));
		break;
		//나중에추가할거미리해둠
	case  MONSTERTYPE::KNIFEWOMAN:
		m_pModelCom.lock()->Init_Model("Mon_KnifeWoMan", "", (_uint)TIMESCALE_LAYER::MONSTER);
		m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
		m_pWeapons.back().lock()->Set_WeaponType(MONSTERWEAPONTYPE::WEAPON_KNIFE);
		m_pWeapons.back().lock()->Init_DefaultWeapon(m_pModelCom, Weak_Cast<CGameObject>(m_this), "weapon_r");
		m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_tLinkStateDesc.vYame.x, 0.f, m_tLinkStateDesc.vYame.z, 1.f));
		break;
	case MONSTERTYPE::SKULL:
		break;
	case MONSTERTYPE::GARDENER:
		m_pModelCom.lock()->Init_Model("Mon_Gardner", "", (_uint)TIMESCALE_LAYER::MONSTER);
		m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
		m_pWeapons.back().lock()->Set_WeaponType(MONSTERWEAPONTYPE::WEAPON_SCYTHE);
		m_pWeapons.back().lock()->Init_DefaultWeapon(m_pModelCom, Weak_Cast<CGameObject>(m_this), "weapon_r");
		m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_tLinkStateDesc.vYame.x, 0.f, m_tLinkStateDesc.vYame.z, 1.f));
		break;
	}

		
	//TODO 여기서하는 이유는 몬스터가 배치되고 원점에서 우리가 피킹한위치만큼더해지고 난뒤에 그월드포지션값저장하기위해서 여기서함
		switch (m_tLinkStateDesc.eMonType)
		{
		case  MONSTERTYPE::AXEMAN:
			m_pModelCom.lock()->Set_RootNode("root");
			break;
		case  MONSTERTYPE::KNIFEWOMAN:
			m_pModelCom.lock()->Set_RootNode("root");
			break;
		case  MONSTERTYPE::GARDENER:
			m_pModelCom.lock()->Set_RootNode("root");
			break;
		}

	//StatusDesc.fMaxHP = StatusDesc.fCurrentHP = 360.f;
	//StatusDesc.szName = TEXT("유적 발굴가");
	//
	//m_pStatus.lock()->Init_Status(StatusDesc);
	//m_pStandState = Add_Component<CIdle>();
	
	_vector vecStartPositon = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	XMStoreFloat4(&m_tLinkStateDesc.m_fStartPositon, vecStartPositon);


	m_pStandState = Add_Component<CNorMonState_Idle>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_Stop>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_SitToIdle>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_LightAttack1>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_LightAttack2>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_Run>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_TurnL90>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_TurnR90>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_LightAttack1>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_LightAttack2>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_LightAttack3>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_HeavyAttack1>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_HeavyAttack2>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_HeavyAttack3>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_Walk_F>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_Walk_FR>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_Walk_FL>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_Walk_L>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_Walk_R>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_Walk_B>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_Walk_BR>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_Walk_BL>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_Awake>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_HurtL>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_HurtR>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_GroggyStart>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_GroggyLoop>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_GroggyEnd>(&m_tLinkStateDesc);
	Add_Component<CNorMonState_Die>(&m_tLinkStateDesc);
	

	GET_SINGLE(CGameManager)->Bind_KeyEvent(m_pStatus.lock()->Get_Desc().m_szModelKey, m_pModelCom, bind(&CNorMonster::Call_NextAnimationKey, this, placeholders::_1));

	USE_START(CNorMonster);
	return S_OK;
}

HRESULT CNorMonster::Start()
{
	__super::Start();

	
	Change_State<CNorMonState_Idle>();


	//m_EffectIndexList.emplace_back("Character_Target", GET_SINGLE(CGameManager)->Use_EffectGroup("Character_Target", m_pTransformCom));

	return S_OK;
}

void CNorMonster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	switch (m_tLinkStateDesc.eMonType)
	{
	case  MONSTERTYPE::AXEMAN:
		_vector vMoveDirs = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		vMoveDirs = m_pModelCom.lock()->Get_DeltaBonePosition("root");
		m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDirs, m_pNaviMeshCom);
		break;
	case  MONSTERTYPE::KNIFEWOMAN:	
		_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root", true, XMMatrixRotationX(XMConvertToRadians(-90.f)));
		m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviMeshCom);
		break;
	case  MONSTERTYPE::GARDENER:
		_vector vMoveDired = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		vMoveDired = m_pModelCom.lock()->Get_DeltaBonePosition("root");
		m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDired, m_pNaviMeshCom);
		break;
	}

}

void CNorMonster::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CNorMonster::Render()
{
	__super::Render();

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
			iPassIndex = 4;
		}

		//m_pShaderCom.lock()->Begin(m_iPassIndex);

		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, iPassIndex, "g_Bones");
		//m_pModelCom.lock()->Render_Mesh(i);
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

void CNorMonster::Respawn_Monster(_fvector In_vPosition)
{
	//__super::Respawn_Monster(In_vPosition);

	//Change_State<CMonster1State_Stand>();
	//GAMEINSTANCE->PlaySoundW(TEXT("MonsterBorn.wav"), 0.3f);
}

void CNorMonster::OnCollisionEnter(weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pOtherCollider);
}

void CNorMonster::OnCollisionStay(weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pOtherCollider);
}

void CNorMonster::OnCollisionExit(weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pOtherCollider);
}

void CNorMonster::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);


}

void CNorMonster::OnDisable()
{
	__super::OnDisable();
}



void CNorMonster::Free()
{
}
