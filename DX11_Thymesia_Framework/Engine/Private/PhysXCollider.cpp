#include "..\Public\PhysXCollider.h"
#include "GameInstance.h"
#include "PhysX_Manager.h"
#include "Transform.h"
#include "SMath.h"
#include "GameObject.h"

GAMECLASS_C(CPhysXCollider);
IMPLEMENT_CLONABLE(CPhysXCollider, CComponent);

_uint CPhysXCollider::m_iClonedColliderIndex = 0;

HRESULT CPhysXCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPhysXCollider::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	m_iColliderIndex = m_iClonedColliderIndex++;

	if (nullptr != pArg)
	{
		ZeroMemory(m_ConvexMeshes, sizeof(PxConvexMesh*) * 16);

		memcpy(&m_PhysXColliderDesc, pArg, sizeof(PHYSXCOLLIDERDESC));

		CreatePhysXActor(m_PhysXColliderDesc);
	}

	GET_SINGLE(CPhysX_Manager)->Register_PhysXCollider(Weak_StaticCast<CPhysXCollider>(m_this));

	return S_OK;
}

void CPhysXCollider::Start()
{

}

void CPhysXCollider::Init_MeshCollider(weak_ptr<MESH_DATA> pMeshData)
{
	PxU32 iNumVertices = pMeshData.lock()->iNumVertices;
	PxU32 iNumFaces = pMeshData.lock()->iNumFaces;
	PxVec3* pVertices = DBG_NEW PxVec3[iNumVertices];
	FACEINDICES32* pIndices = DBG_NEW FACEINDICES32[iNumFaces];



	SMath::Convert_PxVec3FromMeshData(pVertices, pMeshData);

	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = iNumVertices;
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = pVertices;


	for (PxU32 i = 0; i < iNumFaces; ++i)
	{
		memcpy(&pIndices[i], &pMeshData.lock()->pIndices[i], sizeof(FACEINDICES32));
	}

	meshDesc.triangles.count = iNumFaces;
	meshDesc.triangles.stride = 3 * sizeof(PxU32);
	meshDesc.triangles.data = pIndices;

	GET_SINGLE(CPhysX_Manager)->Create_MeshFromTriangles(meshDesc, &m_TriangleMesh);


	/*PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = nbVerts;
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = physx::PxTriangle::verts;

	meshDesc.triangles.count = triCount;
	meshDesc.triangles.stride = 3 * sizeof(PxU32);
	meshDesc.triangles.data = indices32;

	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;
	bool status = cooking.cookTriangleMesh(meshDesc, writeBuffer, result);
	if (!status)
		return NULL;

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	return physics.createTriangleMesh(readBuffer);*/

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);
}

void CPhysXCollider::Synchronize_Transform(weak_ptr<CTransform> pTransform, _fvector In_vOffset)
{
	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	_vector vPos = { Transform.p.x, Transform.p.y, Transform.p.z };
	vPos += In_vOffset;
	vPos.m128_f32[3] = 1.f;
	_vector vQuaternion = { Transform.q.x, Transform.q.y, Transform.q.z, Transform.q.w };
	pTransform.lock()->Set_State(CTransform::STATE_TRANSLATION, vPos);
	pTransform.lock()->Rotation_Quaternion(vQuaternion);
}

void CPhysXCollider::Synchronize_Transform_Position(weak_ptr<CTransform> pTransform)
{
	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	_vector vPos = { Transform.p.x, Transform.p.y, Transform.p.z };
	pTransform.lock()->Set_State(CTransform::STATE_TRANSLATION, vPos);
}

void CPhysXCollider::Synchronize_Transform_Rotation(weak_ptr<CTransform> pTransform)
{
	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	_vector vQuaternion = { Transform.q.x, Transform.q.y, Transform.q.z, Transform.q.w };
	pTransform.lock()->Rotation_Quaternion(vQuaternion);
}

void CPhysXCollider::Synchronize_Collider(weak_ptr<CTransform> pTransform, _fvector In_vOffset)
{
	_vector vPos = pTransform.lock()->Get_State(CTransform::STATE_TRANSLATION);
	vPos += In_vOffset;
	vPos.m128_f32[3] = 1.f;
	_vector vQuaternion = XMQuaternionRotationMatrix(SMath::Get_RotationMatrix(pTransform.lock()->Get_WorldMatrix()));
	Set_Position(vPos, vQuaternion);
}

void CPhysXCollider::PutToSleep()
{
	if (m_pRigidDynamic)
		m_pRigidDynamic->putToSleep();
}

void CPhysXCollider::WakeUp()
{
	if (m_pRigidDynamic)
		m_pRigidDynamic->wakeUp();
}

void CPhysXCollider::Attach_Shape(PxShape * pShape)
{
	if (m_pRigidDynamic)
		m_pRigidDynamic->attachShape(*pShape);
}

void CPhysXCollider::Detach_Shape(PxShape * pShape)
{
	if (m_pRigidDynamic)
		m_pRigidDynamic->detachShape(*pShape);
}

HRESULT CPhysXCollider::Add_Force(_vector _vForce)
{
	PxVec3	vForce;
	vForce = { XMVectorGetX(_vForce), XMVectorGetY(_vForce), XMVectorGetZ(_vForce) };

	if (m_pRigidDynamic)
		m_pRigidDynamic->addForce(vForce);

	PxVec3	vMaxVel = { XMVectorGetX(m_vMaxVelocity), XMVectorGetY(m_vMaxVelocity), XMVectorGetZ(m_vMaxVelocity) };
	PxVec3	vVelocity = m_pRigidDynamic->getLinearVelocity();

	_vector	XDir = { 1.f, 0.f, 0.f, 0.f };
	_vector	ZDir = { 0.f, 1.f, 0.f, 0.f }; //vector2Dot으로 계산 하기 때문

	_vector	VelocityVec2 = { vVelocity.x, vVelocity.z, 0.f, 0.f };

	_vector vNormalizedVelocity = XMVector2Normalize(VelocityVec2);

	//m_vMaxVelocity
	_vector	vSpeed = XMVector2Length(VelocityVec2);
	_float	fSpeed = min(XMVectorGetX(vSpeed), XMVectorGetX(m_vMaxVelocity));

	vNormalizedVelocity *= fSpeed;

	_vector	XSpeed = XDir * XMVectorGetX(XMVector2Dot(XDir, vNormalizedVelocity));
	_vector	ZSpeed = ZDir * XMVectorGetX(XMVector2Dot(ZDir, vNormalizedVelocity));

	vVelocity.x = XMVectorGetX(XSpeed);
	vVelocity.z = XMVectorGetY(ZSpeed);
	//vVelocity.x = min(fabsf(vVelocity.x), vMaxVel.x) * vVelocity.x / vVelocity.x;
	//vVelocity.y = min(fabsf(vVelocity.y), vMaxVel.y) * vVelocity.y / vVelocity.y;
	//vVelocity.z = min(fabsf(vVelocity.z), vMaxVel.z) * vVelocity.z / vVelocity.z;

	m_pRigidDynamic->setLinearVelocity(vVelocity);

	return S_OK;
}

HRESULT CPhysXCollider::Clear_Force()
{
	if (m_pRigidDynamic)
	{
		m_pRigidDynamic->clearForce(PxForceMode::eACCELERATION);
		m_pRigidDynamic->clearForce(PxForceMode::eFORCE);
		m_pRigidDynamic->clearForce(PxForceMode::eIMPULSE);
		m_pRigidDynamic->clearForce(PxForceMode::eVELOCITY_CHANGE);
	}
	//m_pRigidDynamic->setMassSpaceInertiaTensor(PxVec3(0.f));
	m_pRigidDynamic->putToSleep();
	m_pRigidDynamic->wakeUp();
	return S_OK;
}

HRESULT CPhysXCollider::Clear_Velocity()
{
	m_pRigidDynamic->setLinearVelocity(PxVec3(0.f));
	return S_OK;
}

HRESULT CPhysXCollider::Add_LinearVelocityResistance(_vector vResistanceRate)
{
	PxVec3	vResistRate = {XMVectorGetX(vResistanceRate), XMVectorGetY(vResistanceRate), XMVectorGetZ(vResistanceRate) };
	PxVec3	vVelocity = m_pRigidDynamic->getLinearVelocity();
	vVelocity.x *= vResistRate.x;
	vVelocity.y *= vResistRate.y;
	vVelocity.z *= vResistRate.z;

	m_pRigidDynamic->setLinearVelocity(vVelocity);

	return S_OK;
}

_vector CPhysXCollider::Get_Position()
{
	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	return XMVectorSet(Transform.p.x, Transform.p.y, Transform.p.z, 1.f);
}

_vector CPhysXCollider::Get_Velocity()
{
	if (m_pRigidDynamic)
	{
		PxVec3 Velocity = m_pRigidDynamic->getLinearVelocity();
		return XMVectorSet(Velocity.x, Velocity.y, Velocity.z, 0.f);
	}

	return _vector{ 0.f, 0.f, 0.f, 0.f };
}

_float CPhysXCollider::Get_Mess()
{
	assert(m_pRigidDynamic);

	PxReal fMess = m_pRigidDynamic->getMass();
	return fMess;
}

_vector CPhysXCollider::Get_AngularVelocity()
{
	assert(m_pRigidDynamic);

	PxVec3 vVelocity = m_pRigidDynamic->getAngularVelocity();
	return XMVectorSet(vVelocity.x, vVelocity.y, vVelocity.z, 0.f);
}

_vector CPhysXCollider::Get_LinearVelocity()
{
	assert(m_pRigidDynamic);

	PxVec3 vVelocity = m_pRigidDynamic->getLinearVelocity();
	return XMVectorSet(vVelocity.x, vVelocity.y, vVelocity.z, 0.f);
}

HRESULT CPhysXCollider::Set_Position(_vector _vPos, _vector _vQuaternion)
{
	PxTransform	Transform;
	PxVec3	vPos(XMVectorGetX(_vPos), XMVectorGetY(_vPos), XMVectorGetZ(_vPos));
	PxQuat	vQuaternion(XMVectorGetX(_vQuaternion), XMVectorGetY(_vQuaternion), XMVectorGetZ(_vQuaternion), XMVectorGetW(_vQuaternion));

	Transform.p = vPos;
	Transform.q = vQuaternion;

	if (m_pRigidDynamic)
		m_pRigidDynamic->setGlobalPose(Transform);

	if (m_pRigidStatic)
		m_pRigidStatic->setGlobalPose(Transform);

	return S_OK;
}

HRESULT CPhysXCollider::Set_Position(_vector _vPos)
{
	PxTransform	Transform;
	PxVec3	vPos(XMVectorGetX(_vPos), XMVectorGetY(_vPos), XMVectorGetZ(_vPos));

	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	Transform.p = vPos;

	if (m_pRigidDynamic)
		m_pRigidDynamic->setGlobalPose(Transform);

	if (m_pRigidStatic)
		m_pRigidStatic->setGlobalPose(Transform);

	return S_OK;
}

void CPhysXCollider::Set_Scale(_vector vScale)
{
	//const PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE;

	////PxShape* pShape[1];
	////ZeroMemory(pShape, sizeof(PxShape) * 16);
	//if (m_pRigidDynamic)
	//{
	//	//m_pRigidDynamic->getShapes(pShape, 1);
	//	m_pRigidDynamic->release();
	//	m_pRigidDynamic = nullptr;

	//	m_PhysXColliderDesc.vScale = vScale;
	//	CreatePhysXActor(m_PhysXColliderDesc);

	//	//m_pRigidDynamic->attachShape(*pShape[0]);

	//	for (size_t i = 0; i < 16; ++i)
	//	{
	//		if (nullptr != m_ConvexMeshes[i])
	//		{
	//			PxConvexMeshGeometry PxGeometry;
	//			PxGeometry.convexMesh = m_ConvexMeshes[i];
	//			PxMeshScale	vMeshScale;
	//			vMeshScale.scale.x = XMVectorGetX(vScale);
	//			vMeshScale.scale.y = XMVectorGetY(vScale);
	//			vMeshScale.scale.z = XMVectorGetZ(vScale);
	//			PxGeometry.scale = vMeshScale;
	//			PxShape*	pShape;
	//			PxMaterial*	pMaterial = m_PhysXColliderDesc.pMaterial;

	//			CPhysX_Manager::Get_Instance()->Create_Shape(PxGeometry, pMaterial, shapeFlags, &pShape);

	//			m_pRigidDynamic->attachShape(*pShape);
	//		}
	//	}
	//}

	//if (m_pRigidStatic)
	//{
	//	//m_pRigidStatic->getShapes(pShape, 16);
	//	m_pRigidStatic->release();
	//	m_pRigidStatic = nullptr;

	//	m_PhysXColliderDesc.vScale = vScale;
	//	CreatePhysXActor(m_PhysXColliderDesc);


	//	for (size_t i = 0; i < 16; ++i)
	//	{
	//		if (nullptr != m_ConvexMeshes[i])
	//		{
	//			PxConvexMeshGeometry PxGeometry;
	//			PxGeometry.convexMesh = m_ConvexMeshes[i];
	//			PxMeshScale	vMeshScale;
	//			vMeshScale.scale.x = XMVectorGetX(vScale);
	//			vMeshScale.scale.y = XMVectorGetY(vScale);
	//			vMeshScale.scale.z = XMVectorGetZ(vScale);
	//			PxGeometry.scale = vMeshScale;
	//			PxShape*	pShape;
	//			PxMaterial*	pMaterial = m_PhysXColliderDesc.pMaterial;

	//			CPhysX_Manager::Get_Instance()->Create_Shape(PxGeometry, pMaterial, shapeFlags, &pShape);

	//			m_pRigidStatic->attachShape(*pShape);
	//		}
	//	}
	//}
}

void CPhysXCollider::Set_ActorFlag(PxActorFlag::Enum eFlag, _bool bState)
{
	if (m_pRigidDynamic)
	{
		m_pRigidDynamic->setActorFlag(eFlag, bState);
	}
}


void CPhysXCollider::Delete_Collider()
{
	if (m_pRigidDynamic)
	{
		m_pRigidDynamic->release();
		m_pRigidDynamic = nullptr;
	}

	if (m_pRigidStatic)
	{
		m_pRigidStatic->release();
		m_pRigidStatic = nullptr;
	}
}

void CPhysXCollider::Create_Collider()
{
	if (!m_pRigidDynamic && !m_pRigidStatic)
	{
		CreatePhysXActor(m_PhysXColliderDesc);
	}
}

void CPhysXCollider::CreatePhysXActor(PHYSXCOLLIDERDESC& PhysXColliderDesc)
{
	m_PhysXColliderDesc = PhysXColliderDesc;

	PxTransform	Transform;
	Transform.p = PxVec3(
		XMVectorGetX(PhysXColliderDesc.vPosition),
		XMVectorGetY(PhysXColliderDesc.vPosition),
		XMVectorGetZ(PhysXColliderDesc.vPosition));

	_float4 vQuaternion;
	XMStoreFloat4(&vQuaternion, XMQuaternionRotationRollPitchYawFromVector(PhysXColliderDesc.vAngles));
	Transform.q = PxQuat(
		vQuaternion.x,
		vQuaternion.y,
		vQuaternion.z,
		vQuaternion.w);

	switch (PhysXColliderDesc.eActorType)
	{
	case PHYSXACTOR_TYPE::DYNAMIC:
	case PHYSXACTOR_TYPE::YFIXED_DYNAMIC:
		Create_DynamicActor(PhysXColliderDesc, Transform);
		break;
	case PHYSXACTOR_TYPE::STATIC:
		Create_StaticActor(PhysXColliderDesc, Transform);
		break;
	case PHYSXACTOR_TYPE::ZONE:

		break;
	default:
		//MSG_BOX("Failed to CreateActor : eCollidertype is wrong");
		break;
	}

	
}

void CPhysXCollider::Add_PhysXActorAtScene(const PxVec3& In_MassSpaceInertiaTensor)
{
	if (m_pRigidDynamic && m_pRigidStatic)
	{
		// 둘 다 존재하면 안된다.
		DEBUG_ASSERT;
	}

	else if (m_pRigidDynamic)
	{
		GET_SINGLE(CPhysX_Manager)->Add_DynamicActorAtCurrentScene(*m_pRigidDynamic, m_PhysXColliderDesc.fDensity, In_MassSpaceInertiaTensor);
		Safe_Delete(m_pGeometry);
	}

	else if (m_pRigidStatic)
	{
		GET_SINGLE(CPhysX_Manager)->Add_StaticActorAtCurrentScene(*m_pRigidStatic);
		Safe_Delete(m_pGeometry);
	}

	else
	{
		// 생성된 PhysXActor가 없음. Create부터 할 것.
		DEBUG_ASSERT;
	}

}

PxGeometry* CPhysXCollider::Create_Geometry()
{
	switch (m_PhysXColliderDesc.eShape)
	{
	case PHYSXCOLLIDER_TYPE::SPHERE:
		return DBG_NEW PxSphereGeometry(XMVectorGetX(m_PhysXColliderDesc.vScale));

	case PHYSXCOLLIDER_TYPE::BOX:
		return DBG_NEW PxBoxGeometry(XMVectorGetX(m_PhysXColliderDesc.vScale) * 0.5f,
			XMVectorGetY(m_PhysXColliderDesc.vScale) * 0.5f, 
			XMVectorGetZ(m_PhysXColliderDesc.vScale) * 0.5f);

	case PHYSXCOLLIDER_TYPE::CYLINDER:
	case PHYSXCOLLIDER_TYPE::CONVECMESH:
	{
		PxConvexMeshGeometry* pPxGeometry = DBG_NEW PxConvexMeshGeometry();
		pPxGeometry->convexMesh = m_PhysXColliderDesc.pConvecMesh;
		PxMeshScale	vScale;
		vScale.scale.x = XMVectorGetX(m_PhysXColliderDesc.vScale);
		vScale.scale.y = XMVectorGetY(m_PhysXColliderDesc.vScale);
		vScale.scale.z = XMVectorGetZ(m_PhysXColliderDesc.vScale);
		pPxGeometry->scale = vScale;
		return pPxGeometry;
	}

	case PHYSXCOLLIDER_TYPE::MESHDATA:
	{
		PxTriangleMeshGeometry* PxGeometry = DBG_NEW PxTriangleMeshGeometry();
		PxGeometry->triangleMesh = m_TriangleMesh;
		PxMeshScale	vScale;
		vScale.scale.x = XMVectorGetX(m_PhysXColliderDesc.vScale);
		vScale.scale.y = XMVectorGetY(m_PhysXColliderDesc.vScale);
		vScale.scale.z = XMVectorGetZ(m_PhysXColliderDesc.vScale);
		PxGeometry->scale = vScale;

		return PxGeometry;
	}

	case PHYSXCOLLIDER_TYPE::TYPE_END:
		MSG_BOX("Failed to create DynamicActor : eShape is wrong");
		break;
	default:
		break;
	}

	return nullptr;
}

void CPhysXCollider::Create_DynamicActor(PHYSXCOLLIDERDESC& tPhysXColliderDesc, PxTransform Transform)
{
	m_pRigidDynamic = GET_SINGLE(CPhysX_Manager)->Create_DynamicActor(Transform);
	m_pGeometry = Create_Geometry();
	PxShapeFlags shapeFlags;

	if (tPhysXColliderDesc.bTrigger)
	{
		shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE;
		m_FilterData.word2 = 2;

	}
	else
	{
		shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE;
		m_FilterData.word2 = 0;
	}

	GET_SINGLE(CPhysX_Manager)->Create_Shape(*m_pGeometry, m_PhysXColliderDesc.pMaterial, false, shapeFlags, &m_pShape);
	
	m_FilterData.word0 = (1 << m_PhysXColliderDesc.iFilterType);
	m_FilterData.word1 = GET_SINGLE(CPhysX_Manager)->Get_PhysXFilterGroup(m_PhysXColliderDesc.iFilterType);
	m_FilterData.word3 = m_iColliderIndex;
	
	//FilterData.word1 = m_PhysXColliderDesc.iFilterType;

	if (!m_pShape)
	{
		// Shape가 생성되지 않음.
		DEBUG_ASSERT;
	}

	m_pShape->setSimulationFilterData(m_FilterData);

	m_pRigidDynamic->attachShape(*m_pShape);
	m_pRigidDynamic->userData = &m_iColliderIndex;
}

void CPhysXCollider::Create_StaticActor(PHYSXCOLLIDERDESC& tPhysXColliderDesc, PxTransform Transform)
{
	m_pRigidStatic = GET_SINGLE(CPhysX_Manager)->Create_StaticActor(Transform);
	m_pGeometry = Create_Geometry();

	PxShapeFlags shapeFlags;

	if (tPhysXColliderDesc.bTrigger)
	{
		shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE;
		m_FilterData.word2 = 2;
	}
	else
	{
		shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE;
		m_FilterData.word2 = 0;
	}

	GET_SINGLE(CPhysX_Manager)->Create_Shape(*m_pGeometry, m_PhysXColliderDesc.pMaterial, false, shapeFlags, &m_pShape);

	m_FilterData.word0 = (1 << m_PhysXColliderDesc.iFilterType);
	m_FilterData.word1 = GET_SINGLE(CPhysX_Manager)->Get_PhysXFilterGroup(m_PhysXColliderDesc.iFilterType);
	

	if (!m_pShape)
	{
		// Shape가 생성되지 않음.
		DEBUG_ASSERT;
	}

	m_pShape->setSimulationFilterData(m_FilterData);

	m_pRigidStatic->attachShape(*m_pShape);
	m_pRigidStatic->userData = &m_iColliderIndex;
}

void CPhysXCollider::PhysXCollisionEnter(weak_ptr<CPhysXCollider> pOtherCollider)
{
	CallBack_CollisionEnter(pOtherCollider);
	m_pOwner.lock()->PhysXCollisionEnter(pOtherCollider);

	cout << "Shape is entering trigger volume\n";
}

void CPhysXCollider::PhysXCollisionStay(weak_ptr<CPhysXCollider> pOtherCollider)
{
	CallBack_CollisionStay(pOtherCollider);
	m_pOwner.lock()->PhysXCollisionStay(pOtherCollider);
}

void CPhysXCollider::PhysXCollisionExit(weak_ptr<CPhysXCollider> pOtherCollider)
{
	CallBack_CollisionExit(pOtherCollider);
	m_pOwner.lock()->PhysXCollisionExit(pOtherCollider);

	cout << "Shape is Exit trigger volume\n";

}

void CPhysXCollider::OnDestroy()
{
	if (m_pRigidDynamic)
		m_pRigidDynamic->release();
	if (m_pRigidStatic)
		m_pRigidStatic->release();
}

void CPhysXCollider::Free()
{
}
