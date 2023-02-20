#include "..\Public\PhysXCollider.h"
#include "GameInstance.h"
#include "PhysX_Manager.h"
#include "Transform.h"
#include "SMath.h"
#include "GameObject.h"
#include "ModelData.h"
#include "VIBuffer_Model_Instance.h"

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
		m_PhysXColliderDesc = *(PHYSXCOLLIDERDESC*)pArg;

		CreatePhysXActor(m_PhysXColliderDesc);
	}

	GET_SINGLE(CPhysX_Manager)->Register_PhysXCollider(Weak_StaticCast<CPhysXCollider>(m_this));

	return S_OK;
}

void CPhysXCollider::Start()
{

}

void CPhysXCollider::Init_MeshCollider(weak_ptr<MESH_DATA> pMeshData, const vector<INSTANCE_MESH_DESC>* In_ParticleDescs)
{
	PxU32 iNumVertices;
	PxU32 iNumFaces;
	PxVec3* pVertices;
	FACEINDICES32* pIndices;
	PxTriangleMeshDesc meshDesc;

	if (In_ParticleDescs)
	{
		for (_size_t i = 0; i < In_ParticleDescs->size(); ++i)
		{
			iNumVertices = pMeshData.lock()->iNumVertices;
			iNumFaces = pMeshData.lock()->iNumFaces;
			pVertices = DBG_NEW PxVec3[iNumVertices];
			pIndices = DBG_NEW FACEINDICES32[iNumFaces];

			SMath::Convert_PxVec3FromMeshDataWithTransformMatrix(pVertices, pMeshData, (*In_ParticleDescs)[i].Get_Matrix());

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

			m_TriangleMesh.push_back(nullptr);

			GET_SINGLE(CPhysX_Manager)->Create_MeshFromTriangles(meshDesc, &m_TriangleMesh.back());

			Safe_Delete_Array(pVertices);
			Safe_Delete_Array(pIndices);
		}

	}
	else
	{
		iNumVertices = pMeshData.lock()->iNumVertices;
		iNumFaces = pMeshData.lock()->iNumFaces;
		pVertices = DBG_NEW PxVec3[iNumVertices];
		pIndices = DBG_NEW FACEINDICES32[iNumFaces];

		SMath::Convert_PxVec3FromMeshData(pVertices, pMeshData);

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

		m_TriangleMesh.push_back(nullptr);

		GET_SINGLE(CPhysX_Manager)->Create_MeshFromTriangles(meshDesc, &m_TriangleMesh.back());

		Safe_Delete_Array(pVertices);
		Safe_Delete_Array(pIndices);
	}
}

void CPhysXCollider::Init_ConvexMeshCollider(weak_ptr<MESH_DATA> pMeshData, const vector<INSTANCE_MESH_DESC>* In_ParticleDescs)
{
	PxU32 iNumVertices;
	PxVec3* pVertices;
	PxConvexMeshDesc meshDesc;

	if (In_ParticleDescs)
	{
		/*_matrix TransformationMatrix;
		_matrix RotationMatrix, ScaleMatrix;

		_vector vPitchYawRoll;
		_vector vPosition;*/

		for (_size_t i = 0; i < In_ParticleDescs->size(); ++i)
		{
			iNumVertices = pMeshData.lock()->iNumVertices;
			pVertices = DBG_NEW PxVec3[iNumVertices];

			/*vPitchYawRoll = XMLoadFloat3(&(*In_ParticleDescs)[i].vRotation);
			vPosition = XMLoadFloat3(&(*In_ParticleDescs)[i].vTarnslation);
			vPosition.m128_f32[3] = 1.f;

			RotationMatrix = XMMatrixRotationRollPitchYawFromVector(vPitchYawRoll);
			ScaleMatrix = XMMatrixScaling((*In_ParticleDescs)[i].vScale.x, (*In_ParticleDescs)[i].vScale.y, (*In_ParticleDescs)[i].vScale.z);
			TransformationMatrix = ScaleMatrix * RotationMatrix;
			TransformationMatrix.r[3] = vPosition;*/

			SMath::Convert_PxVec3FromMeshDataWithTransformMatrix(pVertices, pMeshData, (*In_ParticleDescs)[i].Get_Matrix());

			meshDesc.points.count = iNumVertices;
			meshDesc.points.stride = sizeof(PxVec3);
			meshDesc.points.data = pVertices;

			meshDesc.flags |= PxConvexFlag::Enum::ePLANE_SHIFTING;
			meshDesc.flags |= PxConvexFlag::Enum::eGPU_COMPATIBLE;
			meshDesc.flags |= PxConvexFlag::Enum::eCOMPUTE_CONVEX;

			m_ConvexMeshes.push_back(nullptr);

			GET_SINGLE(CPhysX_Manager)->Create_ConvexMesh(meshDesc, &m_ConvexMeshes.back());

			Safe_Delete_Array(pVertices);
		}
	}

	else
	{
		iNumVertices = pMeshData.lock()->iNumVertices;
		pVertices = DBG_NEW PxVec3[iNumVertices];

		SMath::Convert_PxVec3FromMeshData(pVertices, pMeshData);

		meshDesc.points.count = iNumVertices;
		meshDesc.points.stride = sizeof(PxVec3);
		meshDesc.points.data = pVertices;

		meshDesc.flags |= PxConvexFlag::Enum::ePLANE_SHIFTING;
		meshDesc.flags |= PxConvexFlag::Enum::eGPU_COMPATIBLE;
		meshDesc.flags |= PxConvexFlag::Enum::eCOMPUTE_CONVEX;

		m_ConvexMeshes.push_back(nullptr);

		GET_SINGLE(CPhysX_Manager)->Create_ConvexMesh(meshDesc, &m_ConvexMeshes.back());

		Safe_Delete_Array(pVertices);
	}
	
}

void CPhysXCollider::Init_ModelCollider(weak_ptr<MODEL_DATA> pModelData, const _bool In_isConvex)
{
	if (!pModelData.lock())
	{
		DEBUG_ASSERT;
	}

	for (_uint i = 0; i < pModelData.lock()->iNumMeshs; ++i)
	{
		if (In_isConvex)
		{
			Init_ConvexMeshCollider((pModelData.lock()->Mesh_Datas[i]));
		}
		else
		{
			Init_MeshCollider(pModelData.lock()->Mesh_Datas[i]);
		}
	}
}

void CPhysXCollider::Init_ModelInstanceCollider(weak_ptr<MODEL_DATA> pModelData, const vector<INSTANCE_MESH_DESC>& In_ParticleDescs, const _bool In_isConvex)
{
	if (!pModelData.lock())
	{
		DEBUG_ASSERT;
	}

	for (_uint i = 0; i < pModelData.lock()->iNumMeshs; ++i)
	{
		if (In_isConvex)
		{
			Init_ConvexMeshCollider((pModelData.lock()->Mesh_Datas[i]), &In_ParticleDescs);
		}
		else
		{
			Init_MeshCollider(pModelData.lock()->Mesh_Datas[i], &In_ParticleDescs);
		}
	}
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

_matrix CPhysXCollider::Synchronize_Matrix(_fmatrix In_WorldMatrix)
{
	_matrix ResultMatrix = SMath::Get_ScaleMatrix(In_WorldMatrix);

	ResultMatrix *= Get_WorldMatrix();

	return ResultMatrix;
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

	//PxVec3	vMaxVel = { XMVectorGetX(m_vMaxVelocity), XMVectorGetY(m_vMaxVelocity), XMVectorGetZ(m_vMaxVelocity) };
	//PxVec3	vVelocity = m_pRigidDynamic->getLinearVelocity();

	//_vector	XDir = { 1.f, 0.f, 0.f, 0.f };
	//_vector	ZDir = { 0.f, 1.f, 0.f, 0.f }; //vector2Dot으로 계산 하기 때문

	//_vector	VelocityVec2 = { vVelocity.x, vVelocity.z, 0.f, 0.f };

	//_vector vNormalizedVelocity = XMVector2Normalize(VelocityVec2);

	////m_vMaxVelocity
	//_vector	vSpeed = XMVector2Length(VelocityVec2);
	//_float	fSpeed = min(XMVectorGetX(vSpeed), XMVectorGetX(m_vMaxVelocity));

	//vNormalizedVelocity *= fSpeed;

	//_vector	XSpeed = XDir * XMVectorGetX(XMVector2Dot(XDir, vNormalizedVelocity));
	//_vector	ZSpeed = ZDir * XMVectorGetX(XMVector2Dot(ZDir, vNormalizedVelocity));

	//vVelocity.x = XMVectorGetX(XSpeed);
	//vVelocity.z = XMVectorGetY(ZSpeed);
	////vVelocity.x = min(fabsf(vVelocity.x), vMaxVel.x) * vVelocity.x / vVelocity.x;
	////vVelocity.y = min(fabsf(vVelocity.y), vMaxVel.y) * vVelocity.y / vVelocity.y;
	////vVelocity.z = min(fabsf(vVelocity.z), vMaxVel.z) * vVelocity.z / vVelocity.z;

	//m_pRigidDynamic->setLinearVelocity(vVelocity);

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

_vector CPhysXCollider::Get_Quaternion()
{
	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	_vector vQuaternion = { Transform.q.x, Transform.q.y, Transform.q.z, Transform.q.w };

	return vQuaternion;
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

_matrix CPhysXCollider::Get_WorldMatrix()
{
	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	_vector vPos = { Transform.p.x, Transform.p.y, Transform.p.z };
	vPos.m128_f32[3] = 1.f;
	_vector vQuaternion = { Transform.q.x, Transform.q.y, Transform.q.z, Transform.q.w };
	
	_matrix ResultMatrix = XMMatrixRotationQuaternion(vQuaternion);
	ResultMatrix.r[3] = vPos;

	return ResultMatrix;
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

void CPhysXCollider::Add_PhysXActorAtSceneWithOption(const PxVec3& In_MassSpaceInertiaTensor, const PxReal In_fMass)
{
	if (m_pRigidDynamic && m_pRigidStatic)
	{
		// 둘 다 존재하면 안된다.
		DEBUG_ASSERT;
	}

	else if (m_pRigidDynamic)
	{
		// 저항값
		m_pRigidDynamic->setAngularDamping(0.5f);
		m_pRigidDynamic->setMass(In_fMass);
		m_pRigidDynamic->setLinearDamping(0.f);
		// 속도
		//DynamicActor.setLinearVelocity(velocity);
		m_pRigidDynamic->setMassSpaceInertiaTensor(In_MassSpaceInertiaTensor);
		//m_pRigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);

		GET_SINGLE(CPhysX_Manager)->Add_DynamicActorAtCurrentScene(*m_pRigidDynamic);
	}

	else if (m_pRigidStatic)
	{
		GET_SINGLE(CPhysX_Manager)->Add_StaticActorAtCurrentScene(*m_pRigidStatic);
	}

	else
	{
		// 생성된 PhysXActor가 없음. Create부터 할 것.
#ifdef _DEBUG
		cout << "No Actor was created." << endl;
#endif // _DEBUG
	}

	for (auto& elem : m_pGeometry)
	{
		Safe_Delete(elem);
	}

}

void CPhysXCollider::Add_PhysXActorAtScene()
{
	if (m_pRigidDynamic && m_pRigidStatic)
	{
		// 둘 다 존재하면 안된다.
		DEBUG_ASSERT;
	}

	else if (m_pRigidDynamic)
	{
		GET_SINGLE(CPhysX_Manager)->Add_DynamicActorAtCurrentScene(*m_pRigidDynamic);
	}

	else if (m_pRigidStatic)
	{
		GET_SINGLE(CPhysX_Manager)->Add_StaticActorAtCurrentScene(*m_pRigidStatic);
	}

	else
	{
		// 생성된 PhysXActor가 없음. Create부터 할 것.
#ifdef _DEBUG
		cout << "No Actor was created." << endl;
#endif // _DEBUG

	}

	for (auto& elem : m_pGeometry)
	{
		Safe_Delete(elem);
	}

}

void CPhysXCollider::Create_Geometry()
{


	switch (m_PhysXColliderDesc.eShape)
	{
	case PHYSXCOLLIDER_TYPE::SPHERE:
		m_pGeometry.push_back(DBG_NEW PxSphereGeometry(XMVectorGetX(m_PhysXColliderDesc.vScale)));
		break;

	case PHYSXCOLLIDER_TYPE::BOX:
		m_pGeometry.push_back(DBG_NEW PxBoxGeometry(XMVectorGetX(m_PhysXColliderDesc.vScale) * 0.5f,
			XMVectorGetY(m_PhysXColliderDesc.vScale) * 0.5f, 
			XMVectorGetZ(m_PhysXColliderDesc.vScale) * 0.5f));

		
		break;

	case PHYSXCOLLIDER_TYPE::CYLINDER:
	case PHYSXCOLLIDER_TYPE::CONVEXMESH:
	{
		PxConvexMeshGeometry* pPxGeometry = DBG_NEW PxConvexMeshGeometry();
		pPxGeometry->convexMesh = m_PhysXColliderDesc.pConvecMesh;
		PxMeshScale	vScale;
		vScale.scale.x = XMVectorGetX(m_PhysXColliderDesc.vScale);
		vScale.scale.y = XMVectorGetY(m_PhysXColliderDesc.vScale);
		vScale.scale.z = XMVectorGetZ(m_PhysXColliderDesc.vScale);
		pPxGeometry->scale = vScale;
		m_pGeometry.push_back(pPxGeometry);
		break;
	}

	case PHYSXCOLLIDER_TYPE::CONVEXMODEL:
	{
		for (_size_t i = 0; i < m_ConvexMeshes.size(); ++i)
		{
			PxConvexMeshGeometry* pPxGeometry = DBG_NEW PxConvexMeshGeometry();
			pPxGeometry->convexMesh = m_ConvexMeshes[i];
			PxMeshScale	vScale;
			vScale.scale.x = XMVectorGetX(m_PhysXColliderDesc.vScale);
			vScale.scale.y = XMVectorGetY(m_PhysXColliderDesc.vScale);
			vScale.scale.z = XMVectorGetZ(m_PhysXColliderDesc.vScale);
			pPxGeometry->scale = vScale;
			m_pGeometry.push_back(pPxGeometry);
		}
		break;
	}

	case PHYSXCOLLIDER_TYPE::MESH:
	{
		PxTriangleMeshGeometry* PxGeometry = DBG_NEW PxTriangleMeshGeometry();
		PxGeometry->triangleMesh = m_TriangleMesh.front();
		PxMeshScale	vScale;
		vScale.scale.x = XMVectorGetX(m_PhysXColliderDesc.vScale);
		vScale.scale.y = XMVectorGetY(m_PhysXColliderDesc.vScale);
		vScale.scale.z = XMVectorGetZ(m_PhysXColliderDesc.vScale);
		PxGeometry->scale = vScale;
		m_pGeometry.push_back(PxGeometry);
		break;
	}

	case PHYSXCOLLIDER_TYPE::MODEL:
	{
		for (_size_t i = 0; i < m_TriangleMesh.size(); ++i)
		{
			PxTriangleMeshGeometry* PxGeometry = DBG_NEW PxTriangleMeshGeometry();
			PxGeometry->triangleMesh = m_TriangleMesh[i];
			PxMeshScale	vScale;
			vScale.scale.x = XMVectorGetX(m_PhysXColliderDesc.vScale);
			vScale.scale.y = XMVectorGetY(m_PhysXColliderDesc.vScale);
			vScale.scale.z = XMVectorGetZ(m_PhysXColliderDesc.vScale);
			PxGeometry->scale = vScale;
			m_pGeometry.push_back(PxGeometry);
		}
		break;
	}

	case PHYSXCOLLIDER_TYPE::TYPE_END:
		MSG_BOX("Failed to create DynamicActor : eShape is wrong");
		break;
	default:
		break;
	}
}

void CPhysXCollider::Create_DynamicActor(PHYSXCOLLIDERDESC& tPhysXColliderDesc, PxTransform Transform)
{
	m_pRigidDynamic = GET_SINGLE(CPhysX_Manager)->Create_DynamicActor(Transform);
	Create_Geometry();

	// 지오메트리 개수만큼 셰이프를 만든다.
	for (_size_t i = 0; i < m_pGeometry.size(); ++i)
	{
		PxShapeFlags shapeFlags;

		if (tPhysXColliderDesc.bTrigger)
		{
			shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE;
			m_FilterData.word2 = 1;

		}
		else
		{
			shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE;
			m_FilterData.word2 = 1;
		}

		PxShape* pShape = nullptr;

		GET_SINGLE(CPhysX_Manager)->Create_Shape(*m_pGeometry[i], m_PhysXColliderDesc.pMaterial, false, shapeFlags, &pShape);

		m_FilterData.word0 = (1 << m_PhysXColliderDesc.iFilterType);
		m_FilterData.word1 = GET_SINGLE(CPhysX_Manager)->Get_PhysXFilterGroup(m_PhysXColliderDesc.iFilterType);
		m_FilterData.word3 = m_iColliderIndex;

		//FilterData.word1 = m_PhysXColliderDesc.iFilterType;

		if (!pShape)
		{
			// Shape가 생성되지 않음.
			DEBUG_ASSERT;
		}

		pShape->setSimulationFilterData(m_FilterData);

		m_pRigidDynamic->attachShape(*pShape);
		m_pRigidDynamic->userData = &m_iColliderIndex;

		m_pShape.push_back(pShape);
	}

	
}

void CPhysXCollider::Create_StaticActor(PHYSXCOLLIDERDESC& tPhysXColliderDesc, PxTransform Transform)
{
	m_pRigidStatic = GET_SINGLE(CPhysX_Manager)->Create_StaticActor(Transform);
	Create_Geometry();


	// 지오메트리 개수만큼 셰이프를 만든다.
	for (_size_t i = 0; i < m_pGeometry.size(); ++i)
	{

		PxShapeFlags shapeFlags;

		if (tPhysXColliderDesc.bTrigger)
		{
			shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE;
			m_FilterData.word2 = 1;
		}
		else
		{
			shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE;
			m_FilterData.word2 = 1;
		}

		PxShape* pShape = nullptr;

		GET_SINGLE(CPhysX_Manager)->Create_Shape(*m_pGeometry[i], m_PhysXColliderDesc.pMaterial, false, shapeFlags, &pShape);

		m_FilterData.word0 = (1 << m_PhysXColliderDesc.iFilterType);
		m_FilterData.word1 = GET_SINGLE(CPhysX_Manager)->Get_PhysXFilterGroup(m_PhysXColliderDesc.iFilterType);
		m_FilterData.word3 = m_iColliderIndex;

		if (!pShape)
		{
			// Shape가 생성되지 않음.
			DEBUG_ASSERT;
		}

		pShape->setSimulationFilterData(m_FilterData);

		m_pRigidStatic->attachShape(*pShape);
		m_pRigidStatic->userData = &m_iColliderIndex;

		m_pShape.push_back(pShape);
	}
}


void CPhysXCollider::OnCollision(weak_ptr<CPhysXCollider> pOtherCollider)
{
	m_isColl = true;

	list<weak_ptr<CPhysXCollider>>::iterator iter = find_if(m_pPreOtherColliders.begin(),
		m_pPreOtherColliders.end(),
		[&](weak_ptr<CPhysXCollider> pPreOtherCollider)
		{
			if (!pOtherCollider.lock().get() || !pPreOtherCollider.lock().get())
			{
				DEBUG_ASSERT;
				return false;
			}

			return pPreOtherCollider.lock()->Get_PColliderIndex() == pOtherCollider.lock()->Get_PColliderIndex();
		});

	if (m_pPreOtherColliders.end() == iter)
	{
		PhysXCollisionEnter(pOtherCollider);
	}
	else
	{
		PhysXCollisionStay(pOtherCollider);
	}

	m_pOtherColliders.push_back(pOtherCollider);
}

void CPhysXCollider::End_CollisionCheck()
{
	if (m_pOtherColliders.empty())
		m_isColl = false;



	_bool isErase = false;

	for (auto iter = m_pPreOtherColliders.begin(); iter != m_pPreOtherColliders.end();)
	{
		isErase = false;

		for (auto& elem : m_pOtherColliders)
		{
			if ((*iter).lock() == elem.lock())
			{
				iter = m_pPreOtherColliders.erase(iter);
				isErase = true;
				break;
			}
		}

		if (!isErase)
			iter++;
	}

	// 이전에 들어왔지만, 이번 프레임에 안들어온 충돌체.
	// Exit를 호출한다. 
	for (auto& elem : m_pPreOtherColliders)
	{
		PhysXCollisionExit(elem);
	}

	m_pPreOtherColliders = m_pOtherColliders;
	m_pOtherColliders.clear();
}

void CPhysXCollider::PhysXCollisionEnter(weak_ptr<CPhysXCollider> pOtherCollider)
{
	CallBack_CollisionEnter(pOtherCollider);
	m_pOwner.lock()->OnPhysXCollisionEnter(pOtherCollider);

	cout << "Shape is entering trigger volume\n";
}

void CPhysXCollider::PhysXCollisionStay(weak_ptr<CPhysXCollider> pOtherCollider)
{
	CallBack_CollisionStay(pOtherCollider);
	m_pOwner.lock()->OnPhysXCollisionStay(pOtherCollider);
}

void CPhysXCollider::PhysXCollisionExit(weak_ptr<CPhysXCollider> pOtherCollider)
{
	CallBack_CollisionExit(pOtherCollider);
	m_pOwner.lock()->OnPhysXCollisionExit(pOtherCollider);

	cout << "Shape is Exit trigger volume\n";

}

void CPhysXCollider::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

	m_FilterData.word2 = 1;

	for (auto& elem : m_pShape)
	{
		elem->setSimulationFilterData(m_FilterData);
	}

	if (m_pRigidDynamic)
		m_pRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);

	if (m_pRigidStatic)
		m_pRigidStatic->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
}

void CPhysXCollider::OnDisable()
{
	__super::OnDisable();

	m_FilterData.word2 = 0;

	for (auto& elem : m_pShape)
	{
		elem->setSimulationFilterData(m_FilterData);
	}

	if (m_pRigidDynamic)
		m_pRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);

	if (m_pRigidStatic)
		m_pRigidStatic->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
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

