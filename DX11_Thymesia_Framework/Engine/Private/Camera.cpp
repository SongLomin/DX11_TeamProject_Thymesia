#include "..\Public\Camera.h"
#include "GameInstance.h"
#include "Transform.h"

GAMECLASS_C(CCamera)

CCamera::CCamera(const CCamera & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	GAMEINSTANCE->Set_CameraFar(m_CameraDesc.fFar);

	//m_pTransformCom.lock()->Set_TransformDesc(&m_CameraDesc.TransformDesc);
	/*_vector		vPos = XMLoadFloat3(&m_CameraDesc.vEye);
	vPos = XMVectorSetW(vPos, 1.f);*/
	m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_CameraDesc.vEye));
	m_pTransformCom.lock()->LookAt(XMLoadFloat4(&m_CameraDesc.vAt));

	Set_OwnerForMyComponents();

	return S_OK;
}

HRESULT CCamera::Start()
{
	__super::Start();

	return S_OK;
}

void CCamera::Tick(_float fTimeDelta)
{
}

void CCamera::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera::Render(ID3D11DeviceContext* pDeviceContext)
{
	return S_OK;
}

void CCamera::Init_Camera(const CAMERADESC& CameraDesc)
{
	m_CameraDesc = CameraDesc;

	m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_CameraDesc.vEye));
	m_pTransformCom.lock()->LookAt(XMLoadFloat4(&m_CameraDesc.vAt));
}

HRESULT CCamera::Bind_PipeLine()
{
	_matrix		WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();
	GET_SINGLE(CPipeLine)->Set_Transform(CPipeLine::D3DTS_WORLD, WorldMatrix);

	_matrix		ViewMatrix = XMMatrixInverse(nullptr, WorldMatrix);
	GET_SINGLE(CPipeLine)->Set_Transform(CPipeLine::D3DTS_VIEW, ViewMatrix);

	_matrix		ProjMatrix = XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	GET_SINGLE(CPipeLine)->Set_Transform(CPipeLine::D3DTS_PROJ, ProjMatrix);

	//여기에 CPipeLine->Set_Far(값); 함수 추가할 것.
	return S_OK;
}

void CCamera::Free()
{


}
