#include "stdafx.h"
#include "CCorvus.h"
#include "Client_Components.h"
#include "CorvusStates/CorvusStates.h"
#include "Camera_Target.h"
#include "Corvus_DefaultSaber.h"
#include "Corvus_DefaultDagger.h"
#include "PhysXCollider.h"

GAMECLASS_C(CCorvus)
CLONE_C(CCorvus, CGameObject)

HRESULT CCorvus::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CCorvus::Initialize(void* pArg)
{

	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxAnimModel"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	m_pModelCom.lock()->Init_Model("Corvus", "", (_uint)TIMESCALE_LAYER::PLAYER);


	m_pModelCom.lock()->Set_RootNode("root_$AssimpFbx$_Translation");

	m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CCorvus_DefaultSaber>(m_CreatedLevel));
	m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, Weak_Cast<CGameObject>(m_this), "weapon_r");
	m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CCorvus_DefaultDagger>(m_CreatedLevel));
	m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, Weak_Cast<CGameObject>(m_this), "weapon_l");

	m_pStandState = Add_Component<CCorvusState_Idle>();
	Add_Component<CCorvusState_Jogging>();
	Add_Component<CCorvusState_JoggingStart>();
	Add_Component<CCorvusState_JoggingStartEnd>();
	Add_Component<CCorvusState_Run>();
	Add_Component<CCorvusState_Sprint>();
	Add_Component<CCorvusState_SprintStart>();
	Add_Component<CCorvusState_AVoid>();
	Add_Component<CVarg_Execution>();
	Add_Component<CCorvusState_SprintAttack>();
	Add_Component<CCorvusState_LAttack1>();
	Add_Component<CCorvusState_LAttack2>();
	Add_Component<CCorvusState_LAttack3>();
	Add_Component<CCorvusState_Parry1>();
	Add_Component<CCorvusState_Parry2>();
	Add_Component<CCorvusState_BasicHealing>();
	GET_SINGLE(CGameManager)->Set_CurrentPlayer(Weak_StaticCast<CPlayer>(m_this));

	GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_Cast<CGameObject>(m_this));

	// Key Frame Effect ON
	GET_SINGLE(CGameManager)->Bind_KeyEvent("Corvus", m_pModelCom, bind(&CCorvus::Call_NextAnimationKey, this, placeholders::_1));

	USE_START(CCorvus);

	return S_OK;
}

HRESULT CCorvus::Start()
{
	__super::Start();

	Change_State<CCorvusState_Idle>();
	
	m_pCamera = GET_SINGLE(CGameManager)->Get_TargetCamera();
	m_pCameraTransform = m_pCamera.lock()->Get_Component<CTransform>();


	return S_OK;
}

void CCorvus::Tick(_float fTimeDelta)
{
	m_pPhysXColliderCom.lock()->Synchronize_Transform(m_pTransformCom, XMVectorSet(0.f, -0.5f, 0.f, 1.f));

	__super::Tick(fTimeDelta);
	
	this->RootMove();
}

void CCorvus::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom, XMVectorSet(0.f, 0.5f, 0.f, 1.f));
}

HRESULT CCorvus::Render()
{
	__super::Render();

	_int iPassIndex = 0;

	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
	

		if (i == 4 || i == 9 || i == 12)
			continue;
			

		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{

		}

		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			iPassIndex = 0;
		}
		else
		{
			iPassIndex = 4;
		}

		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, iPassIndex, "g_Bones");
		
	}

	return S_OK;
}

void CCorvus::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	_vector vLightFlag = { 1.f, 0.f, 0.f, 0.f };

	m_pShaderCom.lock()->Set_RawValue("g_vLightFlag", &vLightFlag, sizeof(_vector));
}

void CCorvus::OnCollisionEnter(weak_ptr<CCollider> pOtherCollider)
{
}

void CCorvus::OnCollisionStay(weak_ptr<CCollider> pOtherCollider)
{
}

void CCorvus::OnCollisionExit(weak_ptr<CCollider> pOtherCollider)
{
}

void CCorvus::RootMove()
{
	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation");
	m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviMeshCom);
}

void CCorvus::OnBattleEnd()
{
}

void CCorvus::OnEnable(void* pArg)
{
}

void CCorvus::OnDisable()
{
}

void CCorvus::OnDestroy()
{
}

void CCorvus::Free()
{
}
