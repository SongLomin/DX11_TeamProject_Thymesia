#include "stdafx.h"
#include "Effect_Point.h"
#include "Client_Components.h"

GAMECLASS_C(CEffect_Point)
CLONE_C(CEffect_Point, CGameObject)

HRESULT CEffect_Point::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	m_eRenderGroup = RENDERGROUP::RENDER_NONLIGHT;

	return S_OK;
}

HRESULT CEffect_Point::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	_uint iNumInstance = 5;
	m_pVIBuffer = Add_Component<CVIBuffer_Point_Instance>(&iNumInstance);

	m_pShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_VtxPointInstance"),
		VTXPOINT_INSTANCE_DECLARATION::Element, 
		VTXPOINT_INSTANCE_DECLARATION::iNumElements);


	//m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 10.f, 1.f));

	m_pTextureCom.lock()->Use_Texture(("Snow"));

	return S_OK;
}

void CEffect_Point::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pVIBuffer.lock()->Update(fTimeDelta * GAMEINSTANCE->Get_TimeScale(m_iTimeScaleLayerIndex));
}

void CEffect_Point::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CEffect_Point::Render(ID3D11DeviceContext* pDeviceContext)
{

	SetUp_ShaderResource();

	__super::Render(pDeviceContext);
	m_pShaderCom.lock()->Begin(0, pDeviceContext);
	m_pVIBuffer.lock()->Render(pDeviceContext);

	return S_OK;
}

void CEffect_Point::Init_EffectParticle(const _char* In_szTextureKey)
{
	m_pTextureCom.lock()->Use_Texture(In_szTextureKey);

}

void CEffect_Point::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

}

void CEffect_Point::Free()
{
}