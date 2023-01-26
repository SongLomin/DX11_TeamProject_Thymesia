#include "stdafx.h"
#include "Camera_Static.h"
GAMECLASS_C(CCamera_Static)
CLONE_C(CCamera_Static, CGameObject)

CCamera_Static::CCamera_Static(const CCamera_Static& rhs)
{
	*this = rhs;

}

HRESULT CCamera_Static::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CCamera_Static::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

void CCamera_Static::Tick(_float fTimeDelta)
{
	/*if (GetFocus() != g_hWnd)
	{
		Bind_PipeLine();
		return;
	}*/


	if (FAILED(Bind_PipeLine()))
		return;

}

void CCamera_Static::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera_Static::Render(ID3D11DeviceContext* pDeviceContext)
{
	return S_OK;
}

void CCamera_Static::Free()
{
}
