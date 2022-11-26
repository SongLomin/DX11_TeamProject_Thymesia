#include "stdafx.h"
#include "BossUrd/Urd.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "MobWeapon.h"
#include "BossUrd/UrdStates.h"
//#include "MonsterWeapon.h"
//#include "Monster1States/Monster1States.h"
#include "Client_Components.h"

GAMECLASS_C(CUrd);
CLONE_C(CUrd, CGameObject);

HRESULT CUrd::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUrd::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxAnimModel"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	memcpy(&m_tLinkStateDesc, pArg, sizeof(STATE_LINK_MONSTER_DESC));

	m_pModelCom.lock()->Init_Model("Boss_Urd", "", (_uint)TIMESCALE_LAYER::MONSTER);
	m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
	m_pWeapons.back().lock()->Init_Model("Boss_UrdWeapon", TIMESCALE_LAYER::MONSTER);
	m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");
	//TODO 위치이동 야매임
	m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_tLinkStateDesc.vYame.x, m_tLinkStateDesc.vYame.y, m_tLinkStateDesc.vYame.z, 1.f));
	m_pTransformCom.lock()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-140.f));
	//TODO 여기서하는 이유는 몬스터가 배치되고 원점에서 우리가 피킹한위치만큼더해지고 난뒤에 그월드포지션값저장하기위해서 여기서함

	m_pModelCom.lock()->Set_RootNode("root");

	m_pStandState = Add_Component<CUrdBossState_Start>(&m_tLinkStateDesc);
	Add_Component<CUrdBossState_Idle>(&m_tLinkStateDesc);



	//GET_SINGLE(CGameManager)->Bind_KeyEvent("Monster1", m_pModelCom, bind(&CUrd::Call_NextAnimationKey, this, placeholders::_1));

	m_pPhysXControllerCom.lock()->Init_Controller(Preset::PhysXControllerDesc::PlayerSetting(m_pTransformCom));

	m_fCullingRange = 999.f;

	USE_START(CUrd);
	return S_OK;
}

HRESULT CUrd::Start()
{
	__super::Start();


	Change_State<CUrdBossState_Start>();


	//m_EffectIndexList.emplace_back("Character_Target", GET_SINGLE(CGameManager)->Use_EffectGroup("Character_Target", m_pTransformCom));

	return S_OK;
}

void CUrd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root", true, XMMatrixRotationX(XMConvertToRadians(-90.f)));
	m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, PxControllerFilters(), nullptr, m_pTransformCom);


}

void CUrd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUrd::Render()
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

void CUrd::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

}



void CUrd::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	//__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CUrd::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	//__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CUrd::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	//__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CUrd::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);


}

void CUrd::OnDisable()
{
	__super::OnDisable();
}



void CUrd::Free()
{
}
