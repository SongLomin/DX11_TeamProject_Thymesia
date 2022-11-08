#include "stdafx.h"
#include "Camera_Target.h"
#include "GameManager.h"
#include "Monster.h"
#include "Player.h"
#include "GameObject.h"

GAMECLASS_C(CCamera_Target);
CLONE_C(CCamera_Target, CGameObject);

CCamera_Target::CCamera_Target(const CCamera_Target& rhs)
{
	*this = rhs;
}

HRESULT CCamera_Target::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CCamera_Target::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pTransformCom.lock()->Add_Position(XMVectorSet(0.f, 2.f, 0.f, 1.f));

	m_vTargetPosition = _float3(0.f, 0.f, 0.f);
	m_vTargetQuaternion = _float4(0.f, 0.f, 0.f, 0.f);

	_matrix MatLookAtZeroPoint = XMMatrixIdentity();
	MatLookAtZeroPoint.r[3] = XMVectorSet(0.f, 3.5f, -3.5f, 1.f);
	MatLookAtZeroPoint = SMath::LookAt(MatLookAtZeroPoint, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	_vector vLookAtZeroPointQuat = XMQuaternionRotationMatrix(SMath::Get_RotationMatrix(MatLookAtZeroPoint));


	return S_OK;
}

HRESULT CCamera_Target::Start()
{
	m_pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	//현재 플레이어가 NULLPTR임.
	if (!m_pCurrentPlayer.lock().get())
		DEBUG_ASSERT;

	m_pCurrentPlayerTransformCom = m_pCurrentPlayer.lock()->Get_Component<CTransform>();


	return S_OK;
}

void CCamera_Target::Tick(_float fTimeDelta)
{
	if (GetFocus() != g_hWnd)
	{
		return;
	}

	if (!m_pCurrentPlayer.lock().get())
		return;


	if (m_bIsFocused)
	{
		if (!m_pTargetMonster.lock()|| m_pTargetMonster.lock()->Get_Dead())
		{
			return;
		}
		Bake_TargetCamera(fTimeDelta);
	}
	else
	{
		Interpolate_Camera(fTimeDelta);
	}


}

void CCamera_Target::LateTick(_float fTimeDelta)
{

	Bind_PipeLine();
}

HRESULT CCamera_Target::Render()
{
	return S_OK;
}

void CCamera_Target::Change_Target()
{
	/*추후에 시간 나면 타겟 바꾸는 기능 추가*/
}

void CCamera_Target::Focus_Monster(weak_ptr<CMonster> _pMonster)
{
	m_pTargetMonster = _pMonster;
	m_pTargetMonsterTransformCom = _pMonster.lock()->Get_Component<CTransform>();

	m_bIsFocused = true;
}

void CCamera_Target::Release_Focus()
{
	m_pTargetMonster = weak_ptr<CMonster>();
	m_pTargetMonsterTransformCom = weak_ptr<CTransform>();
	m_bIsFocused = false;
}


HRESULT CCamera_Target::Bind_PipeLine()
{
	GAMEINSTANCE->Set_Transform(CPipeLine::D3DTS_WORLD, m_pTransformCom.lock()->Get_WorldMatrix());

	//_matrix		WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();

	_matrix WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();

	/*_matrix		WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix()
		* XMLoadFloat4x4(&m_MatRotation)
		* SMath::Get_PositionMatrix(m_pCamTargetTransformCom.lock()->Get_WorldMatrix());
	GAMEINSTANCE->Set_Transform(CPipeLine::D3DTS_WORLD, WorldMatrix);*/

	_matrix		ViewMatrix = XMMatrixInverse(nullptr, WorldMatrix);
	GAMEINSTANCE->Set_Transform(CPipeLine::D3DTS_VIEW, ViewMatrix);

	_matrix		ProjMatrix = XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	GAMEINSTANCE->Set_Transform(CPipeLine::D3DTS_PROJ, ProjMatrix);

	return S_OK;
}

void CCamera_Target::Bake_TargetCamera(_float fTimeDelta)//타겟 잡았을 때
{
	m_pTransformCom.lock()->Set_WorldMatrix(m_pCurrentPlayerTransformCom.lock()->Get_UnScaledWorldMatrix());
	m_pTransformCom.lock()->Go_Backward(3.f);
	m_pTransformCom.lock()->Go_Up(3.f);
}


void CCamera_Target::Interpolate_Camera(_float fTimeDelta)//일반 모드
{
	
}

void CCamera_Target::OnLevelExit()
{
	m_bFirst = true;
}

void CCamera_Target::Free()
{
}
