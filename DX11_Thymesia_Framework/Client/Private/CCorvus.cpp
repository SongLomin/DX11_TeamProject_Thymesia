#include "stdafx.h"
#include "CCorvus.h"
#include "Client_Components.h"
#include "CorvusStates/CorvusStates.h"

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


	m_pModelCom.lock()->Set_RootNode("Bip001");

	
	m_pStandState = Add_Component<CCorvusState_Idle>();
	Add_Component<CCorvusState_Jogging>();
	Add_Component<CCorvusState_JoggingStart>();
	Add_Component<CCorvusState_JoggingStartEnd>();
	Add_Component<CCorvusState_Stop>();
	
	GET_SINGLE(CGameManager)->Set_CurrentPlayer(Weak_StaticCast<CPlayer>(m_this));


	return S_OK;
}

HRESULT CCorvus::Start()
{
	__super::Start();

	Change_State<CCorvusState_Idle>();

	GET_SINGLE(CGameManager)->Set_CurrentPlayer(Weak_StaticCast<CPlayer>(m_this));

	return S_OK;
}

void CCorvus::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvus::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	_vector vDebugMove = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	if (KEY_INPUT(KEY::UP, KEY_STATE::HOLD))
	{
		vDebugMove.m128_f32[2] = 10.f * fTimeDelta;
	}

	if (KEY_INPUT(KEY::DOWN, KEY_STATE::HOLD))
	{
		vDebugMove.m128_f32[2] = -10.f * fTimeDelta;
	}

	if (KEY_INPUT(KEY::LEFT, KEY_STATE::HOLD))
	{
		vDebugMove.m128_f32[0] = -10.f * fTimeDelta;
	}

	if (KEY_INPUT(KEY::RIGHT, KEY_STATE::HOLD))
	{
		vDebugMove.m128_f32[0] = 10.f * fTimeDelta;
	}

	m_pTransformCom.lock()->Add_Position(vDebugMove);


}

HRESULT CCorvus::Render()
{
	__super::Render();

	_int iPassIndex = 0;

	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
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
