#include "stdafx.h"
#include "Camera_Interior.h"

#include "GameManager.h"

GAMECLASS_C(CCamera_Interior)
CLONE_C(CCamera_Interior, CGameObject)

CCamera_Interior::CCamera_Interior(const CCamera_Interior& rhs)
{
	*this = rhs;
}

HRESULT CCamera_Interior::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Interior::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_iCameraSpeed = 7;
	Set_Enable(false);

	GET_SINGLE(CGameManager)->Registration_Camera("Camera_Interior", Weak_Cast<CCamera>(m_this));

	return S_OK;
}

HRESULT CCamera_Interior::Start()
{
	return S_OK;
}

void CCamera_Interior::Tick(_float fTimeDelta)
{
	ControlCamera(fTimeDelta);

	if (FAILED(CCamera::Bind_PipeLine()))
		return;
}

void CCamera_Interior::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera_Interior::Render(ID3D11DeviceContext* pDeviceContext)
{
	return S_OK;
}

void  CCamera_Interior::OnEventMessage(_uint iArg)
{
	switch ((EVENT_TYPE)iArg)
	{

	}
}

const void CCamera_Interior::ControlCamera(const _float& fTimeDelta)
{
	MoveCamera_KeyInput(fTimeDelta);
	MoveCamera_MouseInput(fTimeDelta);
}

const void CCamera_Interior::MoveCamera_KeyInput(const _float& fTimeDelta)
{
	if (KEY_INPUT(KEY::W, KEY_STATE::HOLD))
		m_pTransformCom.lock()->Go_Straight(fTimeDelta * m_iCameraSpeed);

	if (KEY_INPUT(KEY::S, KEY_STATE::HOLD))
		m_pTransformCom.lock()->Go_Backward(fTimeDelta * m_iCameraSpeed);

	if (KEY_INPUT(KEY::A, KEY_STATE::HOLD))
		m_pTransformCom.lock()->Go_Left(fTimeDelta * m_iCameraSpeed);

	if (KEY_INPUT(KEY::D, KEY_STATE::HOLD))
		m_pTransformCom.lock()->Go_Right(fTimeDelta * m_iCameraSpeed);
}

const void CCamera_Interior::MoveCamera_MouseInput(const _float& fTimeDelta)
{
	_long MouseMove(0);

	if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_X))
	{
		if (KEY_INPUT(KEY::RBUTTON, KEY_STATE::HOLD))
			m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * 0.1f);
	}

	if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
	{
		if (KEY_INPUT(KEY::RBUTTON, KEY_STATE::HOLD))
			m_pTransformCom.lock()->Turn(m_pTransformCom.lock()->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
	}
}

void CCamera_Interior::Free()
{
}
