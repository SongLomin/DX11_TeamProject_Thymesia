#pragma once
#include "Component.h"

BEGIN(Engine)
class CTransform;
struct MODEL_DATA;
struct MESH_DATA;
class CVIBuffer_Model_Instance;

class ENGINE_DLL CPhysXController : 
	public CComponent, public PxControllerFilterCallback, public PxUserControllerHitReport, public PxQueryFilterCallback
{
	GAMECLASS_H(CPhysXController);
	SHALLOW_COPY(CPhysXController);
	DECLARE_CLONABLE(CPhysXController, CComponent);

public:
	_uint	Get_PControllerIndex() const { return m_iControllerIndex; }
	_bool	Is_EnableSimulation() const { return m_EnableSimulation; }
	void	Set_EnableSimulation(const _bool In_EnableSimulation);
	void	Set_EnableColliderSimulation(const _bool In_EnableSimulation) { m_EnableColliderSimulation = In_EnableSimulation; }
	void	Set_CurrentCameraController();
	PxController* Get_Controller();
	virtual _vector	Get_Position();

	void	Enable_Gravity(const _bool In_bGravity);

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

	// PxQueryFilterCallback을(를) 통해 상속됨
	virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) override;
	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit) override;

protected:
	void Bind_FilterOptions(PxControllerFilters& Out_Filters);

public:
	virtual void	Init_Controller(const PxCapsuleControllerDesc& In_ControllerDesc, const _uint In_CollisionLayer);

protected:
	void			Create_Controller();

public:
	virtual void						Synchronize_Transform(weak_ptr<CTransform> pTransform, _fvector In_vOffset = { 0.f, 0.f, 0.f });

	virtual PxControllerCollisionFlags	Synchronize_Controller(weak_ptr<CTransform> pTransform, PxF32 elapsedTime, PxControllerFilters& filters, _fvector In_vOffset = { 0.f, 0.f, 0.f });
	virtual PxControllerCollisionFlags	Set_Position(_fvector In_vPosition, PxF32 elapsedTime, PxControllerFilters& filters);
	virtual PxControllerCollisionFlags	MoveWithRotation(_fvector disp, PxF32 minDist, PxF32 elapsedTime, PxControllerFilters& filters, const PxObstacleContext* obstacles, weak_ptr<CTransform> pTransform, const _flag In_RootFlag = (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Y + (_byte)ROOTNODE_FLAG::Z);
	virtual PxControllerCollisionFlags	Move(_fvector disp, PxF32 minDist, PxF32 elapsedTime, PxControllerFilters& filters, const PxObstacleContext* obstacles = nullptr);
	virtual PxControllerCollisionFlags	MoveGravity(const _float fDeltaTime, PxControllerFilters& filters);
	void								Reset_Gravity();

public:
	FDelegate<const PxControllersHit&>	Callback_ControllerHit;


public:
	//void		CreatePhysXActor(PHYSXCOLLIDERDESC& PhysXColliderDesc);
	//void		Add_PhysXActorAtScene(const PxVec3& In_MassSpaceInertiaTensor = { 0.f, 0.f, 0.f }, const PxReal In_fMass = 0.f);

private:
	void		Create_Geometry();
	//void		Create_DynamicActor(PHYSXCOLLIDERDESC& PhysXColliderDesc, PxTransform Transform);


private:
	static	_uint							m_iClonedControllerIndex;
protected:
	_uint									m_iControllerIndex;
	

protected:
	// 최대 속도는 XZ,Y 로 나뉘어 진다. XZ에 들어가있는 값은 X에 있는 값을 사용한다.
	PxFilterData							m_FilterData;
	_vector									m_vMaxVelocity;
	_float									m_fGravityAcc = 0.f;

protected:
	PxCapsuleControllerDesc					m_pControllerDesc;
	PxController* m_pController = nullptr;

	_bool									m_EnableSimulation = true;
	_bool									m_EnableColliderSimulation = true;
	_bool									m_bEnableGravity = true;


public:
	virtual void OnEnable(void* pArg) override;
	virtual void OnDisable() override;

private:
	virtual void OnDestroy() override;

private:
	void Free();
};

END