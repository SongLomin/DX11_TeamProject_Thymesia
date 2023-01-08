#include "stdafx.h"
#include "Effect_Decal.h"
#include "Client_Components.h"
#include "VIBuffer_Cube.h"

GAMECLASS_C(CEffect_Decal)
CLONE_C(CEffect_Decal, CGameObject)

HRESULT CEffect_Decal::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CEffect_Decal::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pVIBuffer = Add_Component<CVIBuffer_Cube>();
	
	m_pShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_VtxDecal"),
		VTXCUBETEX_DECLARATION::Element, VTXCUBETEX_DECLARATION::iNumElements);

	m_pTextureCom.lock()->Use_Texture("DecalTexture");

	m_eRenderGroup = RENDERGROUP::RENDER_DECAL;

	m_pTransformCom.lock()->Rotation(m_pTransformCom.lock()->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(-90.f));

	memcpy(&m_DecalDesc, pArg, sizeof(DECAL_DESC));

	m_pTransformCom.lock()->Set_Position(XMLoadFloat4(&m_DecalDesc.vPosition));
	m_pTransformCom.lock()->Set_Scaled(m_DecalDesc.vScale);

	return S_OK;
}

void CEffect_Decal::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_DecalDesc.fTime -= fTimeDelta;
}

void CEffect_Decal::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (Check_IsDead())
	{
		Set_Dead();
		return;
	}
	m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Cast<CGameObject>(m_this));
}

HRESULT CEffect_Decal::Render(ID3D11DeviceContext* pDeviceContext)
{
	SetUp_ShaderResource();

	__super::Render(pDeviceContext);

	m_pShaderCom.lock()->Begin(0,pDeviceContext);
	m_pVIBuffer.lock()->Render(pDeviceContext);

	return S_OK;
}

void CEffect_Decal::SetUp_ShaderResource()
{
	CallBack_Bind_SRV(m_pShaderCom, "");

	_matrix WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();
	_matrix WorldInvMatrix = XMMatrixInverse(nullptr, WorldMatrix);

	_float4x4		ViewMatrixInv, ProjMatrixInv;
	_matrix		ViewMatrix, ProjMatrix;

	ViewMatrix = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_VIEW);
	ProjMatrix = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_PROJ);

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, ViewMatrix)));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, ProjMatrix)));

	m_pShaderCom.lock()->Set_RawValue("g_InvViewMatrix", &ViewMatrixInv, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_InvProjMatrix", &ProjMatrixInv, sizeof(_float4x4));

	_matrix TempMatrix = XMMatrixTranspose(ViewMatrix);
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&TempMatrix, sizeof(_matrix));
	TempMatrix = XMMatrixTranspose(ProjMatrix);
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&TempMatrix, sizeof(_matrix));

	TempMatrix = XMMatrixTranspose(WorldMatrix);
	m_pShaderCom.lock()->Set_RawValue( "g_WorldMatrix", (void*)&TempMatrix,sizeof(_matrix));

	TempMatrix = XMMatrixTranspose(WorldInvMatrix);
	m_pShaderCom.lock()->Set_RawValue("g_InvWorldMatrix", (void*)&TempMatrix, sizeof(_matrix));

	m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture",0);
	m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_NormalTexture",1);
	m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_ORMTexture",2);

	m_pShaderCom.lock()->Set_ShaderResourceView("g_DepthTexture", GAMEINSTANCE->Get_RenderTarget_SRV(TEXT("Target_Depth")));
		
}

_bool CEffect_Decal::Check_IsDead()
{
	if (0.f > m_DecalDesc.fTime)
	{
		return true;
	}
	return false;
}

void CEffect_Decal::Free()
{
}
