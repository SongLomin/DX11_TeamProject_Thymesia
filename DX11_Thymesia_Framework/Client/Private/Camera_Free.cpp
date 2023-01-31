#include "stdafx.h"
#include "Camera_Free.h"

GAMECLASS_C(CCamera_Free)
CLONE_C(CCamera_Free, CGameObject)

CCamera_Free::CCamera_Free(const CCamera_Free& rhs)
{
	*this = rhs;

}

HRESULT CCamera_Free::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_iCameraSpeed = 5;

#ifdef _DEBUG
	this->ReadyText();
#endif // _DEBUG
	return S_OK;
}

void CCamera_Free::Tick
(
	_float fTimeDelta
)
{
	if (GetFocus() != g_hWnd)
	{
		CCamera::Bind_PipeLine();
		return;
	}

	this->ControlCamera(fTimeDelta);

	if (FAILED(CCamera::Bind_PipeLine()))
		return;

#ifdef _DEBUG
	this->UpdateText();
#endif // _DEBUG
}

void CCamera_Free::LateTick
(
	_float fTimeDelta
)
{
#ifdef _DEBUG
	GAMEINSTANCE->Add_Text(_uint(FONT_INDEX::PRETENDARD), m_tCameraSpeedText);
#endif // _DEBUG
}

HRESULT CCamera_Free::Render(ID3D11DeviceContext* pDeviceContext)
{
	return S_OK;
}

void  CCamera_Free::OnEventMessage(_uint iArg)
{
	switch ((EVENT_TYPE)iArg)
	{
		case EVENT_TYPE::ON_EDITDRAW :
		{
			ImGui::InputInt("Speed", &m_iCameraSpeed);
			ImGui::InputFloat("Turn Ratio", &m_fCameraTurnRatio);
		}
		break;
	}
}

#ifdef _DEBUG
const void CCamera_Free::ReadyText()
{
	m_tCameraSpeedText.bAlways = false;
	m_tCameraSpeedText.bCenterAlign = true;
	m_tCameraSpeedText.fRotation = 0.f;

	std::wstring CameraText;
	CameraText = L"Camera Speed : ";
	CameraText += std::to_wstring(m_iCameraSpeed);
	m_tCameraSpeedText.szText = CameraText;

	m_tCameraSpeedText.vColor = { 1.f, 0.f, 0.f, 1.f };
	m_tCameraSpeedText.vPosition = { _float(g_iWinCX >> 1), _float(g_iWinCY) - 25.f };
	m_tCameraSpeedText.vScale = { 0.5f, 0.5f };
}

const void CCamera_Free::UpdateText()
{
	std::wstring CameraText;
	CameraText = L"Camera Speed : ";
	CameraText += std::to_wstring(m_iCameraSpeed);
	m_tCameraSpeedText.szText = CameraText;
}
#endif // _DEBUG

const void CCamera_Free::ControlCamera
(
	const _float& fTimeDelta
)
{
	ChangeCameraSpeed();

	if (KEY_INPUT(KEY::RBUTTON, KEY_STATE::HOLD))
	{
		MoveCamera_KeyInput(fTimeDelta);
		MoveCamera_MouseInput(fTimeDelta);
	}
}

const void CCamera_Free::MoveCamera_KeyInput
(
	const _float& fTimeDelta
)
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

const void CCamera_Free::MoveCamera_MouseInput
(
	const _float& fTimeDelta
)
{
	_long MouseMove(0);

	if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_X))
	{
		m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * m_fCameraTurnRatio);
	}

	if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
	{
		m_pTransformCom.lock()->Turn(m_pTransformCom.lock()->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * m_fCameraTurnRatio);
	}

}

const void CCamera_Free::ChangeCameraSpeed()
{
	if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
	{
		if (KEY_INPUT(KEY::NUM1, KEY_STATE::TAP))
			m_iCameraSpeed = 1;
		if (KEY_INPUT(KEY::NUM2, KEY_STATE::TAP))
			m_iCameraSpeed = 2;
		if (KEY_INPUT(KEY::NUM3, KEY_STATE::TAP))
			m_iCameraSpeed = 3;
		if (KEY_INPUT(KEY::NUM4, KEY_STATE::TAP))
			m_iCameraSpeed = 4;
		if (KEY_INPUT(KEY::NUM5, KEY_STATE::TAP))
			m_iCameraSpeed = 5;
		if (KEY_INPUT(KEY::NUM6, KEY_STATE::TAP))
			m_iCameraSpeed = 6;
		if (KEY_INPUT(KEY::NUM7, KEY_STATE::TAP))
			m_iCameraSpeed = 7;
		if (KEY_INPUT(KEY::NUM8, KEY_STATE::TAP))
			m_iCameraSpeed = 8;
		if (KEY_INPUT(KEY::NUM9, KEY_STATE::TAP))
			m_iCameraSpeed = 9;
		if (KEY_INPUT(KEY::NUM0, KEY_STATE::TAP))
			m_iCameraSpeed = 10;
	}
}

void CCamera_Free::Free()
{
}
