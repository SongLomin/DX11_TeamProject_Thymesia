#include "PhysXController.h"
#include "GameInstance.h"
#include "PhysX_Manager.h"
#include "Transform.h"
#include "SMath.h"
#include "GameObject.h"
#include "ModelData.h"
#include "VIBuffer_Model_Instance.h"

GAMECLASS_C(CPhysXController);
IMPLEMENT_CLONABLE(CPhysXController, CComponent);

_uint CPhysXController::m_iClonedControllerIndex = 0;

HRESULT CPhysXController::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPhysXController::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iControllerIndex = m_iClonedControllerIndex++;

	return S_OK;
}

void CPhysXController::Start()
{

}

void CPhysXController::Init_Controller(const PxCapsuleControllerDesc& In_ControllerDesc)
{
	m_pControllerDesc = In_ControllerDesc;
	GET_SINGLE(CPhysX_Manager)->Create_Controller(In_ControllerDesc, &m_pController);

}

void CPhysXController::Synchronize_Transform(weak_ptr<CTransform> pTransform, _fvector In_vOffset)
{	
	if (!m_pController)
		return;

	PxExtendedVec3 vPosFromPx = m_pController->getFootPosition();

	_vector vPos = { vPosFromPx.x, vPosFromPx.y, vPosFromPx.z };
	vPos += In_vOffset;
	vPos.m128_f32[3] = 1.f;
	pTransform.lock()->Set_State(CTransform::STATE_TRANSLATION, vPos);
}

void CPhysXController::Synchronize_Controller(weak_ptr<CTransform> pTransform, _fvector In_vOffset)
{
	_vector vPos = pTransform.lock()->Get_State(CTransform::STATE_TRANSLATION);
	vPos += In_vOffset;
	vPos.m128_f32[3] = 1.f;

	m_pController->setFootPosition(SMath::Convert_PxExtendedVec3(pTransform.lock()->Get_Position()));
}

void CPhysXController::Set_Position(_fvector In_vPosition)
{
	m_pController->setFootPosition(SMath::Convert_PxExtendedVec3(In_vPosition));
}

_vector CPhysXController::Get_Position()
{
	PxExtendedVec3 vPosFromPx = m_pController->getFootPosition();

	return { (_float)vPosFromPx.x, (_float)vPosFromPx.y, (_float)vPosFromPx.z, (_float)1.f };
}

PxControllerCollisionFlags CPhysXController::MoveWithRotation(const _vector& disp, PxF32 minDist, PxF32 elapsedTime, const PxControllerFilters& filters, const PxObstacleContext* obstacles, weak_ptr<CTransform> pTransform)
{
	_vector vRotatedPosition = XMVector3TransformCoord(disp, SMath::Get_RotationMatrix(pTransform.lock()->Get_WorldMatrix()));

	PxVec3 vRotatedPositionFromPx = SMath::Convert_PxVec3(vRotatedPosition);

	//PxVec3 vRotatedPositionFromPx{ vRotatedPosition.m128_f32[0],vRotatedPosition.m128_f32[1] ,vRotatedPosition.m128_f32[2] };

	return m_pController->move(vRotatedPositionFromPx, minDist, elapsedTime, filters, obstacles);
}

PxControllerCollisionFlags CPhysXController::Move(const _vector& disp, PxF32 minDist, PxF32 elapsedTime, const PxControllerFilters& filters, const PxObstacleContext* obstacles)
{
	PxVec3 vPositionFromPx = SMath::Convert_PxVec3(disp);

	return m_pController->move(vPositionFromPx, minDist, elapsedTime, filters, obstacles);
}

PxControllerCollisionFlags CPhysXController::MoveGravity(const _float fDeltaTime)
{
	if (m_fGravityAcc <= DBL_EPSILON)
	{
		m_fGravityAcc += fDeltaTime;
		return PxControllerCollisionFlags();
	}

	/*float m_fPreHeight = -0.5f * 9.81f * m_fGravityAcc * m_fGravityAcc;

	m_fGravityAcc += fDeltaTime;

	_float m_fCurrentHeight = -0.5f * 9.81f * m_fGravityAcc * m_fGravityAcc;

	m_fCurrentHeight -= m_fPreHeight;*/

	_float fDeltaHeight = -0.5f * 9.81f * fDeltaTime*(m_fGravityAcc*2.f + fDeltaTime);

	m_fGravityAcc += fDeltaTime;

	return m_pController->move({0.f, fDeltaHeight, 0.f}, 0.f, fDeltaTime, PxControllerFilters());
}

void CPhysXController::Reset_Gravity()
{
	m_fGravityAcc = 0.f;
}

PxController* CPhysXController::Get_Controller()
{
	return m_pController;
}
void CPhysXController::Create_Geometry()
{

}

void CPhysXController::OnDestroy()
{
	if (m_pController)
		m_pController->release();
}

void CPhysXController::Free()
{
}
