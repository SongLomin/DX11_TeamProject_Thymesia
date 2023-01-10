#pragma once
#include "PhysXController.h"
#include "Client_Defines.h"

BEGIN(Client)

class CPhysXCameraController :
	public CPhysXController, public PxControllerBehaviorCallback
{
	GAMECLASS_H(CPhysXCameraController);
	SHALLOW_COPY(CPhysXCameraController);
	DECLARE_CLONABLE(CPhysXCameraController, CComponent);
	
public:
	_bool Is_Collision() const { return m_bCollision; }
	virtual _vector	Get_Position() override;

protected:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

public: // 피직스 컨트롤러 필터
	virtual bool filter(const PxController& a, const PxController& b) override;

public: // Hit Report
	virtual void onShapeHit(const PxControllerShapeHit& hit) override;
	virtual void onControllerHit(const PxControllersHit& hit) override;
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) override;

	// PxControllerBehaviorCallback을(를) 통해 상속됨
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor) override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller) override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle) override;
	
	// PxQueryFilterCallback을(를) 통해 상속됨
	virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) override;
	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit) override;

public:
	virtual void Init_Controller(const PxCapsuleControllerDesc& In_ControllerDesc, const _uint In_CollisionLayer) override;

public:
	virtual void						Synchronize_Transform(weak_ptr<CTransform> pTransform, _fvector In_vOffset = { 0.f, 0.f, 0.f }) override;
	virtual PxControllerCollisionFlags	Synchronize_Controller(weak_ptr<CTransform> pTransform, PxF32 elapsedTime, PxControllerFilters& filters, _fvector In_vOffset = { 0.f, 0.f, 0.f }) override;
	virtual PxControllerCollisionFlags	Set_Position(_fvector In_vPosition, PxF32 elapsedTime, PxControllerFilters& filters) override;
	virtual PxControllerCollisionFlags	MoveWithRotation(_fvector disp, PxF32 minDist, PxF32 elapsedTime, PxControllerFilters& filters, const PxObstacleContext* obstacles, weak_ptr<CTransform> pTransform, const _flag In_RootFlag = (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Y + (_byte)ROOTNODE_FLAG::Z) override;
	virtual PxControllerCollisionFlags	Move(_fvector disp, PxF32 minDist, PxF32 elapsedTime, PxControllerFilters& filters, const PxObstacleContext* obstacles = nullptr) override;
	virtual PxControllerCollisionFlags	MoveGravity(const _float fDeltaTime, PxControllerFilters& filters)  override;
	void								Update_RayCastCollision(_float fDeltaTime);


public:
	void Update_Ray(weak_ptr<CTransform> pTargetTransform);


private:
	weak_ptr<CTransform> m_pTargetTransformCom;
	RAY m_RayCamera;
	_float3 m_vPrePosition;

	PxShape* m_pLastHitShape = nullptr;
	PxRigidActor* m_pLastHitActor = nullptr;

	_bool	m_bCollision = false;


private:
	void Free();

};

END