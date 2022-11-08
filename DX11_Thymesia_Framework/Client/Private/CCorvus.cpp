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

	return S_OK;
}

void CCorvus::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	_vector vLookDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	_vector vLook = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2];
	vLook.m128_f32[1] = 0.f;
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook));
	vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

	if (GetKeyState('W') & 0x80)
	{
		vLookDir += vLook;
	}
	if (GetKeyState('A') & 0x80)
	{
		vLookDir -= vRight;
	}
	if (GetKeyState('D') & 0x80)
	{
		vLookDir += vRight;
	}
	if (GetKeyState('S') & 0x80)
	{
		vLookDir -= vLook;
	}

	vLookDir = XMVector3Normalize(vLookDir);

	if (XMVectorGetX(XMVector3Length(vLookDir)) > DBL_EPSILON)
	{
		_vector vPlayerLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
		vPlayerLook = XMVector3Normalize(XMVectorLerp(vPlayerLook, vLookDir, 0.5f));


		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		vRight = XMVector3Cross(vUp, vPlayerLook);

		m_pTransformCom.lock()->Set_State(CTransform::STATE_LOOK, vPlayerLook);
		m_pTransformCom.lock()->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransformCom.lock()->Set_State(CTransform::STATE_UP, vUp);

		_float TimeDelta = fTimeDelta;

		if (GetKeyState(VK_SHIFT) & 0x80)
			TimeDelta *= 2.f;

		m_pTransformCom.lock()->Go_Straight(TimeDelta);
	}
}

void CCorvus::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

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
