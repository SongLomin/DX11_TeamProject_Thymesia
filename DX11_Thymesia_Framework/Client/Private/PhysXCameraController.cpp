#include "stdafx.h"
#include "PhysXCameraController.h"

GAMECLASS_C(CPhysXCameraController);
IMPLEMENT_CLONABLE(CPhysXCameraController, CComponent);

HRESULT CPhysXCameraController::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CPhysXCameraController::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CPhysXCameraController::Start()
{
	__super::Start();
}

bool CPhysXCameraController::filter(const PxController& a, const PxController& b)
{
	return false;
}

void CPhysXCameraController::onShapeHit(const PxControllerShapeHit& hit)
{
	

	if (!hit.shape->getActor())
	{
		cout << "No Actor." << endl;
		return;
	}

	cout << "OnShapeHit" << endl;
	PxRaycastHit newHit;
	// 컨버트 피직스Vec3 
	PxVec3 RayPos = SMath::Convert_PxVec3(XMLoadFloat4(&m_RayCamera.vOrigin));
	PxVec3 RayDir = SMath::Convert_PxVec3(XMLoadFloat3(&m_RayCamera.vDirection));

	

	PxU32 n = PxShapeExt::raycast(
		*hit.shape, *hit.shape->getActor()
		, RayPos, RayDir,
		(PxReal)m_RayCamera.fLength,
		PxHitFlag::ePOSITION, 1, &newHit);

	
	if (n > 0)
	{
		PxExtendedVec3 position{newHit.position.x, newHit.position.y, newHit.position.z};

		m_pController->setFootPosition(position);
	}

	
}

void CPhysXCameraController::onControllerHit(const PxControllersHit& hit)
{
}

void CPhysXCameraController::onObstacleHit(const PxControllerObstacleHit& hit)
{
}

PxControllerBehaviorFlags CPhysXCameraController::getBehaviorFlags(const PxShape& shape, const PxActor& actor)
{
	return PxControllerBehaviorFlags(0);
}

PxControllerBehaviorFlags CPhysXCameraController::getBehaviorFlags(const PxController& controller)
{
	return PxControllerBehaviorFlags(0);
}

PxControllerBehaviorFlags CPhysXCameraController::getBehaviorFlags(const PxObstacle& obstacle)
{
	return PxControllerBehaviorFlags(0);
}

void CPhysXCameraController::Init_Controller(const PxCapsuleControllerDesc& In_ControllerDesc)
{
	m_pControllerDesc = In_ControllerDesc;
	m_pControllerDesc.reportCallback = this;
	m_pControllerDesc.behaviorCallback = this;

	Create_Controller();
}

PxControllerCollisionFlags CPhysXCameraController::Synchronize_Controller(weak_ptr<CTransform> pTransform, PxF32 elapsedTime, PxControllerFilters& filters, _fvector In_vOffset)
{
	return __super::Synchronize_Controller(pTransform, elapsedTime, filters, In_vOffset);
}

PxControllerCollisionFlags CPhysXCameraController::Set_Position(_fvector In_vPosition, PxF32 elapsedTime, PxControllerFilters& filters)
{
	return  __super::Set_Position(In_vPosition, elapsedTime, filters);
}

PxControllerCollisionFlags CPhysXCameraController::MoveWithRotation(const _vector& disp, PxF32 minDist, PxF32 elapsedTime, PxControllerFilters& filters, const PxObstacleContext* obstacles, weak_ptr<CTransform> pTransform)
{
	return  __super::MoveWithRotation(disp, minDist, elapsedTime, filters, obstacles, pTransform);
}

PxControllerCollisionFlags CPhysXCameraController::Move(const _vector& disp, PxF32 minDist, PxF32 elapsedTime, PxControllerFilters& filters, const PxObstacleContext* obstacles)
{
	return  __super::Move(disp, minDist, elapsedTime, filters, obstacles);
}

PxControllerCollisionFlags CPhysXCameraController::MoveGravity(const _float fDeltaTime, PxControllerFilters& filters)
{
	return  __super::MoveGravity(fDeltaTime, filters);
}


void CPhysXCameraController::Update_Ray(const RAY& In_Ray)
{
	m_RayCamera = In_Ray;
}

void CPhysXCameraController::Free()
{
}

