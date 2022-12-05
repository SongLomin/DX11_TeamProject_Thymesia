#include "stdafx.h"
#include "PhysXCharacterController.h"
#include "Transform.h"

GAMECLASS_C(CPhysXCharacterController);
IMPLEMENT_CLONABLE(CPhysXCharacterController, CComponent);

HRESULT CPhysXCharacterController::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CPhysXCharacterController::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CPhysXCharacterController::Start()
{
	__super::Start();
}

bool CPhysXCharacterController::filter(const PxController& a, const PxController& b)
{
	return true;
}

void CPhysXCharacterController::onShapeHit(const PxControllerShapeHit& hit)
{


	m_pLastCollisionActor = hit.actor;
	m_pLastCollisionShape = hit.shape;


}

void CPhysXCharacterController::onControllerHit(const PxControllersHit& hit)
{
}

void CPhysXCharacterController::onObstacleHit(const PxControllerObstacleHit& hit)
{
}

PxControllerBehaviorFlags CPhysXCharacterController::getBehaviorFlags(const PxShape& shape, const PxActor& actor)
{
	return PxControllerBehaviorFlags(0);
}

PxControllerBehaviorFlags CPhysXCharacterController::getBehaviorFlags(const PxController& controller)
{
	return PxControllerBehaviorFlags(0);
}

PxControllerBehaviorFlags CPhysXCharacterController::getBehaviorFlags(const PxObstacle& obstacle)
{
	return PxControllerBehaviorFlags(0);
}

void CPhysXCharacterController::Init_Controller(const PxCapsuleControllerDesc& In_ControllerDesc)
{
	m_pControllerDesc = In_ControllerDesc;
	m_pControllerDesc.reportCallback = this;
	m_pControllerDesc.behaviorCallback = this;
	m_vPrePosition = { (_float)m_pControllerDesc.position.x, (_float)m_pControllerDesc.position.y, (_float)m_pControllerDesc.position.z };

	Create_Controller();
}

void CPhysXCharacterController::Synchronize_Transform(weak_ptr<CTransform> pTransform, _fvector In_vOffset)
{
	if (!m_pController)
		return;

	__super::Synchronize_Transform(pTransform, In_vOffset);
}

PxControllerCollisionFlags CPhysXCharacterController::Synchronize_Controller(weak_ptr<CTransform> pTransform, PxF32 elapsedTime, PxControllerFilters& filters, _fvector In_vOffset)
{
	return __super::Synchronize_Controller(pTransform, elapsedTime, filters, In_vOffset);
}

PxControllerCollisionFlags CPhysXCharacterController::Set_Position(_fvector In_vPosition, PxF32 elapsedTime, PxControllerFilters& filters)
{
	return  __super::Set_Position(In_vPosition, elapsedTime, filters);
}

PxControllerCollisionFlags CPhysXCharacterController::MoveWithRotation(_fvector disp, PxF32 minDist, PxF32 elapsedTime, PxControllerFilters& filters, const PxObstacleContext* obstacles, weak_ptr<CTransform> pTransform)
{
	return  __super::MoveWithRotation(disp, minDist, elapsedTime, filters, obstacles, pTransform);
}

PxControllerCollisionFlags CPhysXCharacterController::Move(_fvector disp, PxF32 minDist, PxF32 elapsedTime, PxControllerFilters& filters, const PxObstacleContext* obstacles)
{
	return  __super::Move(disp, minDist, elapsedTime, filters, obstacles);
}

PxControllerCollisionFlags CPhysXCharacterController::MoveGravity(const _float fDeltaTime, PxControllerFilters& filters)
{
	if (!Get_Enable())
		return PxControllerCollisionFlags();

	if (!m_bEnableGravity)
		return PxControllerCollisionFlags();

	PxControllerCollisionFlags ResultFlags = __super::MoveGravity(fDeltaTime, filters);

	if (m_PreResultFlags & PxControllerCollisionFlag::eCOLLISION_DOWN &&
		!(ResultFlags & PxControllerCollisionFlag::eCOLLISION_DOWN))
	{
		if (Move_FootOffset(fDeltaTime))
		{
			ResultFlags |= PxControllerCollisionFlag::eCOLLISION_DOWN;
		}
	}

	m_PreResultFlags = ResultFlags;

	return ResultFlags;
}

_bool CPhysXCharacterController::Move_FootOffset(const _float fTimeDelta)
{
	if (!m_pLastCollisionActor || !m_pLastCollisionShape)
	{
		cout << "No Actor or Shape." << endl;
		return false;
	}


	PxRaycastHit newHit;

	PxVec3 RayPos = SMath::Convert_PxVec3(m_pController->getFootPosition());
	PxVec3 RayDir = -m_pController->getUpDirection();

	PxU32 iHitCount = PxShapeExt::raycast(
		*m_pLastCollisionShape, *m_pLastCollisionActor
		, RayPos, RayDir,
		m_pController->getStepOffset() * 1.1f,
		PxHitFlag::ePOSITION, 1, &newHit);

	if (iHitCount != 0)
	{
		m_pController->setFootPosition(SMath::Convert_PxExtendedVec3(newHit.position));
		return true;
	}

	return false;
}

void CPhysXCharacterController::Free()
{
}

