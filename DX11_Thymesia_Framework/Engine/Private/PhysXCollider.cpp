#include "..\Public\PhysXCollider.h"
#include "GameInstance.h"
#include "PhysX_Manager.h"
#include "Transform.h"
#include "SMath.h"

GAMECLASS_C(CPhysXCollider);
IMPLEMENT_CLONABLE(CPhysXCollider, CComponent);

_uint CPhysXCollider::m_iClonedColliderIndex = 0;

HRESULT CPhysXCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPhysXCollider::Initialize(void * pArg)
{
	if (nullptr == pArg)
		__debugbreak();

	ZeroMemory(m_ConvexMeshes, sizeof(PxConvexMesh*) * 16);

	memcpy(&m_ColliderDesc, pArg, sizeof(PHYSXCOLLIDERDESC));

	CreatePhysActor(m_ColliderDesc);

	m_iColliderIndex = m_iClonedColliderIndex++;

	return S_OK;
}

void CPhysXCollider::Start()
{
}

void CPhysXCollider::Synchronize_Transform(weak_ptr<CTransform> pTransform)
{
	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	_vector vPos = { Transform.p.x, Transform.p.y, Transform.p.z };
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

void CPhysXCollider::Synchronize_Collider(weak_ptr<CTransform> pTransform)
{
	_vector vPos = pTransform.lock()->Get_State(CTransform::STATE_TRANSLATION);
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
	//PxShape* pShape[1];
	//ZeroMemory(pShape, sizeof(PxShape) * 16);
	if (m_pRigidDynamic)
	{
		//m_pRigidDynamic->getShapes(pShape, 1);
		m_pRigidDynamic->release();
		m_pRigidDynamic = nullptr;

		m_ColliderDesc.vScale = vScale;
		CreatePhysActor(m_ColliderDesc);

		//m_pRigidDynamic->attachShape(*pShape[0]);

		for (size_t i = 0; i < 16; ++i)
		{
			if (nullptr != m_ConvexMeshes[i])
			{
				PxConvexMeshGeometry PxGeometry;
				PxGeometry.convexMesh = m_ConvexMeshes[i];
				PxMeshScale	vMeshScale;
				vMeshScale.scale.x = XMVectorGetX(vScale);
				vMeshScale.scale.y = XMVectorGetY(vScale);
				vMeshScale.scale.z = XMVectorGetZ(vScale);
				PxGeometry.scale = vMeshScale;
				PxShape*	pShape;
				PxMaterial*	pMaterial = m_ColliderDesc.pMaterial;

				CPhysX_Manager::Get_Instance()->Create_Shape(PxGeometry, pMaterial, &pShape);

				m_pRigidDynamic->attachShape(*pShape);
			}
		}
	}

	if (m_pRigidStatic)
	{
		//m_pRigidStatic->getShapes(pShape, 16);
		m_pRigidStatic->release();
		m_pRigidStatic = nullptr;

		m_ColliderDesc.vScale = vScale;
		CreatePhysActor(m_ColliderDesc);


		for (size_t i = 0; i < 16; ++i)
		{
			if (nullptr != m_ConvexMeshes[i])
			{
				PxConvexMeshGeometry PxGeometry;
				PxGeometry.convexMesh = m_ConvexMeshes[i];
				PxMeshScale	vMeshScale;
				vMeshScale.scale.x = XMVectorGetX(vScale);
				vMeshScale.scale.y = XMVectorGetY(vScale);
				vMeshScale.scale.z = XMVectorGetZ(vScale);
				PxGeometry.scale = vMeshScale;
				PxShape*	pShape;
				PxMaterial*	pMaterial = m_ColliderDesc.pMaterial;

				CPhysX_Manager::Get_Instance()->Create_Shape(PxGeometry, pMaterial, &pShape);

				m_pRigidStatic->attachShape(*pShape);
			}
		}
	}
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
		CreatePhysActor(m_ColliderDesc);
	}
}

void CPhysXCollider::CreatePhysActor(PHYSXCOLLIDERDESC PhysXColliderDesc)
{
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

	switch (PhysXColliderDesc.eType)
	{
	case PHYSXACTOR_TYPE::DYNAMIC:
	case PHYSXACTOR_TYPE::YFIXED_DYNAMIC:
		Create_DynamicActor(PhysXColliderDesc, Transform, PhysXColliderDesc.pConvecMesh);
		break;
	case PHYSXACTOR_TYPE::STATIC:
		Create_StaticActor(PhysXColliderDesc, Transform, PhysXColliderDesc.pConvecMesh);
		break;
	case PHYSXACTOR_TYPE::ZONE:

		break;
	default:
		//MSG_BOX("Failed to CreateActor : eCollidertype is wrong");
		break;
	}

	
}

void CPhysXCollider::Create_DynamicActor(PHYSXCOLLIDERDESC PhysXColliderDesc, PxTransform Transform, PxConvexMesh* pConvexMesh)
{
	switch (PhysXColliderDesc.eShape)
	{
	case PHYSXCOLLIDER_TYPE::SPHERE:
		m_pRigidDynamic = CPhysX_Manager::Get_Instance()->Create_DynamicActor(Transform,
			PxSphereGeometry(XMVectorGetX(PhysXColliderDesc.vScale)), CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.fDensity, PxVec3(0), PhysXColliderDesc.pMaterial);
		break;
	case PHYSXCOLLIDER_TYPE::BOX:
		m_pRigidDynamic = CPhysX_Manager::Get_Instance()->Create_DynamicActor(Transform,
			PxBoxGeometry(XMVectorGetX(PhysXColliderDesc.vScale) * 0.5f, XMVectorGetY(PhysXColliderDesc.vScale) * 0.5f, XMVectorGetZ(PhysXColliderDesc.vScale) * 0.5f),
			CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.fDensity, PxVec3(0), PhysXColliderDesc.pMaterial);
		break;
	case PHYSXCOLLIDER_TYPE::CYLINDER:
	case PHYSXCOLLIDER_TYPE::CONVECMESH:
	{
		PxConvexMeshGeometry PxGeometry; (PhysXColliderDesc.pConvecMesh);
		PxGeometry.convexMesh = PhysXColliderDesc.pConvecMesh;
		PxMeshScale	vScale;
		vScale.scale.x = XMVectorGetX(PhysXColliderDesc.vScale);
		vScale.scale.y = XMVectorGetY(PhysXColliderDesc.vScale);
		vScale.scale.z = XMVectorGetZ(PhysXColliderDesc.vScale);
		PxGeometry.scale = vScale;
		m_pRigidDynamic = CPhysX_Manager::Get_Instance()->Create_DynamicActor(Transform,
			PxGeometry, CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.fDensity, PxVec3(0), PhysXColliderDesc.pMaterial);

		m_pRigidDynamic->userData = &m_iColliderIndex;

		//m_pRigidDynamic = CPhysX_Manager::Get_Instance()->Create_DynamicActor(Transform,
		//	PxConvexMeshGeometry(PhysXColliderDesc.pConvecMesh), CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.fDensity, PxVec3(0), PhysXColliderDesc.pMaterial);
		break;
	}
	case PHYSXCOLLIDER_TYPE::TYPE_END:
		MSG_BOX("Failed to create DynamicActor : eShape is wrong");
		break;
	default:
		break;
	}

	m_pRigidDynamic->userData = &m_iColliderIndex;

}

void CPhysXCollider::Create_StaticActor(PHYSXCOLLIDERDESC PhysXColliderDesc, PxTransform Transform, PxConvexMesh* pConvexMesh)
{
	switch (PhysXColliderDesc.eShape)
	{
	case PHYSXCOLLIDER_TYPE::SPHERE:
		m_pRigidStatic = CPhysX_Manager::Get_Instance()->Create_StaticActor(Transform,
			PxSphereGeometry(XMVectorGetX(PhysXColliderDesc.vScale)), CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.pMaterial);
		break;
	case PHYSXCOLLIDER_TYPE::BOX:
		m_pRigidStatic = CPhysX_Manager::Get_Instance()->Create_StaticActor(Transform,
			PxBoxGeometry(XMVectorGetX(PhysXColliderDesc.vScale) * 0.5f, XMVectorGetY(PhysXColliderDesc.vScale) * 0.5f, XMVectorGetZ(PhysXColliderDesc.vScale) * 0.5f),
			CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.pMaterial);
		break;
	case PHYSXCOLLIDER_TYPE::CYLINDER:
	case PHYSXCOLLIDER_TYPE::CONVECMESH:
		m_pRigidStatic = CPhysX_Manager::Get_Instance()->Create_StaticActor(Transform,
			PxConvexMeshGeometry(PhysXColliderDesc.pConvecMesh), CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.pMaterial);
		break;
	case PHYSXCOLLIDER_TYPE::TYPE_END:
		MSG_BOX("Failed to create StaticActor : eShape is wrong");
		break;
	default:
		break;
	}

	m_pRigidStatic->userData = &m_iColliderIndex;
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
