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


	return S_OK;
}

void CCamera_Free::Tick(_float fTimeDelta)
{
	if (GetFocus() != g_hWnd)
	{
		Bind_PipeLine();
		return;
	}
		

	if (KEY_INPUT(KEY::W, KEY_STATE::HOLD))
	{
		m_pTransformCom.lock()->Go_Straight(fTimeDelta * 15.f);
	}

	if (KEY_INPUT(KEY::S, KEY_STATE::HOLD))
	{
		m_pTransformCom.lock()->Go_Backward(fTimeDelta * 15.f);
	}

	if (KEY_INPUT(KEY::A, KEY_STATE::HOLD))
	{
		m_pTransformCom.lock()->Go_Left(fTimeDelta * 15.f);
	}

	if (KEY_INPUT(KEY::D, KEY_STATE::HOLD))
	{
		m_pTransformCom.lock()->Go_Right(fTimeDelta * 15.f);
	}

	_long		MouseMove = 0;

	if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_X))
	{
		if (KEY_INPUT(KEY::RBUTTON, KEY_STATE::HOLD))
		{
			m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * 0.1f);

		}

	}

	if (MouseMove = GAMEINSTANCE->Get_DIMouseMoveState(MMS_Y))
	{
		if (KEY_INPUT(KEY::RBUTTON, KEY_STATE::HOLD))
		{
			m_pTransformCom.lock()->Turn(m_pTransformCom.lock()->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
		}
	}


	if (FAILED(Bind_PipeLine()))
		return;

}

void CCamera_Free::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

void CCamera_Free::Free()
{
}
