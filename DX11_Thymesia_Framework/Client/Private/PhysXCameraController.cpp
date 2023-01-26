#include "stdafx.h"
#include "PhysXCameraController.h"
#include "Transform.h"
#include "GameInstance.h"
#include "GameObject.h"

GAMECLASS_C(CPhysXCameraController);
IMPLEMENT_CLONABLE(CPhysXCameraController, CComponent);

_vector CPhysXCameraController::Get_Position()
{
	PxExtendedVec3 vPosFromPx = m_pController->getFootPosition();

	return { (_float)vPosFromPx.x, (_float)vPosFromPx.y, (_float)vPosFromPx.z, (_float)1.f };
}

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

PxQueryHitType::Enum CPhysXCameraController::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
{
	PxFilterData OtherFilter = shape->getSimulationFilterData();

	if ((m_FilterData.word0 & OtherFilter.word1) && (OtherFilter.word0 & m_FilterData.word1))
	{
		return PxQueryHitType::eTOUCH;
	}

	return PxQueryHitType::eNONE;
}

PxQueryHitType::Enum CPhysXCameraController::postFilter(const PxFilterData& filterData, const PxQueryHit& hit)
{
	PxFilterData OtherFilter = hit.shape->getSimulationFilterData();

	if ((filterData.word0 & OtherFilter.word1) && (OtherFilter.word0 & filterData.word1))
	{
		return PxQueryHitType::eTOUCH;
	}

	return PxQueryHitType::eNONE;
}


void CPhysXCameraController::Init_Controller(const PxCapsuleControllerDesc& In_ControllerDesc, const _uint In_CollisionLayer)
{
	m_pControllerDesc = In_ControllerDesc;
	m_pControllerDesc.reportCallback = this;
	m_pControllerDesc.behaviorCallback = this;
	m_pControllerDesc.userData = &m_iControllerIndex;
	m_vPrePosition = { (_float)m_pControllerDesc.position.x, (_float)m_pControllerDesc.position.y, (_float)m_pControllerDesc.position.z };
	m_FilterData.word0 = (1 << In_CollisionLayer);
	m_FilterData.word1 = GAMEINSTANCE->Get_PhysXFilterGroup(In_CollisionLayer);


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
	m_pController->setPosition(SMath::Convert_PxExtendedVec3(m_pTargetTransformCom.lock()->Get_Position() + XMVectorSet(0.f, 1.1f, 0.f, 0.f)));

	m_bCollision = false;

	_vector vTargetPosition = m_pTargetTransformCom.lock()->Get_Position();
	_vector vMovePosition = pTransform.lock()->Get_Position() - vTargetPosition;

	PxVec3 vMovePositionToPx = SMath::Convert_PxVec3(vMovePosition);

	Bind_FilterOptions(filters);
	//filters.mCCTFilterCallback = this;
	//filters.mFilterCallback = this;

	return m_pController->move(vMovePositionToPx, 0.f, elapsedTime, filters);
}

PxControllerCollisionFlags CPhysXCameraController::Set_Position(_fvector In_vPosition, PxF32 elapsedTime, PxControllerFilters& filters)
{
	return  __super::Set_Position(In_vPosition, elapsedTime, filters);
}

PxControllerCollisionFlags CPhysXCameraController::MoveWithRotation(_fvector disp, PxF32 minDist, PxF32 elapsedTime, PxControllerFilters& filters, const PxObstacleContext* obstacles, weak_ptr<CTransform> pTransform, const _flag In_RootFlag)
{
	return  __super::MoveWithRotation(disp, minDist, elapsedTime, filters, obstacles, pTransform, In_RootFlag);
}

PxControllerCollisionFlags CPhysXCameraController::Move(_fvector disp, PxF32 minDist, PxF32 elapsedTime, PxControllerFilters& filters, const PxObstacleContext* obstacles)
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


	_vector vPlayerPosition = m_pTargetTransformCom.lock()->Get_Position() + XMVectorSet(0.f, 1.1f, 0.f, 0.f);
	_vector vPlayerToCameraDir = m_pOwner.lock()->Get_Transform()->Get_State(CTransform::STATE_LOOK)*-1.f;
	//_vector vPlayerToCameraDir = m_pOwner.lock()->Get_Transform()->Get_Position() - vPlayerPosition;
	//_float fLength = XMVectorGetX(XMVector3Length(vPlayerToCameraDir));
	vPlayerToCameraDir = XMVector3Normalize(vPlayerToCameraDir);




	XMStoreFloat4(&m_RayCamera.vOrigin, vPlayerPosition);
	XMStoreFloat3(&m_RayCamera.vDirection, vPlayerToCameraDir);
	m_RayCamera.vOrigin.w = 1.f;
	m_RayCamera.fLength = 3.5f/*fLength * 1.2f */ ;


	PxRaycastHit newHit[2];

	PxVec3 RayPos = SMath::Convert_PxVec3(XMLoadFloat4(&m_RayCamera.vOrigin));
	PxVec3 RayDir = SMath::Convert_PxVec3(XMLoadFloat3(&m_RayCamera.vDirection));

	PxU32 n = PxShapeExt::raycast(
		*m_pLastHitShape, *m_pLastHitActor
		, RayPos, RayDir,
		(PxReal)m_RayCamera.fLength,
		PxHitFlag::ePOSITION, 2, newHit);


	PxVec3 RayDirOffset = RayDir * -1.5f;

	// 충돌했는지?
	if (n > 0)
	{


		// + PxExtendedVec3(RayDirOffset.x, RayDirOffset.y, RayDirOffset.z)

		//newHit.position += RayDirOffset;

		//PxExtendedVec3 position = m_pController->getPosition();

		//PxVec3 MovePosition = newHit.position - PxVec3(position.x, position.y, position.z);

		//PxControllerFilters Filters;
		//Filters.mFilterFlags = PxQueryFlag::Enum(0);

		m_pController->setPosition({ newHit[0].position.x, newHit[0].position.y, newHit[0].position.z});
		m_bCollision = true;

		//Print_Vector(SMath::Convert_PxExtendedVec3ToVector(position));
		//m_pController->move(MovePosition, 0.f, fDeltaTime, Filters);

	}

	//cout << "ShapeHit" << endl;
}


void CPhysXCameraController::Update_Ray(weak_ptr<CTransform> pTargetTransform)
{
	m_pTargetTransformCom = pTargetTransform;
}

void CPhysXCameraController::Free()
{
}

