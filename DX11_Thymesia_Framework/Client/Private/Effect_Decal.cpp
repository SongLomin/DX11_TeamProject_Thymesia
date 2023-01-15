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


	m_eRenderGroup = RENDERGROUP::RENDER_DECAL;

	m_pTransformCom.lock()->Rotation(m_pTransformCom.lock()->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(-90.f));

	m_DecalDesc = *(DECAL_DESC*)pArg;

	m_pTransformCom.lock()->Set_Position(XMLoadFloat4(&m_DecalDesc.vPosition));

	_matrix WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix() * XMLoadFloat4x4(&m_DecalDesc.WorldMatrix);
	m_pTransformCom.lock()->Set_WorldMatrix(WorldMatrix);

	m_pTransformCom.lock()->Set_Scaled(m_DecalDesc.vScale);  
	m_pTextureCom.lock()->Use_Texture(m_DecalDesc.strTextureTag.c_str());



	m_fTimeAcc = m_DecalDesc.fDisapearTime;

	return S_OK;
}

void CEffect_Decal::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_DecalDesc.fAppearTime -= fTimeDelta;

	if(0.f>m_DecalDesc.fAppearTime)
		m_DecalDesc.fTime -= fTimeDelta;

}

void CEffect_Decal::LateTick(_float fTimeDelta)
{
	if (0.f < m_DecalDesc.fAppearTime)
		return;

	__super::LateTick(fTimeDelta);

	if (m_DecalDesc.fTime < 0.f)
		m_DecalDesc.fDisapearTime -= fTimeDelta;

	if (Check_IsDead())
	{
		Set_Dead();
		return;
	}
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

	_float fCamFar = GAMEINSTANCE->Get_CameraFar();

	ViewMatrix = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_VIEW);
	ProjMatrix = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_PROJ);

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, ViewMatrix)));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, ProjMatrix)));

	m_pShaderCom.lock()->Set_RawValue("g_InvViewMatrix", &ViewMatrixInv, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_InvProjMatrix", &ProjMatrixInv, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_fFar", &fCamFar, sizeof(_float));

	_matrix TempMatrix = XMMatrixTranspose(ViewMatrix);
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&TempMatrix, sizeof(_matrix));
	TempMatrix = XMMatrixTranspose(ProjMatrix);
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&TempMatrix, sizeof(_matrix));

	TempMatrix = XMMatrixTranspose(WorldMatrix);
	m_pShaderCom.lock()->Set_RawValue( "g_WorldMatrix", (void*)&TempMatrix,sizeof(_matrix));

	TempMatrix = XMMatrixTranspose(WorldInvMatrix);
	m_pShaderCom.lock()->Set_RawValue("g_InvWorldMatrix", (void*)&TempMatrix, sizeof(_matrix));

	m_pShaderCom.lock()->Set_RawValue("g_vColor", (void*)&m_DecalDesc.vColor, sizeof(_float3));

	_float fAlphaRatio = m_DecalDesc.fDisapearTime / m_fTimeAcc;

	m_pShaderCom.lock()->Set_RawValue("g_fAlphaValue", &fAlphaRatio, sizeof(_float));

	if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		DEBUG_ASSERT;
	if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_NormalTexture", 1)))
		DEBUG_ASSERT;
	if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_ORMTexture", 2)))
		DEBUG_ASSERT;
	if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_EmissiveTexture1", 3)))
		DEBUG_ASSERT;
	if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_EmissiveTexture2", 4)))
		DEBUG_ASSERT;

	m_pShaderCom.lock()->Set_ShaderResourceView("g_DepthTexture", GAMEINSTANCE->Get_RenderTarget_SRV(TEXT("Target_Depth")));
		
}

_bool CEffect_Decal::Check_IsDead()
{
	if (0.f > m_DecalDesc.fDisapearTime)
	{
		return true;
	}
	return false;
}

void CEffect_Decal::Free()
{
}
