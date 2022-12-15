#include "stdafx.h"
#include "Terrain.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_Terrain.h"
#include "Navigation.h"
#include "PhysXCollider.h"

GAMECLASS_C(CTerrain)
CLONE_C(CTerrain, CGameObject)

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom = Add_Component<CShader>();
	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxNorTex"),
		VTXNORTEX_DECLARATION::Element, 
		VTXNORTEX_DECLARATION::iNumElements);
	m_pRendererCom = Add_Component<CRenderer>();
	m_pTextureCom = Add_Component<CTexture>();

	m_pTextureCom.lock()->Use_Texture(("TestTile"));
	m_pVIBufferCom = Add_Component<CVIBuffer_Terrain>((void*)TEXT("../Bin/Resources/Textures/Terrain/DefaultHeight.bmp"));
	//m_pVIBufferCom.lock()

	/*PHYSXCOLLIDERDESC tPhysxColliderDesc;
	tPhysxColliderDesc.eShape = PHYSXCOLLIDER_TYPE::BOX;
	tPhysxColliderDesc.eActorType = PHYSXACTOR_TYPE::STATIC;
	tPhysxColliderDesc.fDensity = 10.f;
	tPhysxColliderDesc.vAngles = { 0.f, 0.f, 0.f, 0.f };
	tPhysxColliderDesc.vPosition = { 0.f, 0.f, 0.f, 1.f };
	tPhysxColliderDesc.vScale = { 30.f, 1.f, 30.f };
	PxMaterial* pMaterial = nullptr;
	GAMEINSTANCE->Create_Material(0.f, 0.f, -100.f, &pMaterial);
	tPhysxColliderDesc.pMaterial = pMaterial;

	m_pPhyxXColliderCom = Add_Component<CPhysXCollider>(&tPhysxColliderDesc);*/

	Set_OwnerForMyComponents();

	return S_OK;
}

HRESULT CTerrain::Start()
{
	__super::Start();

	return S_OK;
}

void CTerrain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	// m_pPhyxXColliderCom.lock()->Synchronize_Transform(m_pTransformCom);
}

void CTerrain::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONALPHABLEND, Cast<CGameObject>(m_this));
}

HRESULT CTerrain::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	__super::Render(pDeviceContext);

	m_pShaderCom.lock()->Begin(0, pDeviceContext);

	m_pVIBufferCom.lock()->Render(pDeviceContext);

	return S_OK;
}

HRESULT CTerrain::SetUp_ShaderResource()
{
	CallBack_Bind_SRV(m_pShaderCom, "");

	/*if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;*/
	
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_SourDiffTexture", 0)))
		return E_FAIL;

	_vector vShaderFlag = { 0.f, 0.f, 0.f, 0.f };
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
		return E_FAIL;

	return S_OK;
}

void CTerrain::Free()
{
}
