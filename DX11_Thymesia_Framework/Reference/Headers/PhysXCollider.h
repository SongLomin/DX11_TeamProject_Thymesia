#pragma once
#include "Component.h"

BEGIN(Engine)
class CTransform;

class ENGINE_DLL CPhyXCollider final : public CComponent
{
	GAMECLASS_H(CPhyXCollider);
	SHALLOW_COPY(CPhyXCollider);
	DECLARE_CLONABLE(CPhyXCollider, CComponent);

public:
	typedef struct PhysXColliderDesc
	{
		PhysXColliderDesc() 
		{
			ZeroMemory(this, sizeof(PhysXColliderDesc));
		}

		PxConvexMesh*		pConvecMesh;
		PxMaterial*			pMaterial;
		PHYSXACTOR_TYPE		eType;
		XMVECTOR			vPosition;
		XMVECTOR			vAngles;
		PHYSXCOLLIDER_TYPE		eShape;
		XMVECTOR			vScale;
		float				fDensity;

	}PHYSXCOLLIDERDESC;

public:
	_vector	Get_Position();
	_vector	Get_Velocity();
	_float	Get_Mess();
	_vector	Get_AngularVelocity();
	_vector	Get_LinearVelocity();


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

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

public:
	void	Synchronize_Transform(weak_ptr<CTransform> pTransform);
	void	Synchronize_Transform_Position(weak_ptr<CTransform> pTransform);
	void	Synchronize_Transform_Rotation(weak_ptr<CTransform> pTransform);

	void	Synchronize_Collider(weak_ptr<CTransform> pTransform);

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

private:
	// 최대 속도는 XZ,Y 로 나뉘어 진다. XZ에 들어가있는 값은 X에 있는 값을 사용한다.
	_vector					m_vMaxVelocity;

private:
	PHYSXCOLLIDERDESC		m_ColliderDesc;
	PxRigidDynamic*			m_pRigidDynamic = nullptr;
	PxRigidStatic*			m_pRigidStatic = nullptr;

	PxConvexMesh*			m_ConvexMeshes[16];

	_bool					m_bPickState = false;
	_bool					m_bPickable = true;
	_bool					m_bYFixed = false;

private:
	void		CreatePhysActor(PHYSXCOLLIDERDESC PhysXColliderDesc);
	void		Create_DynamicActor(PHYSXCOLLIDERDESC PhysXColliderDesc, PxTransform Transform, PxConvexMesh* pConvexMesh = nullptr);
	void		Create_StaticActor(PHYSXCOLLIDERDESC PhysXColliderDesc, PxTransform Transform, PxConvexMesh* pConvexMesh = nullptr);

public:
	virtual void OnDestroy() override;

private:
	void Free();
};

END