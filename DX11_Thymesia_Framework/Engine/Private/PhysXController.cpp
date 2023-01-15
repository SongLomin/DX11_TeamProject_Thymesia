#include "PhysXController.h"
#include "GameInstance.h"
#include "PhysX_Manager.h"
#include "Transform.h"
#include "SMath.h"
#include "GameObject.h"
#include "ModelData.h"
#include "VIBuffer_Model_Instance.h"
#include "Model.h"

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

	GET_SINGLE(CPhysX_Manager)->Register_PhysXController(Weak_StaticCast<CPhysXController>(m_this));

	return S_OK;
}

void CPhysXController::Start()
{

}

bool CPhysXController::filter(const PxController& a, const PxController& b)
{
	shared_ptr<CPhysX_Manager> pPhysxManager = GET_SINGLE(CPhysX_Manager);

	weak_ptr<CPhysXController> pLeftControllerCom = pPhysxManager->Find_PhysXController(*(_uint*)a.getUserData());
	weak_ptr<CPhysXController> pRightControllerCom = pPhysxManager->Find_PhysXController(*(_uint*)b.getUserData());

	if (!pLeftControllerCom.lock() || !pRightControllerCom.lock())
		return false;

	if (pLeftControllerCom.lock()->Is_EnableSimulation() && pRightControllerCom.lock()->Is_EnableSimulation())
		return true;



	return false;
}

void CPhysXController::onShapeHit(const PxControllerShapeHit& hit)
{
}

void CPhysXController::onControllerHit(const PxControllersHit& hit)
{
	Callback_ControllerHit(hit);
}


void CPhysXController::onObstacleHit(const PxControllerObstacleHit& hit)
{

}

PxQueryHitType::Enum CPhysXController::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
{
	if (!Get_Enable())
		return PxQueryHitType::eNONE;

	if (!m_EnableColliderSimulation)
	{
		return PxQueryHitType::eNONE;
	}
	
	PxFilterData OtherFilter = shape->getSimulationFilterData();

	if ((m_FilterData.word0 & OtherFilter.word1) && (OtherFilter.word0 & m_FilterData.word1))
	{
		return PxQueryHitType::eBLOCK;
	}

	return PxQueryHitType::eNONE;
}

PxQueryHitType::Enum CPhysXController::postFilter(const PxFilterData& filterData, const PxQueryHit& hit)
{
	if (!Get_Enable())
		return PxQueryHitType::eNONE;

	if (!m_EnableColliderSimulation)
	{
		return PxQueryHitType::eNONE;
	}

	PxFilterData OtherFilter = hit.shape->getSimulationFilterData();

	if ((filterData.word0 & OtherFilter.word1) && (OtherFilter.word0 & filterData.word1))
	{
		return PxQueryHitType::eBLOCK;
	}

	return PxQueryHitType::eNONE;
}

void CPhysXController::Bind_FilterOptions(PxControllerFilters& Out_Filters)
{
	Out_Filters.mCCTFilterCallback = this;
	Out_Filters.mFilterCallback = this;
	//Out_Filters.mFilterData = &m_FilterData;

	//if (!m_EnableSimulation)
	//{
	//	//cout << "Null Filter" << endl;
	//	Out_Filters.mFilterFlags = PxQueryFlags(0);
	//}
}


void CPhysXController::Init_Controller(const PxCapsuleControllerDesc& In_ControllerDesc, const _uint In_CollisionLayer)
{
	m_pControllerDesc = In_ControllerDesc;
	m_pControllerDesc.reportCallback = this;
	m_pControllerDesc.userData = &m_iControllerIndex;
	m_FilterData.word0 = (1 << In_CollisionLayer);
	m_FilterData.word1 = GET_SINGLE(CPhysX_Manager)->Get_PhysXFilterGroup(In_CollisionLayer);
	Create_Controller();
}

void CPhysXController::Create_Controller()
{
	GET_SINGLE(CPhysX_Manager)->Create_Controller(m_pControllerDesc, &m_pController);
	m_pController->setUserData(&m_iControllerIndex);
	
}

void CPhysXController::Synchronize_Transform(weak_ptr<CTransform> pTransform, _fvector In_vOffset)
{
	if (!m_pController)
		return;

	//if (!Get_Enable())
	//	return;

	PxExtendedVec3 vPosFromPx = m_pController->getFootPosition();

	_vector vPos = { (_float)vPosFromPx.x, (_float)vPosFromPx.y, (_float)vPosFromPx.z };
	vPos += In_vOffset;
	vPos.m128_f32[3] = 1.f;
	pTransform.lock()->Set_State(CTransform::STATE_TRANSLATION, vPos);
}

PxControllerCollisionFlags CPhysXController::Synchronize_Controller(weak_ptr<CTransform> pTransform, PxF32 elapsedTime, PxControllerFilters& filters, _fvector In_vOffset)
{
	_vector vPos = pTransform.lock()->Get_State(CTransform::STATE_TRANSLATION);
	vPos += In_vOffset;
	vPos.m128_f32[3] = 1.f;

	m_pController->setFootPosition(SMath::Convert_PxExtendedVec3(pTransform.lock()->Get_Position()));

	Bind_FilterOptions(filters);

	return m_pController->move({ 0.f, 0.f, 0.f }, 0.f, elapsedTime, filters);
}

PxControllerCollisionFlags CPhysXController::Set_Position(_fvector In_vPosition, PxF32 elapsedTime, PxControllerFilters& filters)
{
	m_pController->setFootPosition(SMath::Convert_PxExtendedVec3(In_vPosition));

	Bind_FilterOptions(filters);

	return m_pController->move({ 0.f, 0.f, 0.f }, 0.f, elapsedTime, filters);
}

_vector CPhysXController::Get_Position()
{
	PxExtendedVec3 vPosFromPx = m_pController->getFootPosition();

	return { (_float)vPosFromPx.x, (_float)vPosFromPx.y, (_float)vPosFromPx.z, (_float)1.f };
}

void CPhysXController::Enable_Gravity(const _bool In_bGravity)
{
	m_bEnableGravity = In_bGravity;
}

PxControllerCollisionFlags CPhysXController::MoveWithRotation(_fvector disp, PxF32 minDist, PxF32 elapsedTime, PxControllerFilters& filters, const PxObstacleContext* obstacles, weak_ptr<CTransform> pTransform, const _flag In_RootFlag)
{
	//if (!Get_Enable())
	//	return PxControllerCollisionFlags();

	_vector vRotatedPosition = XMVector3TransformCoord(disp, SMath::Get_RotationMatrix(pTransform.lock()->Get_WorldMatrix()));

	Bind_FilterOptions(filters);

	PxVec3 vRotatedPositionFromPx = SMath::Convert_PxVec3(vRotatedPosition);

	PxExtendedVec3 PrePosition = m_pController->getPosition();

	//PxVec3 vRotatedPositionFromPx{ vRotatedPosition.m128_f32[0],vRotatedPosition.m128_f32[1] ,vRotatedPosition.m128_f32[2] };

	//cout << "피직스 이동 값: ";
	//Print_Vector(vRotatedPosition);

	if (DBL_EPSILON < vRotatedPositionFromPx.y || DBL_EPSILON < vRotatedPosition.m128_f32[1])
		int a = 0;

	if (!(In_RootFlag & (_byte)ROOTNODE_FLAG::X))
	{
		vRotatedPositionFromPx.x = 0.f;
	}

	if (!(In_RootFlag & (_byte)ROOTNODE_FLAG::Y))
	{
		vRotatedPositionFromPx.y = 0.f;
	}

	if (!(In_RootFlag & (_byte)ROOTNODE_FLAG::Z))
	{
		vRotatedPositionFromPx.z = 0.f;
	}

	auto Result = m_pController->move(vRotatedPositionFromPx, minDist, elapsedTime, filters, obstacles);

	PxExtendedVec3 CurPosition = m_pController->getPosition();
	PxVec3 RealMove = CurPosition - PrePosition;

	//cout << "실제 이동 값: ";
	//Print_Vector({ (_float)RealMove.x, (_float)RealMove.y, (_float)RealMove.z });

	return Result;
}

PxControllerCollisionFlags CPhysXController::Move(_fvector disp, PxF32 minDist, PxF32 elapsedTime, PxControllerFilters& filters, const PxObstacleContext* obstacles)
{
	if (!Get_Enable())
		return PxControllerCollisionFlags();

	PxVec3 vPositionFromPx = SMath::Convert_PxVec3(disp);

	Bind_FilterOptions(filters);

	return m_pController->move(vPositionFromPx, minDist, elapsedTime, filters, obstacles);
}

PxControllerCollisionFlags CPhysXController::MoveGravity(const _float fDeltaTime, PxControllerFilters& filters)
{
	if (!Get_Enable())
		return PxControllerCollisionFlags();

	if(!m_bEnableGravity)
		return PxControllerCollisionFlags();

	/*if (m_fGravityAcc <= DBL_EPSILON)
	{
		m_fGravityAcc += fDeltaTime;
		return PxControllerCollisionFlags();
	}*/

	/*float m_fPreHeight = -0.5f * 9.81f * m_fGravityAcc * m_fGravityAcc;
	m_fGravityAcc += fDeltaTime;
	_float m_fCurrentHeight = -0.5f * 9.81f * m_fGravityAcc * m_fGravityAcc;
	m_fCurrentHeight -= m_fPreHeight;*/

	Bind_FilterOptions(filters);

	_float fDeltaHeight = -0.5f * 9.81f * fDeltaTime * (m_fGravityAcc * 2.f + fDeltaTime);
	fDeltaHeight += 0.0001f;
	m_fGravityAcc += fDeltaTime;

	m_fGravityAcc = min(3.f, m_fGravityAcc);

	return m_pController->move({ 0.f, fDeltaHeight, 0.f }, 0.f, fDeltaTime, filters);
}

void CPhysXController::Reset_Gravity()
{
	if (!Get_Enable())
		return;

	m_fGravityAcc = 0.f;
} 

void CPhysXController::Set_EnableSimulation(const _bool In_EnableSimulation)
{
#ifdef _DEBUG
	//cout << "Set_EnableSimulation: " << m_pOwner.lock()->Get_Component<CModel>().lock()->Get_ModelKey() << ": " << (In_EnableSimulation ? "true" : "false") << endl;

	if (In_EnableSimulation)
	{
		int i = 0;
	}
	else
	{
		int i = 0;
	}

#endif // _DEBUG


	m_EnableSimulation = In_EnableSimulation;
}

void CPhysXController::Set_CurrentCameraController()
{
	GET_SINGLE(CPhysX_Manager)->Set_CurrentCameraControllerIndex(m_iControllerIndex);
}

PxController* CPhysXController::Get_Controller()
{
	return m_pController;
}
void CPhysXController::Create_Geometry()
{

}

void CPhysXController::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

	/*if (!m_pController)
	{
		GET_SINGLE(CPhysX_Manager)->Create_Controller(m_pControllerDesc, &m_pController);
	}*/

	Set_EnableSimulation(true);
}

void CPhysXController::OnDisable()
{
	__super::OnDisable();

	/*if (m_pController)
	{
		m_pController->release();
		m_pController = nullptr;
	}*/

	Set_EnableSimulation(false);
}

void CPhysXController::OnDestroy()
{
	if (m_pController)
		m_pController->release();
}

void CPhysXController::Free()
{
}

