#include "..\Public\PhysX_Manager.h"
#include "GameInstance.h"
#include "CollisionSimulationEventCallBack.h"
#include "PhysXCollider.h"
#include "PhysXController.h"

IMPLEMENT_SINGLETON(CPhysX_Manager)

void CPhysX_Manager::Register_PhysXCollider(weak_ptr<CPhysXCollider> pPhysXCollider)
{
	m_pPhysXCollders.emplace(pPhysXCollider.lock()->Get_PColliderIndex(), pPhysXCollider);
}

weak_ptr<CPhysXCollider> CPhysX_Manager::Find_PhysXCollider(const _uint In_iPhysXColliderIndex)
{
	auto iter = m_pPhysXCollders.find(In_iPhysXColliderIndex);

	if (iter != m_pPhysXCollders.end())
	{
		return iter->second;
	}

	return weak_ptr<CPhysXCollider>();
}

void CPhysX_Manager::Register_PhysXController(weak_ptr<CPhysXController> pPhysXController)
{
	m_pPhysXControllers.emplace(pPhysXController.lock()->Get_PControllerIndex(), pPhysXController);
}

weak_ptr<CPhysXController> CPhysX_Manager::Find_PhysXController(const _uint In_iPhysXControllerIndex)
{
	auto iter = m_pPhysXControllers.find(In_iPhysXControllerIndex);

	if (iter != m_pPhysXControllers.end())
	{
		return iter->second;
	}

	return weak_ptr<CPhysXController>();
}

void CPhysX_Manager::Set_CurrentCameraControllerIndex(const _uint In_iPhysXCurrentCameraControllerIndex)
{
	m_iCurrentCameraIndex = (_int)In_iPhysXCurrentCameraControllerIndex;
}

HRESULT CPhysX_Manager::Initialize(const _uint In_iNumLayer)
{
	m_arrCheck.reserve(In_iNumLayer);

	for (_uint i = 0; i < In_iNumLayer; ++i)
	{
		m_arrCheck.emplace_back(0);
	}

	ZeroMemory(m_pScenes, sizeof(PxScene*) * SCENE_END);

	// Create Foundation
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);
	PxRevoluteJointCreate;
	// Create PVD
	char* strTransport = "127.0.0.1";
	m_pPVD = PxCreatePvd(*m_pFoundation);
	PxPvdTransport* Transport = PxDefaultPvdSocketTransportCreate(strTransport, 5425, 10);
	_bool	bPVDConnectionResult = m_pPVD->connect(*Transport, PxPvdInstrumentationFlag::eALL);
	if (!bPVDConnectionResult)
	{
		//MSG_BOX("Faiied to connect to PVD!");
	}

	// Create PhysX
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, m_pPVD);

	// Create Cooking
	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, PxCookingParams(PxTolerancesScale()));

	// Create Cuda
	PxCudaContextManagerDesc tCudaDesc;
	tCudaDesc.graphicsDevice = DEVICE;
	tCudaDesc.interopMode = PxCudaInteropMode::Enum::D3D11_INTEROP;
	tCudaDesc.ctx = GET_SINGLE(CCuda_Device)->Get_CudaContext();

	m_pCudaContextManager = PxCreateCudaContextManager(*m_pFoundation, tCudaDesc, PxGetProfilerCallback());

	if (m_pCudaContextManager)
	{
		if (!m_pCudaContextManager->contextIsValid())
		{
			if(m_pCudaContextManager)
				m_pCudaContextManager->release();
			m_pCudaContextManager = nullptr;
		}
	}
	else
	{
		//DEBUG_ASSERT;
	}
	// Crate Material
	// 충돌체 정지 마찰계수, 운동 마찰 계수, 탄성력
	//m_pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	m_pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, -10.f);

	//m_pCollisionSimulationEventCallBack = DBG_NEW CollisionSimulationEventCallBack();
	//if (nullptr != m_pScenes)
	//	__debugbreak();

	//m_iNumScenes = iNumLevels;
	//m_pScenes = new LPSCENE[iNumLevels];

	// TODO : for test [ Create Scene ]
	//Create_Scene(SCENE_CURRENT);
	//m_pCurScene = m_pScenes[SCENE_CURRENT];



	//// Create Plane
	//PxRigidStatic* groundPlane = PxCreatePlane(*m_pPhysics, PxPlane(0, 1, 0, 0), *m_pMaterial);
	//m_pScenes[SCENE_CURRENT]->addActor(*groundPlane);

	//PxReal stackZ = 0.f;
	//for (PxU32 i = 0; i<3; i++)
	//	Create_Stack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);

	//Create_StaticActor(PxTransform(PxVec3(0.f, 10.f, 0.f)), PxBoxGeometry(5.f, 5.f, 5.f), SCENE_CURRENT);
	//Create_DynamicActor(PxTransform(PxVec3(0.f, 0.f, 0.f)), PxBoxGeometry(5.f, 5.f, 5.f), SCENE_CURRENT, 10.f, PxVec3(0.f,0.f,0.f));



	return S_OK;
}


void CPhysX_Manager::Tick(_float fTimeDelta)
{

	if (m_fTimeAcc > 1.f)
	{
		Garbage_Collector();
		m_fTimeAcc = 0.f;
	}
	else
	{
		m_fTimeAcc += fTimeDelta;
	}



	if (m_pCurScene)
	{
		if (fTimeDelta > 3.f)
		{
			return;
		}

		if (1 == m_pCurScene->getTimestamp() ||
			2 == m_pCurScene->getTimestamp())
			fTimeDelta = 0.16f;

		m_pCurScene->simulate(fTimeDelta);
		m_pCurScene->fetchResults(true);
	}

	//PxTransform camera = {0.f, 0.f, 0.f};

	//USEGAMEINSTANCE;
	//if (KEY_DOWN('K'))
	//{
	//	//m_pTemp = CreateDynamicActor(camera, PxSphereGeometry(3.0f), SCENE_FIRST, 10.f, camera.rotate(PxVec3(0, 0, -1)) * 200);



	//	PxConvexMesh* pConvexMesh = nullptr;
	//	Create_CylinderMesh(3.f, 1.f, 4.f, &pConvexMesh);

	//	m_pTemp = Create_DynamicActor(camera, PxConvexMeshGeometry(pConvexMesh), SCENE_CURRENT, 10.f, camera.rotate(PxVec3(0, 0, -1)) * 200);
	//}

	//if (KEY_DOWN('L') && m_pTemp)
	//{
	//	m_pTemp->setGlobalPose(PxTransform(0.f, 0.f, 0.f));
	//	m_pTemp->getGlobalPose();
	//}

	//if (m_pTemp)
	//{
	//	PxVec3 TempPos = m_pTemp->getGlobalPose().p;
	//	_float3 vTempPos = { TempPos.x, TempPos.y ,TempPos.z };
	//}


}



void CPhysX_Manager::Check_PhysXFilterGroup(const _uint In_iLeftLayer, const _uint In_iRightLayer)
{
	_uint iRow = (_uint)In_iLeftLayer; // 행
	_uint iCol = (_uint)In_iRightLayer; // 열

	_uint iMax = iCol;
	if (iRow > iCol)
	{
		iMax = iRow;
	}

	// 공간 할당
	for (_uint i = (_uint)m_arrCheck.size(); i <= iMax; ++i)
	{
		m_arrCheck.emplace_back(0);
	}

	if (m_arrCheck[iRow] & (1 << iCol)) // 이미 그 자리가 1이면
	{
		m_arrCheck[iRow] &= ~(1 << iCol); // 빼주고
		m_arrCheck[iCol] &= ~(1 << iRow); // 빼주고
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol);
		m_arrCheck[iCol] |= (1 << iRow);
	}

}

_uint CPhysX_Manager::Get_PhysXFilterGroup(const _uint In_iIndex)
{
	return m_arrCheck[In_iIndex];
}


//// let triggers through
	//if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	//{
	//	pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
	//	return PxFilterFlag::eDEFAULT;

	//	//cout << "Is Exit? filterData0.word2 : " << filterData0.word2 << endl;
	//	//cout << "Is Exit? filterData1.word2 : " << filterData1.word2 << endl;
	//}
	//// generate contacts for all that were not filtered above
	//pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	//// trigger the contact callback for pairs (A,B) where 
	//// the filtermask of A contains the ID of B and vice versa.
	//if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	//	pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	//return PxFilterFlag::eDEFAULT;

	// let triggers through
	//if (filterData0.word2 == 2 && filterData1.word2 == 2)
	//{
	//	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	//	{
	//		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
	//		return PxFilterFlag::eDEFAULT;
	//	}
	//	else
	//	{
	//		return PxFilterFlag::eKILL;
	//	}
	//	//cout << "Is Exit? filterData0.word2 : " << filterData0.word2 << endl;
	//	//cout << "Is Exit? filterData1.word2 : " << filterData1.word2 << endl;
	//}

	//else if (filterData0.word2 == 0 && filterData1.word2 == 0)
	//{
	//	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	//	return PxFilterFlag::eDEFAULT;
	//}

	//else
	//{
	//	//pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	//	return PxFilterFlag::eKILL;
	//}

	// trigger the contact callback for pairs (A,B) where 
	// the filtermask of A contains the ID of B and vice versa.

PxFilterFlags CollisionFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	if (filterData0.word2 != 1 || filterData1.word2 != 1)
	{
		return PxFilterFlag::eKILL;
	}

	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	{

		pairFlags |= PxPairFlag::eDETECT_DISCRETE_CONTACT;
		pairFlags |= PxPairFlag::eCONTACT_DEFAULT;
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
		return PxFilterFlag::eDEFAULT;
	}
	else
	{
		return PxFilterFlag::eKILL;
	}

	return PxFilterFlag::eDEFAULT;
}

HRESULT CPhysX_Manager::Create_Scene(Scene eScene, PxVec3 Gravity)
{
	// Set Scene
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = Gravity;

	// Set Dispatcher
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = CollisionFilterShader;
	sceneDesc.simulationEventCallback = GET_SINGLE(CCollision_Manager)->Get_CollisionSimulationEventCallBack();
	sceneDesc.cudaContextManager = m_pCudaContextManager;
	sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;
	sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;	//Enable GPU dynamics - without this enabled, simulation (contact gen and solver) will run on the CPU.
	sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;			//Enable PCM. PCM NP is supported on GPU. Legacy contact gen will fall back to CPU
	sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;	//Improve solver stability by enabling post-stabilization.
	sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;		//Enable GPU broad phase. Without this set, broad phase will run on the CPU.
	sceneDesc.gpuMaxNumPartitions = 8;						//Defines the maximum number of partitions used by the solver. Only power-of-2 values are valid. 
															//A value of 8 generally gives best balance between performance and stability.

	m_pScenes[eScene] = m_pPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = m_pScenes[eScene]->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	m_pScenes[eScene]->setSimulationEventCallback(GET_SINGLE(CCollision_Manager)->Get_CollisionSimulationEventCallBack());
	//m_pScenes[eScene]->setContactModifyCallback(GET_SINGLE(CCollision_Manager)->Get_CollisionCallBack());

	m_pCurScene = m_pScenes[eScene];

	if (m_pControllerManager)
		m_pControllerManager->release();



	m_pControllerManager = PxCreateControllerManager(*m_pCurScene);

	//PxSceneDesc	SceneDesc(m_pPhysics->getTolerancesScale());

	//// 중력
	//SceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	//SceneDesc.cpuDispatcher = m_pDispatcher;

	//// 시뮬레이션 중 출돌 관련 이벤트 함수가 정의된 클래스를 주면 된다
	////SceneDesc.simulationEventCallback = this;

	//// 시뮬레이션 중 사용할 필터 함수를 주면 된다.
	////SceneDesc.filterShader = FilterShader;

	//m_pScene = m_pPhysics->createScene(SceneDesc);

	//// 해당 Flag를 켜주면 Kineamtic Actor 의 충돌 검사가 가능하다
	//// 기본으로는 이벤트가 날라오지 않는다.

	////m_pScene->setFlag(PxSceneFlag::eENABLE_KINEMATIC_PAIRS, true);
	////m_pScene->setFlag(PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS, true);

	//// PVD
	//PxPvdSceneClient* pPvdClient = m_pScene->getScenePvdClient();
	//if (pPvdClient)
	//{
	//	pPvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
	//	pPvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
	//	pPvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	//}

	//// 바닥 만들기
	//// 충돌 재질을 생성한다.
	//// 각 파라미터는 마찰력과 탄성에 관여한다.
	//// 정지마찰, 운동 마찰
	//PxMaterial*	pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);
	//PxRigidStatic*	pGroundPlane = PxCreatePlane(*m_pPhysics, PxPlane(0, 1, 0, 0), *pMaterial);

	//m_pScene->addActor(*pGroundPlane);

	return S_OK;
}

HRESULT CPhysX_Manager::Delete_Scene(Scene eScene)
{
	if (m_pScenes[eScene])
		m_pScenes[eScene]->release();
	m_pScenes[eScene] = nullptr;

	if (m_pDispatcher)
		m_pDispatcher->release();
	m_pDispatcher = nullptr;
	return S_OK;
}

HRESULT CPhysX_Manager::Change_Scene(Scene eNextScene, PxVec3 Gravity)
{
	m_pCurScene->release();
	Create_Scene(eNextScene, Gravity);
	m_pCurScene = m_pScenes[eNextScene];
	return S_OK;
}

PxRigidDynamic* CPhysX_Manager::Create_DynamicActor(const PxTransform& Transform, const PxGeometry& Geometry, PxMaterial* pMaterial)
{
	return PxCreateDynamic(*m_pPhysics, Transform, Geometry, *pMaterial, 10.f);

	//m_pPhysics->createRigidDynamic(Transform);

}

PxRigidDynamic* CPhysX_Manager::Create_DynamicActor(const PxTransform& t)
{
	return m_pPhysics->createRigidDynamic(t);
}

PxRigidStatic* CPhysX_Manager::Create_StaticActor(const PxTransform& Transform, const PxGeometry& Geometry, PxMaterial* pMaterial)
{
	return PxCreateStatic(*m_pPhysics, Transform, Geometry, *pMaterial);
}

PxRigidStatic* CPhysX_Manager::Create_StaticActor(const PxTransform& t)
{
	return m_pPhysics->createRigidStatic(t);
}

void CPhysX_Manager::Add_DynamicActorAtCurrentScene(PxRigidDynamic& DynamicActor)
{
	//PxRigidBodyExt::updateMassAndInertia(DynamicActor, Density);
	m_pCurScene->addActor(DynamicActor);
}

void CPhysX_Manager::Add_StaticActorAtCurrentScene(PxRigidStatic& StaticActor)
{
	m_pCurScene->addActor(StaticActor);
}

//void CPhysX_Manager::Create_Stack(const PxTransform & t, PxU32 size, PxReal halfExtent)
//{
//	PxShape* shape = m_pPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *m_pMaterial);
//	for (PxU32 i = 0; i<size; i++)
//
//	{
//		for (PxU32 j = 0; j<size - i; j++)
//		{
//			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
//			PxRigidDynamic* body = m_pPhysics->createRigidDynamic(t.transform(localTm));
//			//PxRigidStatic* body = m_pPhysics->createRigidStatic(t.transform(localTm));
//			body->attachShape(*shape);
//			// 해당 메시의 관성을 넣어줌
//			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
//			m_pCurScene->addActor(*body);
//		}
//	}
//	shape->release();
//}

void CPhysX_Manager::Create_ConvexMesh(PxVec3** pVertices, _uint iNumVertice, PxConvexMesh** ppOut)
{
	PxCookingParams params = m_pCooking->getParams();

	// Use the new (default) PxConvexMeshCookingType::eQUICKHULL
	params.convexMeshCookingType = PxConvexMeshCookingType::eQUICKHULL;

	// If the gaussMapLimit is chosen higher than the number of output vertices, no gauss map is added to the convex mesh data (here 256).
	// If the gaussMapLimit is chosen lower than the number of output vertices, a gauss map is added to the convex mesh data (here 16).
	params.gaussMapLimit = 16;
	params.buildGPUData = true;
	m_pCooking->setParams(params);

	// Setup the convex mesh descriptor
	PxConvexMeshDesc Desc;

	// We provide points only, therefore the PxConvexFlag::eCOMPUTE_CONVEX flag must be specified
	Desc.points.data = *pVertices;
	Desc.points.count = iNumVertice;
	Desc.points.stride = sizeof(PxVec3);
	Desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

	*ppOut = m_pCooking->createConvexMesh(Desc, m_pPhysics->getPhysicsInsertionCallback());
}

void CPhysX_Manager::Create_ConvexMesh(const PxConvexMeshDesc& In_MeshDesc, PxConvexMesh** ppOut)
{
	PxCookingParams params = m_pCooking->getParams();

	// Use the new (default) PxConvexMeshCookingType::eQUICKHULL
	params.convexMeshCookingType = PxConvexMeshCookingType::eQUICKHULL;

	// If the gaussMapLimit is chosen higher than the number of output vertices, no gauss map is added to the convex mesh data (here 256).
	// If the gaussMapLimit is chosen lower than the number of output vertices, a gauss map is added to the convex mesh data (here 16).
	params.gaussMapLimit = 16;
	params.buildGPUData = true;
	m_pCooking->setParams(params);

	*ppOut = m_pCooking->createConvexMesh(In_MeshDesc, m_pPhysics->getPhysicsInsertionCallback());

}

void CPhysX_Manager::Create_Material(_float fStaticFriction, _float fDynamicFriction, _float fRestitution, PxMaterial** ppOut)
{
	*ppOut = m_pPhysics->createMaterial(fStaticFriction, fDynamicFriction, fRestitution);
}

void CPhysX_Manager::Create_Shape(const PxGeometry& Geometry, PxMaterial* pMaterial, const _bool isExculsive, const PxShapeFlags In_ShapeFlags, PxShape** ppOut)
{
	*ppOut = m_pPhysics->createShape(Geometry, *pMaterial, isExculsive, In_ShapeFlags);
}

void CPhysX_Manager::Create_MeshFromTriangles(const PxTriangleMeshDesc& In_MeshDesc, PxTriangleMesh** ppOut)
{
	PxCookingParams params = m_pCooking->getParams();

	// Use the new (default) PxConvexMeshCookingType::eQUICKHULL
	params.buildGPUData = true;

	m_pCooking->setParams(params);



	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;
	bool status = m_pCooking->cookTriangleMesh(In_MeshDesc, writeBuffer, &result);
	if (!status)
		DEBUG_ASSERT;

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	*ppOut = m_pPhysics->createTriangleMesh(readBuffer);
}

void CPhysX_Manager::Create_Controller(const PxCapsuleControllerDesc& In_ControllerDesc, PxController** ppOut)
{
	*ppOut = m_pControllerManager->createController(In_ControllerDesc);
}

void CPhysX_Manager::Garbage_Collector()
{
	// 사망한 객체 정리
	for (auto iter = m_pPhysXCollders.begin(); iter != m_pPhysXCollders.end();)
	{
		if (!(*iter).second.lock())
		{
			iter = m_pPhysXCollders.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	for (auto iter = m_pPhysXControllers.begin(); iter != m_pPhysXControllers.end();)
	{
		if (!(*iter).second.lock())
		{
			iter = m_pPhysXControllers.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CPhysX_Manager::Create_CylinderMesh(_float fRadiusBelow, _float fRadiusUpper, _float fHight, PxConvexMesh** ppOut)
{
	_uint	iNumVerts = 32;
	PxVec3* pVertices = DBG_NEW PxVec3[iNumVerts];

	// Below
	for (PxU32 i = 0; i < 16; i++)
	{
		//pVertices[i] = PxVec3(fRandom(-2.0f, 2.0f), fRandom(-2.0f, 2.0f), fRandom(-2.0f, 2.0f));

		_vector vPostion = { fRadiusBelow, 0.f, 0.f, 1.f };

		_matrix RotateMatrix = XMMatrixRotationAxis(_vector{ 0.f, 1.f, 0.f, 0.f }, XMConvertToRadians(360.f / 16 * i));
		vPostion = XMVector3TransformCoord(vPostion, RotateMatrix);
		pVertices[i] = PxVec3(XMVectorGetX(vPostion), -fHight * 0.5f, XMVectorGetZ(vPostion));
	}

	// Upper
	for (PxU32 i = 16; i < 32; i++)
	{
		_vector vPostion = { fRadiusUpper, 0.f, 0.f, 1.f };

		_matrix RotateMatrix = XMMatrixRotationAxis(_vector{ 0.f, 1.f, 0.f, 0.f }, XMConvertToRadians(360.f / 16 * i));
		vPostion = XMVector3TransformCoord(vPostion, RotateMatrix);
		pVertices[i] = PxVec3(XMVectorGetX(vPostion), fHight * 0.5f, XMVectorGetZ(vPostion));
	}

	Create_ConvexMesh(&pVertices, iNumVerts, ppOut);
	Safe_Delete_Array(pVertices);
}


//void CPhysX_Manager::Create_CheeseShape()
//{
//
//}

void CPhysX_Manager::OnDestroy()
{
	PX_UNUSED(true);

	if (m_pControllerManager)
		m_pControllerManager->release();

	for (auto& elem : m_pScenes)
	{
		if (elem)
			elem->release();
	}


	//m_pCurScene->release();
	if (m_pDispatcher)
		m_pDispatcher->release();

	if (m_pPhysics)
		m_pPhysics->release();

	if (m_pCooking)
		m_pCooking->release();

	if (m_pPVD)
	{
		PxPvdTransport* transport = m_pPVD->getTransport();
		m_pPVD->release();
		m_pPVD = nullptr;
		transport->release();
	}
	if (m_pCudaContextManager)
		m_pCudaContextManager->release();

	if (m_pFoundation)
		m_pFoundation->release();


	//m_pCollisionSimulationEventCallBack->Release();
}

void CPhysX_Manager::Free()
{

}


//const PxU32 numVerts = 64;
//PxVec3* vertices = new PxVec3[numVerts];
//
//// Prepare random verts
//for (PxU32 i = 0; i < numVerts; i++)
//{
//	vertices[i] = PxVec3(fRandom(-2.0f, 2.0f), fRandom(-2.0f, 2.0f), fRandom(-2.0f, 2.0f));
//}
//
//PxCookingParams params = m_pCooking->getParams();
//
//// Use the new (default) PxConvexMeshCookingType::eQUICKHULL
//params.convexMeshCookingType = PxConvexMeshCookingType::eQUICKHULL;
//
//// If the gaussMapLimit is chosen higher than the number of output vertices, no gauss map is added to the convex mesh data (here 256).
//// If the gaussMapLimit is chosen lower than the number of output vertices, a gauss map is added to the convex mesh data (here 16).
//params.gaussMapLimit = 16;
//m_pCooking->setParams(params);
//
//// Setup the convex mesh descriptor
//PxConvexMeshDesc Desc;
//
//// We provide points only, therefore the PxConvexFlag::eCOMPUTE_CONVEX flag must be specified
//Desc.points.data = vertices;
//Desc.points.count = numVerts;
//Desc.points.stride = sizeof(PxVec3);
//Desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
//
//PxU32 meshSize = 0;
//PxConvexMesh* convex = NULL;
//
//convex = m_pCooking->createConvexMesh(Desc, m_pPhysics->getPhysicsInsertionCallback());
//
//delete[] vertices;