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

	m_pVIBuffer = Add_Component<CVIBuffer_Trail>(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_EffectMesh"),
		VTXTEX_DECLARATION::Element, 
		VTXTEX_DECLARATION::iNumElements);
	
	m_pTextureCom.lock()->Use_Texture(("UVColorDiffuse"));

	m_pNoiseTextureCom = Add_Component<CTexture>();
	m_pNoiseTextureCom.lock()->Use_Texture(("UVNoise"));

	m_pMaskTextureCom = Add_Component<CTexture>();
	m_pMaskTextureCom.lock()->Use_Texture("UVMask");

	return S_OK;
}

void CEffect_Trail::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//m_pVIBuffer.lock()->Update(fTimeDelta, m_pOwnerTransform, m_pOwnerBoneNode, m_pOwnerModel_Data);
}

void CEffect_Trail::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CEffect_Trail::Render(ID3D11DeviceContext* pDeviceContext)
{

	SetUp_ShaderResource();

	__super::Render(pDeviceContext);
	m_pShaderCom.lock()->Begin(0, pDeviceContext);
	m_pVIBuffer.lock()->Render(pDeviceContext);

	return S_OK;
}
void CEffect_Trail::Update(_float fTimeDelta, weak_ptr <CTransform> _pOwnerTransform, weak_ptr<CBoneNode> _pOwnerBoneNode, weak_ptr<MODEL_DATA> _pOwnerModel_Data)
{
	if (!Get_Enable())
		return;

	m_pVIBuffer.lock()->Update(fTimeDelta, _pOwnerTransform, _pOwnerBoneNode, _pOwnerModel_Data);
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

	m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_iDiffuseIndex);

	m_pNoiseTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_NoiseTexture", m_iNoiseIndex);
	m_pMaskTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", m_iMaskIndex);

}



void CEffect_Trail::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

	
}

void CEffect_Trail::OnDisable()
{
	__super::OnDisable();

	

	m_pVIBuffer.lock()->Reset_Points(m_pOwnerTransform, m_pOwnerBoneNode, m_pOwnerModel_Data);
}


void CEffect_Trail::Free()
{
}