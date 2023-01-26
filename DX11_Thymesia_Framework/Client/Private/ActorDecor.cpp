#include "stdafx.h"
#include "ActorDecor.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Character.h"

GAMECLASS_C(CActorDecor);
CLONE_C(CActorDecor, CGameObject);

HRESULT CActorDecor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CActorDecor::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pModelCom = Add_Component<CModel>();
	m_pShaderCom = Add_Component<CShader>();
	m_pRendererCom = Add_Component<CRenderer>();

	m_vOffset = { 0.f, 0.f, 0.f };
	

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxModel"),
		VTXMODEL_DECLARATION::Element,
		VTXMODEL_DECLARATION::iNumElements);

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CActorDecor::Start()
{
	return S_OK;
}

void CActorDecor::Tick(_float fTimeDelta)
{
	//__super::Tick(fTimeDelta);

	//부모 게임 오브젝트가 없음.
	if (!m_pParentTransformCom.lock())
		return;

	_matrix		ParentMatrix 
		= XMLoadFloat4x4(&m_OffsetMatrix)
		* m_pTargetBoneNode.lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&m_TransformationMatrix);

	//무기 오프셋 나중에 캐릭터별로 매개변수로 받아서 처리하자.
	ParentMatrix = SMath::Go_Right(ParentMatrix, m_vOffset.x);
	ParentMatrix = SMath::Go_Up(ParentMatrix, m_vOffset.y);
	ParentMatrix = SMath::Go_Straight(ParentMatrix, m_vOffset.z);

	m_pTransformCom.lock()->Set_WorldMatrix
	(
		ParentMatrix *
		m_pParentTransformCom.lock()->Get_WorldMatrix()
	);	
}

void CActorDecor::LateTick(_float fTimeDelta)
{
	//__super::LateTick(fTimeDelta);

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONALPHABLEND, Cast<CGameObject>(m_this));
}

HRESULT CActorDecor::Render(ID3D11DeviceContext* pDeviceContext)
{
	SetUp_ShaderResource();

	__super::Render(pDeviceContext);

	_int iPassIndex;
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
			iPassIndex = 3;
		}

		m_pShaderCom.lock()->Begin(iPassIndex, pDeviceContext);

		m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

#ifdef _DEBUG


#endif // _DEBUG

	return S_OK;
}



void CActorDecor::Init_ActorDecor(weak_ptr<CModel> In_pModelCom, weak_ptr<CTransform> In_ParentTransformCom, const string& szTargetNode)
{
	m_pParentTransformCom = In_ParentTransformCom;
	m_pTargetBoneNode = In_pModelCom.lock()->Find_BoneNode(szTargetNode);
	m_TransformationMatrix = In_pModelCom.lock()->Get_TransformationMatrix();
}

void CActorDecor::Init_Model(const string& strWeaponName, TIMESCALE_LAYER eLayer)
{
	m_pModelCom.lock()->Init_Model(strWeaponName.c_str(), "", (_uint)eLayer);
}

weak_ptr<CGameObject> CActorDecor::Get_ParentObject()
{
	return m_pParent;
}

void CActorDecor::SetUp_ShaderResource()
{
	CallBack_Bind_SRV(m_pShaderCom, "");
	m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));
}

void CActorDecor::Free()
{
}