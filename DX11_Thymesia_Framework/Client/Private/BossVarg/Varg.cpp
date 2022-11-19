#include "stdafx.h"
#include "BossVarg/Varg.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "MobWeapon/MobWeapon.h"
#include "VargStates.h"
//#include "MonsterWeapon.h"
//#include "Monster1States/Monster1States.h"
#include "Client_Components.h"

GAMECLASS_C(CVarg);
CLONE_C(CVarg, CGameObject);

HRESULT CVarg::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CVarg::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxAnimModel"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	memcpy(&m_tLinkStateDesc, pArg, sizeof(STATE_LINK_DESC));



	m_pModelCom.lock()->Init_Model("Boss_Varg", "", (_uint)TIMESCALE_LAYER::MONSTER);
	m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
	m_pWeapons.back().lock()->Set_WeaponType(MONSTERWEAPONTYPE::WEAPON_BOSSVARG);
	m_pWeapons.back().lock()->Init_DefaultWeapon(m_pModelCom, Weak_Cast<CGameObject>(m_this), "weapon_r");
	//TODO 위치이동 야매임
	m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_tLinkStateDesc.vYame.x, 0.f, m_tLinkStateDesc.vYame.z, 1.f));



	//TODO 여기서하는 이유는 몬스터가 배치되고 원점에서 우리가 피킹한위치만큼더해지고 난뒤에 그월드포지션값저장하기위해서 여기서함

	m_pModelCom.lock()->Set_RootNode("root");



	_vector vecStartPositon = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	XMStoreFloat4(&m_tLinkStateDesc.m_fStartPositon, vecStartPositon);
	m_pStandState = Add_Component<CVargBossState_Idle>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_Stop>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_SitToIdle>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_LightAttack1>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_LightAttack2>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_Run>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_TurnL90>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_TurnR90>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_LightAttack1>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_LightAttack2>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_LightAttack3>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_HeavyAttack1>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_HeavyAttack2>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_HeavyAttack3>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_Walk_F>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_Walk_FR>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_Walk_FL>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_Walk_L>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_Walk_R>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_Walk_B>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_Walk_BR>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_Walk_BL>(&m_tLinkStateDesc);
	//Add_Component<CNorMonState_Awake>(&m_tLinkStateDesc);




	//GET_SINGLE(CGameManager)->Bind_KeyEvent("Monster1", m_pModelCom, bind(&CVarg::Call_NextAnimationKey, this, placeholders::_1));

	USE_START(CVarg);
	return S_OK;
}

HRESULT CVarg::Start()
{
	__super::Start();


	Change_State<CVargBossState_Idle>();


	//m_EffectIndexList.emplace_back("Character_Target", GET_SINGLE(CGameManager)->Use_EffectGroup("Character_Target", m_pTransformCom));

	return S_OK;
}

void CVarg::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root");
	m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviMeshCom);


}

void CVarg::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CVarg::Render()
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

void CVarg::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

}

void CVarg::Respawn_Monster(_fvector In_vPosition)
{
	//__super::Respawn_Monster(In_vPosition);

	//Change_State<CMonster1State_Stand>();
	//GAMEINSTANCE->PlaySoundW(TEXT("MonsterBorn.wav"), 0.3f);
}

void CVarg::OnCollisionEnter(weak_ptr<CCollider> pOtherCollider)
{
	//__super::OnCollisionEnter(pOtherCollider);
}

void CVarg::OnCollisionStay(weak_ptr<CCollider> pOtherCollider)
{
	//__super::OnCollisionStay(pOtherCollider);
}

void CVarg::OnCollisionExit(weak_ptr<CCollider> pOtherCollider)
{
	//__super::OnCollisionExit(pOtherCollider);
}

void CVarg::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);


}

void CVarg::OnDisable()
{
	__super::OnDisable();
}



void CVarg::Free()
{
}
