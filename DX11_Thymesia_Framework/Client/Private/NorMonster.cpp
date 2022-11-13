#include "stdafx.h"
#include "NorMonster.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "MobWeapon/MobWeapon.h"
#include "NorMonStateBase.h"
//#include "MonsterWeapon.h"
//#include "Monster1States/Monster1States.h"
#include "Client_Components.h"

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

	//ZeroMemory(&m_tLinkStateDesc, sizeof(STATE_LINK_DESC));
	memcpy(&m_tLinkStateDesc, pArg, sizeof(STATE_LINK_DESC));


	//m_tLinkStateDesc.eNorMonType = NORMONSTERTYPE::AXEMAN;
	//m_eNorMonType = NORMONSTERTYPE::AXEMAN;
	


	switch (m_tLinkStateDesc.eNorMonType)
	{
	case  NORMONSTERTYPE::AXEMAN:
		m_pModelCom.lock()->Init_Model("Mon_AxeMan", "", (_uint)TIMESCALE_LAYER::MONSTER);
		m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
		m_pWeapons.back().lock()->Set_WeaponType(MONSTERWEAPONTYPE::WEAPON_AXE);
		m_pWeapons.back().lock()->Init_DefaultWeapon(m_pModelCom, Weak_Cast<CGameObject>(m_this), "hand_r");
		m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1.f, 0.f, 1.f, 1.f));
		break;
		//나중에추가할거미리해둠
	case  NORMONSTERTYPE::KNIFEWOMAN:
		m_pModelCom.lock()->Init_Model("Mon_KnifeWoMan", "", (_uint)TIMESCALE_LAYER::MONSTER);
		m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
		m_pWeapons.back().lock()->Set_WeaponType(MONSTERWEAPONTYPE::WEAPON_KNIFE);
		m_pWeapons.back().lock()->Init_DefaultWeapon(m_pModelCom, Weak_Cast<CGameObject>(m_this), "hand_r");
		m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
		m_pWeapons.back().lock()->Set_WeaponType(MONSTERWEAPONTYPE::WEAPON_KNIFE);
		m_pWeapons.back().lock()->Init_DefaultWeapon(m_pModelCom, Weak_Cast<CGameObject>(m_this), "hand_l");
		m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(3.f, 0.f, 1.f, 1.f));
		break;
	case NORMONSTERTYPE::SKULL:
		break;
	case NORMONSTERTYPE::GARDENER:
		break;


	}



	m_pModelCom.lock()->Set_RootNode("root_$AssimpFbx$_Translation");


	//CStatus::STATUS_DESC StatusDesc;
	//StatusDesc.fMaxHP = StatusDesc.fCurrentHP = 360.f;
	//StatusDesc.szName = TEXT("유적 발굴가");
	//
	//m_pStatus.lock()->Init_Status(StatusDesc);
	//m_pStandState = Add_Component<CIdle>();
	m_pStandState = Add_Component<CNorMonState_Idle>(&m_tLinkStateDesc);
	/*Add_Component<CNorMonState_Awake>();
	Add_Component<CNorMonState_Die>();
	Add_Component<CNorMonState_DorMant>();
	Add_Component<CNorMonState_Farry>();
	Add_Component<CNorMonState_Fidget>();
	Add_Component<CNorMonState_GroggyEnd>();
	Add_Component<CNorMonState_GroggyLoop>();
	Add_Component<CNorMonState_GroggyStart>();
	Add_Component<CNorMonState_HeavyAttack1>();
	Add_Component<CNorMonState_HeavyAttack2>();
	Add_Component<CNorMonState_HeavyAttack3>();
	Add_Component<CNorMonState_HurtL>();
	Add_Component<CNorMonState_HurtR>();
	Add_Component<CNorMonState_LightAttack1>();
	Add_Component<CNorMonState_LightAttack2>();
	Add_Component<CNorMonState_LightAttack3>();
	Add_Component<CNorMonState_Run>();
	Add_Component<CNorMonState_SpecialAttack>();
	Add_Component<CNorMonState_TakeExecution>();
	Add_Component<CNorMonState_TurnL90>();
	Add_Component<CNorMonState_TurnR90>();
	Add_Component<CNorMonState_Walk_B>();
	Add_Component<CNorMonState_Walk_BL>();
	Add_Component<CNorMonState_Walk_BR>();
	Add_Component<CNorMonState_Walk_F>();
	Add_Component<CNorMonState_Walk_FL>();
	Add_Component<CNorMonState_Walk_FR>();
	Add_Component<CNorMonState_Walk_L>();
	Add_Component<CNorMonState_Walk_R>();*/
	


	//GET_SINGLE(CGameManager)->Bind_KeyEvent("Monster1", m_pModelCom, bind(&CNorMonster::Call_NextAnimationKey, this, placeholders::_1));

	USE_START(CNorMonster);
	return S_OK;
}

HRESULT CNorMonster::Start()
{
	__super::Start();

	switch (m_tLinkStateDesc.eNorMonIdleType)
	{
	case Client::NORMONSTERIDLETYPE::NORIDLE:
		Change_State<CNorMonState_Idle>();
		break;
	case Client::NORMONSTERIDLETYPE::GROGGYIDLE:
		// Change_State<CNorMonState_GroggyStart>();
		break;
	case Client::NORMONSTERIDLETYPE::DORMANT:
		// Change_State<CNorMonState_DorMant>();
		break;
	}

	

	//m_EffectIndexList.emplace_back("Character_Target", GET_SINGLE(CGameManager)->Use_EffectGroup("Character_Target", m_pTransformCom));

	return S_OK;
}

void CNorMonster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation");

	m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviMeshCom);

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

}

void CNorMonster::Respawn_Monster(_fvector In_vPosition)
{
	//__super::Respawn_Monster(In_vPosition);

	//Change_State<CMonster1State_Stand>();
	//GAMEINSTANCE->PlaySoundW(TEXT("MonsterBorn.wav"), 0.3f);
}

void CNorMonster::OnCollisionEnter(weak_ptr<CCollider> pOtherCollider)
{
	//__super::OnCollisionEnter(pOtherCollider);
}

void CNorMonster::OnCollisionStay(weak_ptr<CCollider> pOtherCollider)
{
	//__super::OnCollisionStay(pOtherCollider);
}

void CNorMonster::OnCollisionExit(weak_ptr<CCollider> pOtherCollider)
{
	//__super::OnCollisionExit(pOtherCollider);
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
