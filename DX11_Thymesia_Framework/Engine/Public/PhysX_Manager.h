#pragma once
#include "Base.h"
#include "PxSimulationEventCallback.h"

BEGIN(Engine)

class CPhysX_Manager final : public CBase
{
	DECLARE_SINGLETON(CPhysX_Manager)

public:
	//enum PxShape
	//{
	//	SPHERE,
	//	BOX,


	//	SHAPE_END
	//}PXSHAPE;

	enum Scene
	{
		SCENE_CURRENT,
		SCENE_NEXT,
		SCENE_BEFORE,

		SCENE_END
	};

	typedef enum tagColliderShape
	{
		SHAPE_CHEESE,

		SHAPE_END
	}COLSHAPE;

public:
	PxConvexMesh*	Get_ShapeTemplate(COLSHAPE eShape) { m_ShapeTemplate[eShape]; }
	void			Set_ShapeSphere(PxSphereGeometry* pGeometry) { m_pSphere = pGeometry; }
	void			Set_ShapeBox(PxBoxGeometry* pGeometry) { m_pBox = pGeometry; }

	void			Begin_PhysScene() { m_bSceneStart = true; }
	void			End_PhysScene() { m_bSceneStart = false; }

public:
	HRESULT	Initialize();
	void	Tick(_float fTimeDelta);

public:
	void			Check_PhysXFilterGroup(const _uint In_iLeftLayer, const _uint In_iRightLayer);
	_uint			Get_PhysXFilterGroup(const _uint In_iIndex);

public:
	// Transform 에는 (float3)위치와 (float4)쿼터니온이 들어간다.
	HRESULT			Create_Scene(Scene eScene, PxVec3 Gravity = PxVec3(0.0f, -9.81f, 0.0f));
	HRESULT			Delete_Scene(Scene eScene);
	HRESULT			Change_Scene(Scene eNextScene, PxVec3 Gravity = PxVec3(0.0f, -9.81f, 0.0f));

	PxRigidDynamic* Create_DynamicActor(const PxTransform& t, const PxGeometry& geometry, PxMaterial* pMaterial = nullptr);
	PxRigidDynamic* Create_DynamicActor(const PxTransform& t);
	PxRigidStatic*	Create_StaticActor(const PxTransform& t, const PxGeometry& geometry, PxMaterial* pMaterial = nullptr);
	PxRigidStatic*	Create_StaticActor(const PxTransform& t);
	
	void			Add_DynamicActorAtCurrentScene(PxRigidDynamic& DynamicActor, const PxReal& Density, const PxVec3& In_MassSpaceInertiaTensor);
	void			Add_StaticActorAtCurrentScene(PxRigidStatic& StaticActor);

	void			Create_CylinderMesh(_float fRadiusBelow, _float fRadiusUpper, _float fHight, PxConvexMesh** ppOut);
	void			Create_ConvexMesh(PxVec3** pVertices, _uint iNumVertice, PxConvexMesh** ppOut);
	void			Create_Material(_float fStaticFriction, _float fDynamicFriction, _float fRestitution, PxMaterial** ppOut);
	void			Create_Shape(const PxGeometry & Geometry, PxMaterial* pMaterial, const PxShapeFlags In_ShapeFlags, PxShape ** ppOut);
	void			Create_MeshFromTriangles(const PxTriangleMeshDesc& In_MeshDesc, PxTriangleMesh** ppOut);


private:
	// Foundation을 생성하는데 필요한 변수
	PxDefaultAllocator		m_Allocator;
	PxDefaultErrorCallback	m_ErrorCallback;
	PxFoundation*			m_pFoundation;

	// PxFoundation이 있어야 Physics를 생성할 수 있다.
	PxPhysics*				m_pPhysics;

	// CPU 리소스를 효율적으로 공유할 수 있도록 하기 위해 구현하는 것을 추천
	PxDefaultCpuDispatcher*	m_pDispatcher = nullptr;

	// MeshCooking을 하기 위해 생성
	PxCooking*				m_pCooking = nullptr;


	// Scene
	//_uint					m_iNumScenes = 0;
	//typedef PxScene*		LPSCENE;
	PxScene*				m_pScenes[SCENE_END];
	PxScene*				m_pCurScene = nullptr;

	// m_pPhysics를 사용해 createMaterial해서 사용
	// 충돌체 마찰력, Dynamic 마찰력, 탄성력을 지정하여 사용
	PxMaterial*				m_pMaterial;

	////Visual Debugger
	PxPvd*					m_pPVD;

	PxRigidDynamic*			m_pTemp = nullptr;


private:
	PxBoxGeometry*			m_pBox = nullptr;
	PxSphereGeometry*		m_pSphere = nullptr;

	PxConvexMesh*			m_ShapeTemplate[SHAPE_END];
	_bool					m_bSceneStart = false;

private: /* For. Filter */
	vector<_uint>			m_arrCheck;


public:
	//CollisionSimulationEventCallBack* m_pCollisionSimulationEventCallBack;

private:
	//void			Create_CheeseShape();
	//void			Create_Stack(const PxTransform& t, PxU32 size, PxReal halfExtent);



public:
	virtual void OnDestroy() override;

private:
	void Free();
};

END