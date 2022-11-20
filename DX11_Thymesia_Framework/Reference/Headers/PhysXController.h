#pragma once
#include "Component.h"

BEGIN(Engine)
class CTransform;
struct MODEL_DATA;
struct MESH_DATA;
class CVIBuffer_Model_Instance;

class ENGINE_DLL CPhysXController final : public CComponent
{
	GAMECLASS_H(CPhysXController);
	SHALLOW_COPY(CPhysXController);
	DECLARE_CLONABLE(CPhysXController, CComponent);

public:
	_uint	Get_PControllerIndex() const { return m_iControllerIndex; }

	PxController* Get_Controller();

private:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

public:
	void	Init_Controller(const PxCapsuleControllerDesc& In_ControllerDesc);

public:
	void	Synchronize_Transform(weak_ptr<CTransform> pTransform, _fvector In_vOffset = {0.f, 0.f, 0.f});
	//void	Synchronize_Transform_Position(weak_ptr<CTransform> pTransform);
	//void	Synchronize_Transform_Rotation(weak_ptr<CTransform> pTransform);

	void	Synchronize_Controller(weak_ptr<CTransform> pTransform, _fvector In_vOffset = { 0.f, 0.f, 0.f });

	void						Set_Position(_fvector In_vPosition);
	_vector					Get_Position();
	PxControllerCollisionFlags	MoveWithRotation(const _vector& disp, PxF32 minDist, PxF32 elapsedTime, const PxControllerFilters& filters, const PxObstacleContext* obstacles, weak_ptr<CTransform> pTransform);
	PxControllerCollisionFlags	MoveGravity(const _float fDeltaTime);
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

private:
	// �ִ� �ӵ��� XZ,Y �� ������ ����. XZ�� ���ִ� ���� X�� �ִ� ���� ����Ѵ�.
	_vector									m_vMaxVelocity;
	_float									m_fGravityAcc = 0.f;

private:
	PxCapsuleControllerDesc					m_pControllerDesc;
	PxController*							m_pController = nullptr;


private:
	virtual void OnDestroy() override;

private:
	void Free();
};

END