#pragma once
#include "Component.h"

BEGIN(Engine)
class CTransform;
struct MODEL_DATA;
struct MESH_DATA;
class CVIBuffer_Model_Instance;

class ENGINE_DLL CPhysXCollider final : public CComponent
{
	GAMECLASS_H(CPhysXCollider);
	SHALLOW_COPY(CPhysXCollider);
	DECLARE_CLONABLE(CPhysXCollider, CComponent);

public:
	_vector	Get_Position();
	_vector	Get_Quaternion();
	_vector	Get_Velocity();
	_float	Get_Mess();
	_vector	Get_AngularVelocity();
	_vector	Get_LinearVelocity();
	_matrix Get_WorldMatrix();

	_uint	Get_PColliderIndex() const { return m_iColliderIndex; }

	PHYSXCOLLIDERDESC Get_ColliderDesc() const { return m_PhysXColliderDesc; }


	HRESULT Set_Position(_vector _vPos, _vector _vQuaternion);
	HRESULT Set_Position(_vector _vPos);
	void	Set_MaxLinearVelocity(_vector vMaxVelocity) { m_vMaxVelocity = vMaxVelocity; }
	void	Set_Scale(_vector vScale);
	void	Set_ActorFlag(PxActorFlag::Enum eFlag, _bool bState);

	void	Set_PickState(_bool bState) { m_bPickState = bState; }
	void	Set_Pickable(_bool bState) { m_bPickable = bState; }
	void	Set_YFixed() { m_bYFixed = true; }

	void	Set_ConvexMesh(_uint iIndex, PxConvexMesh* pMesh) { m_ConvexMeshes[iIndex] = pMesh; }

	_bool	Is_Pickable() { return m_bPickable; }
	_bool	Is_Picked() { return m_bPickState; }
	_bool	Is_YFixed() { return m_bYFixed; }

	void	Delete_Collider();
	void	Create_Collider();

private:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

public:
	void	Init_MeshCollider(weak_ptr<MESH_DATA> pMeshData, const vector<INSTANCE_MESH_DESC>* In_ParticleDescs = nullptr);
	void	Init_ConvexMeshCollider(weak_ptr<MESH_DATA> pMeshData, const vector<INSTANCE_MESH_DESC>* In_ParticleDescs = nullptr);
	void	Init_ModelCollider(weak_ptr<MODEL_DATA> pModelData, const _bool In_isConvex);
	void	Init_ModelInstanceCollider(weak_ptr<MODEL_DATA> pModelData, const vector<INSTANCE_MESH_DESC>& In_ParticleDescs, const _bool In_isConvex);

public:
	void	Synchronize_Transform(weak_ptr<CTransform> pTransform, _fvector In_vOffset = {0.f, 0.f, 0.f});
	void	Synchronize_Transform_Position(weak_ptr<CTransform> pTransform);
	void	Synchronize_Transform_Rotation(weak_ptr<CTransform> pTransform);
	_matrix	Synchronize_Matrix(_fmatrix In_WorldMatrix);

	void	Synchronize_Collider(weak_ptr<CTransform> pTransform, _fvector In_vOffset = { 0.f, 0.f, 0.f });

public:
	void	PutToSleep();
	void	WakeUp();
	void	Attach_Shape(PxShape* pShape);
	void	Detach_Shape(PxShape* pShape);


public:
	HRESULT Add_Force(_vector _vForce);
	HRESULT Clear_Force();
	HRESULT Clear_Velocity();
	HRESULT Add_LinearVelocityResistance(_vector fResistanceRate);

public:
	FDelegate<weak_ptr<CPhysXCollider>> CallBack_CollisionEnter;
	FDelegate<weak_ptr<CPhysXCollider>> CallBack_CollisionStay;
	FDelegate<weak_ptr<CPhysXCollider>> CallBack_CollisionExit;


private:
	static	_uint							m_iClonedColliderIndex;
	_uint									m_iColliderIndex;

private:
	// 최대 속도는 XZ,Y 로 나뉘어 진다. XZ에 들어가있는 값은 X에 있는 값을 사용한다.
	_vector					m_vMaxVelocity;

private:
	PHYSXCOLLIDERDESC		m_PhysXColliderDesc;
	PxRigidDynamic*			m_pRigidDynamic = nullptr;
	PxRigidStatic*			m_pRigidStatic = nullptr;

	vector<PxConvexMesh*>	m_ConvexMeshes;
	vector<PxTriangleMesh*>	m_TriangleMesh;

	_bool					m_bPickState = false;
	_bool					m_bPickable = true;
	_bool					m_bYFixed = false;


	vector<PxGeometry*>		m_pGeometry;
	vector<PxShape*>		m_pShape;
	PxFilterData			m_FilterData;

private: /* For. Collision */
	_bool										m_isColl = false;
	list<weak_ptr<CPhysXCollider>>				m_pPreOtherColliders;
	list<weak_ptr<CPhysXCollider>>				m_pOtherColliders;

public:
	void		CreatePhysXActor(PHYSXCOLLIDERDESC& PhysXColliderDesc);
	void		Add_PhysXActorAtSceneWithOption(const PxVec3& In_MassSpaceInertiaTensor = { 0.f, 0.f, 0.f }, const PxReal In_fMass = 0.f);
	void		Add_PhysXActorAtScene();

private:
	void		Create_Geometry();
	void		Create_DynamicActor(PHYSXCOLLIDERDESC& PhysXColliderDesc, PxTransform Transform);
	void		Create_StaticActor(PHYSXCOLLIDERDESC& PhysXColliderDesc, PxTransform Transform);

public:
	void OnCollision(weak_ptr<CPhysXCollider> pOtherCollider);
	void End_CollisionCheck();
	void PhysXCollisionEnter(weak_ptr<CPhysXCollider> pOtherCollider);
	void PhysXCollisionStay(weak_ptr<CPhysXCollider> pOtherCollider);
	void PhysXCollisionExit(weak_ptr<CPhysXCollider> pOtherCollider);

	virtual void OnEnable(void* pArg) override;
	virtual void OnDisable() override;

	virtual void OnDestroy() override;

private:
	void Free();
};

END