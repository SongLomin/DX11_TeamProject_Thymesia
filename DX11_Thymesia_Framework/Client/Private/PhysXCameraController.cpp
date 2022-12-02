#include "stdafx.h"
#include "PhysXCameraController.h"
#include "Transform.h"
#include "GameInstance.h"
#include "GameObject.h"

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


	if (!hit.actor)
	{
		cout << "No Actor." << endl;
		return;
	}

	m_pLastHitShape = hit.shape;
	m_pLastHitActor = hit.actor;

	cout << "ShapeHit" << endl;

}

void CPhysXCameraController::onControllerHit(const PxControllersHit& hit)
{
}

void CPhysXCameraController::onObstacleHit(const PxControllerObstacleHit& hit)
{
}

PxControllerBehaviorFlags CPhysXCameraController::getBehaviorFlags(const PxShape& shape, const PxActor& actor)
{
	return PxControllerBehaviorFlag::eCCT_USER_DEFINED_RIDE;
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
	m_vPrePosition = { (_float)m_pControllerDesc.position.x, (_float)m_pControllerDesc.position.y, (_float)m_pControllerDesc.position.z };

	Create_Controller();
}

void CPhysXCameraController::Synchronize_Transform(weak_ptr<CTransform> pTransform, _fvector In_vOffset)
{
	if (!m_pController)
		return;



	PxExtendedVec3 vPosFromPx = m_pController->getPosition();

	_vector vPos = { (_float)vPosFromPx.x, (_float)vPosFromPx.y, (_float)vPosFromPx.z };
	vPos += In_vOffset;
	vPos.m128_f32[3] = 1.f;

	XMStoreFloat3(&m_vPrePosition, vPos);

	pTransform.lock()->Set_State(CTransform::STATE_TRANSLATION, vPos);
}

PxControllerCollisionFlags CPhysXCameraController::Synchronize_Controller(weak_ptr<CTransform> pTransform, PxF32 elapsedTime, PxControllerFilters& filters, _fvector In_vOffset)
{
	_vector vPrePositionToVector = XMLoadFloat3(&m_vPrePosition);
	_vector vMovePosition = pTransform.lock()->Get_Position() - vPrePositionToVector;

	PxVec3 vMovePositionToPx = SMath::Convert_PxVec3(vMovePosition);

	filters.mCCTFilterCallback = this;

	return m_pController->move(vMovePositionToPx, 0.f, elapsedTime, filters);
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

void CPhysXCameraController::Update_RayCastCollision(_float fDeltaTime)
{
	if (!m_pLastHitShape || !m_pLastHitActor)
		return;

	

	_vector vPlayerToCameraDir = m_pOwner.lock()->Get_Transform()->Get_Position() - m_pTargetTransformCom.lock()->Get_Position() + XMVectorSet(0.f, 1.1f, 0.f, 0.f);	
	_float fLength = 1000.f;
	vPlayerToCameraDir = XMVector3Normalize(vPlayerToCameraDir);

	

	_vector vPlayerPosition = m_pTargetTransformCom.lock()->Get_Position() + XMVectorSet(0.f, 1.1f, 0.f, 0.f);

	XMStoreFloat4(&m_RayCamera.vOrigin, vPlayerPosition);
	XMStoreFloat3(&m_RayCamera.vDirection, vPlayerToCameraDir);
	m_RayCamera.vOrigin.w = 1.f;
	m_RayCamera.fLength = 1000.f;


	PxRaycastHit newHit;

	PxVec3 RayPos = SMath::Convert_PxVec3(XMLoadFloat4(&m_RayCamera.vOrigin));
	PxVec3 RayDir = SMath::Convert_PxVec3(XMLoadFloat3(&m_RayCamera.vDirection));

	PxU32 n = PxShapeExt::raycast(
		*m_pLastHitShape, *m_pLastHitActor
		, RayPos, RayDir,
		(PxReal)m_RayCamera.fLength,
		PxHitFlag::ePOSITION, 1, &newHit);

	
	PxVec3 RayDirOffset = (RayDir * -1.f) * 0.7f;

	// 충돌했는지?
	if (n > 0)
	{
		

		// + PxExtendedVec3(RayDirOffset.x, RayDirOffset.y, RayDirOffset.z)

		newHit.position += RayDirOffset;

		PxExtendedVec3 position = m_pController->getPosition();

		PxVec3 MovePosition = newHit.position - PxVec3(position.x, position.y, position.z);

		//PxControllerFilters Filters;
		//Filters.mFilterFlags = PxQueryFlag::Enum(0);

		m_pController->setPosition({ newHit.position.x, newHit.position.y, newHit.position.z });
		//Print_Vector(SMath::Convert_PxExtendedVec3ToVector(position));
		//m_pController->move(MovePosition, 0.f, fDeltaTime, Filters);
		
	}
}


void CPhysXCameraController::Update_Ray(weak_ptr<CTransform> pTargetTransform)
{
	m_pTargetTransformCom = pTargetTransform;
}

void CPhysXCameraController::Free()
{
}

