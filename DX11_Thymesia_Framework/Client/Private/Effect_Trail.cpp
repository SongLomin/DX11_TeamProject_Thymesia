#include "stdafx.h"
#include "Effect_Trail.h"
#include "VIBuffer_Trail.h"
#include "Client_Components.h"

GAMECLASS_C(CEffect_Trail)
CLONE_C(CEffect_Trail, CGameObject)

HRESULT CEffect_Trail::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	m_eRenderGroup = RENDERGROUP::RENDER_NONLIGHT;

	return S_OK;
}

HRESULT CEffect_Trail::Initialize(void* pArg)//trail을 사용하는 객체로부터 traildesc를 받는다.
{
	__super::Initialize(pArg);

	m_pVIBuffer = Add_Component<CVIBuffer_Trail>(&pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_VtxPointInstance"),
		VTXPOINT_INSTANCE_DECLARATION::Element, 
		VTXPOINT_INSTANCE_DECLARATION::iNumElements);


	//m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 10.f, 1.f));

	m_pTextureCom.lock()->Use_Texture(("Snow"));

	return S_OK;
}

void CEffect_Trail::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pVIBuffer.lock()->Update(fTimeDelta * GAMEINSTANCE->Get_TimeScale(m_iTimeScaleLayerIndex),m_pOwnerTransform, m_pOwnerBoneNode,m_pOwnerModel_Data);
}

void CEffect_Trail::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CEffect_Trail::Render()
{

	SetUp_ShaderResource();

	__super::Render();
	m_pShaderCom.lock()->Begin(0);
	m_pVIBuffer.lock()->Render();

	return S_OK;
}

void CEffect_Trail::Set_OwnerDesc(weak_ptr <CTransform> _pOwnerTransform, weak_ptr<CBoneNode> _pOwnerBoneNode, weak_ptr<MODEL_DATA> _pOwnerModel_Data)
{
	m_pOwnerTransform = _pOwnerTransform;
	m_pOwnerBoneNode = _pOwnerBoneNode;
	m_pOwnerModel_Data = _pOwnerModel_Data;
}

void CEffect_Trail::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

}

void CEffect_Trail::Free()
{
}