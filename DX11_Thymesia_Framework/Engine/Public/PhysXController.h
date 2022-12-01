#pragma once
#include "Component.h"

BEGIN(Engine)
class CTransform;
struct MODEL_DATA;
struct MESH_DATA;
class CVIBuffer_Model_Instance;

class ENGINE_DLL CPhysXController : public CComponent, public PxControllerFilterCallback, public PxUserControllerHitReport
{
	GAMECLASS_H(CPhysXController);
	SHALLOW_COPY(CPhysXController);
	DECLARE_CLONABLE(CPhysXController, CComponent);

public:
	_uint	Get_PControllerIndex() const { return m_iControllerIndex; }
	_bool	Is_EnableSimulation() const { return m_EnableSimulation; }
	void	Set_EnableSimulation(const _bool In_EnableSimulation) { m_EnableSimulation = In_EnableSimulation; }
	void	Set_CurrentCameraController();
	PxController* Get_Controller();
	_vector	Get_Position();

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

public:
	virtual void	Init_Controller(const PxCapsuleControllerDesc& In_ControllerDesc);

protected:
	void			Create_Controller();

public:
	virtual void						Synchronize_Transform(weak_ptr<CTransform> pTransform, _fvector In_vOffset = { 0.f, 0.f, 0.f });

	virtual PxControllerCollisionFlags	Synchronize_Controller(weak_ptr<CTransform> pTransform, PxF32 elapsedTime, PxControllerFilters& filters, _fvector In_vOffset = { 0.f, 0.f, 0.f });
	virtual PxControllerCollisionFlags	Set_Position(_fvector In_vPosition, PxF32 elapsedTime, PxControllerFilters& filters);
	virtual PxControllerCollisionFlags	MoveWithRotation(const _vector& disp, PxF32 minDist, PxF32 elapsedTime, PxControllerFilters& filters, const PxObstacleContext* obstacles, weak_ptr<CTransform> pTransform);
	virtual PxControllerCollisionFlags	Move(const _vector& disp, PxF32 minDist, PxF32 elapsedTime, PxControllerFilters& filters, const PxObstacleContext* obstacles = nullptr);
	virtual PxControllerCollisionFlags	MoveGravity(const _float fDeltaTime, PxControllerFilters& filters);
	void						Reset_Gravity();



public:
	//void		CreatePhysXActor(PHYSXCOLLIDERDESC& PhysXColliderDesc);
	//void		Add_PhysXActorAtScene(const PxVec3& In_MassSpaceInertiaTensor = { 0.f, 0.f, 0.f }, const PxReal In_fMass = 0.f);

private:
	void		Create_Geometry();
	//void		Create_DynamicActor(PHYSXCOLLIDERDESC& PhysXColliderDesc, PxTransform Transform);


private:
	static	_uint							m_iClonedControllerIndex;
	_uint									m_iControllerIndex;

protected:
	// 최대 속도는 XZ,Y 로 나뉘어 진다. XZ에 들어가있는 값은 X에 있는 값을 사용한다.
	_vector									m_vMaxVelocity;
	_float									m_fGravityAcc = 0.f;

protected:
	PxCapsuleControllerDesc					m_pControllerDesc;
	PxController* m_pController = nullptr;

	_bool									m_EnableSimulation = true;


public:
	virtual void OnEnable(void* pArg) override;
	virtual void OnDisable() override;

private:
	virtual void OnDestroy() override;

private:
	void Free();
};

END