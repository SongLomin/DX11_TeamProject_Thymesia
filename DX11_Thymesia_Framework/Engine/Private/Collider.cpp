#include "Collider.h"
#include "GameInstance.h"
#include "DebugDraw.h"
#include "GameObject.h"
#include "SMath.h"

_uint CCollider::m_iClonedColliderIndex = 0;

GAMECLASS_C(CCollider);
IMPLEMENT_CLONABLE(CCollider, CComponent);

COLLIDERDESC CCollider::Get_ColliderDesc() const
{
	return m_ColliderDesc;
}

_vector CCollider::Get_CurrentPosition() const
{
	_vector vResult = XMVectorSetW(XMLoadFloat3(&m_vCurrentPosition), 1.f);

	return vResult;
}

void CCollider::Set_ColliderScale(_fvector In_vScale)
{
	switch (m_eType)
	{
	case COLLISION_TYPE::AABB:
		
		break;
	case COLLISION_TYPE::OBB:

		break;
	case COLLISION_TYPE::SPHERE:
		XMStoreFloat3(&m_ColliderDesc.vScale, In_vScale);
		m_pSphere_Original.reset();
		m_pSphere_Original = make_shared<BoundingSphere>(m_ColliderDesc.vTranslation, m_ColliderDesc.vScale.x * 0.5f);
		m_pSphere = make_shared<BoundingSphere>(*m_pSphere_Original);
		m_vCurrentPosition = m_pSphere_Original->Center;
		break;
	}

}

HRESULT CCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	m_pBatch = make_shared<PrimitiveBatch<VertexPositionColor>>(DEVICECONTEXT);
	m_pEffect = make_shared<BasicEffect>(DEVICE);
	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		ShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	if (FAILED(DEVICE->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;
	

	m_iColliderIndex = m_iClonedColliderIndex++;
	m_thisFromColliderCom = Weak_StaticCast<CCollider>(m_this);

	return S_OK;
}

void CCollider::Start()
{
}

_bool CCollider::Intersect_Ray(const RAY& In_Ray, float* Out_pDistance) const
{
	_bool bIntersection = false;

	_vector vOrigin = XMLoadFloat4(&In_Ray.vOrigin);
	vOrigin.m128_f32[3] = 1.f;
	_vector vDirection = XMLoadFloat3(&In_Ray.vDirection);

	_float fDistance;

	if (COLLISION_TYPE::AABB == m_eType)
	{
		if (m_pAABB->Intersects(vOrigin, vDirection, fDistance))
		{
			if (Out_pDistance)
			{
				*Out_pDistance = fDistance;
			}

			bIntersection = In_Ray.fLength >= fDistance;
		}

		else
		{
			if (Out_pDistance)
			{
				*Out_pDistance = -1.f;
			}
		}
	}

	else if (COLLISION_TYPE::OBB == m_eType)
	{
		if (m_pOBB->Intersects(vOrigin, vDirection, fDistance))
		{
			if (Out_pDistance)
			{
				*Out_pDistance = fDistance;
			}

			bIntersection = In_Ray.fLength >= fDistance;
		}

		else
		{
			if (Out_pDistance)
			{
				*Out_pDistance = -1.f;
			}
		}
	}

	else if (COLLISION_TYPE::SPHERE == m_eType)
	{
		if (SMath::Is_SphereToRayCollision(m_pSphere->Center, m_pSphere->Radius, vOrigin, vDirection, fDistance))
		{
			if (Out_pDistance)
			{
				*Out_pDistance = fDistance;
			}

			bIntersection = In_Ray.fLength >= fDistance;
		}

		else
		{
			if (Out_pDistance)
			{
				*Out_pDistance = -1.f;
			}
		}
	}

	else
	{
		//해당 콜라이더의 콜리전 타입이 지정되지 않음.
		DEBUG_ASSERT;
		return false;
	}

	return bIntersection;
}

void CCollider::Init_Collider(const COLLISION_TYPE& In_ColliderType, const COLLIDERDESC& In_ColliderDesc)
{
	m_eType = In_ColliderType;
	m_ColliderDesc = In_ColliderDesc;
	GET_SINGLE(CCollision_Manager)->Add_Collision(m_ColliderDesc.iLayer, Weak_Cast<CCollider>(m_this));


	switch (m_eType)
	{
	case COLLISION_TYPE::AABB:
		m_pAABB_Original = make_shared<BoundingBox>(m_ColliderDesc.vTranslation, _float3(m_ColliderDesc.vScale.x * 0.5f, m_ColliderDesc.vScale.y * 0.5f, m_ColliderDesc.vScale.z * 0.5f));
		m_pAABB = make_shared<BoundingBox>(*m_pAABB_Original);
		m_vCurrentPosition = m_pAABB_Original->Center;
		break;
	case COLLISION_TYPE::OBB:
		m_pOBB_Original = make_shared<BoundingOrientedBox>(m_ColliderDesc.vTranslation, _float3(m_ColliderDesc.vScale.x * 0.5f, m_ColliderDesc.vScale.y * 0.5f, m_ColliderDesc.vScale.z * 0.5f), m_ColliderDesc.vRotation);
		m_pOBB = make_shared<BoundingOrientedBox>(*m_pOBB_Original);
		m_vCurrentPosition = m_pOBB_Original->Center;
		break;
	case COLLISION_TYPE::SPHERE:
		m_pSphere_Original = make_shared<BoundingSphere>(m_ColliderDesc.vTranslation, m_ColliderDesc.vScale.x * 0.5f);
		m_pSphere = make_shared<BoundingSphere>(*m_pSphere_Original);
		m_vCurrentPosition = m_pSphere_Original->Center;
		break;

	default:
		//받아온 타입이 유효하지 않음.
		DEBUG_ASSERT;
		
	}


}

void CCollider::Reset_OtherColliders()
{

	m_pPreOtherColliders.clear();
	m_pOtherColliders.clear();
	m_isColl = false;
}

void CCollider::Update(_fmatrix TransformMatrix)
{
	_matrix WorldMatrix = SMath::Add_PositionWithRotation(TransformMatrix, XMLoadFloat3(&m_ColliderDesc.vOffset));

	switch (m_eType)
	{
	case COLLISION_TYPE::AABB:
		m_pAABB_Original->Transform(*m_pAABB, Remove_Rotation(WorldMatrix));
		m_vCurrentPosition = m_pAABB->Center;
		break;
	case COLLISION_TYPE::OBB:
		m_pOBB_Original->Transform(*m_pOBB, WorldMatrix);
		m_vCurrentPosition = m_pOBB->Center;
		break;
	case COLLISION_TYPE::SPHERE:
		m_pSphere_Original->Transform(*m_pSphere, WorldMatrix);
		m_vCurrentPosition = m_pSphere->Center;
		break;
	}
}

void CCollider::Update_AddPosition(const _float3& In_vPosition)
{
	SMath::Add_Float3(&m_vCurrentPosition, In_vPosition);


	switch (m_eType)
	{
	case COLLISION_TYPE::AABB:
		m_pAABB_Original->Transform(*m_pAABB, 1.f, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMLoadFloat3(&m_vCurrentPosition));
		break;
	case COLLISION_TYPE::OBB:
		m_pOBB_Original->Transform(*m_pOBB, 1.f, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMLoadFloat3(&m_vCurrentPosition));
		break;
	case COLLISION_TYPE::SPHERE:
		m_pSphere_Original->Transform(*m_pSphere, 1.f, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMLoadFloat3(&m_vCurrentPosition));
		break;
	}
}

void CCollider::OnCollision(weak_ptr<CCollider> pOtherCollider)
{
	m_isColl = true;

	list<weak_ptr<CCollider>>::iterator iter = find_if(m_pPreOtherColliders.begin(), 
		m_pPreOtherColliders.end(), 
		[&](weak_ptr<CCollider> pPreOtherCollider)
		{
			if (!pOtherCollider.lock().get() || !pPreOtherCollider.lock().get())
			{
				return false;
			}

			return pPreOtherCollider.lock()->Get_ColliderIndex() == pOtherCollider.lock()->Get_ColliderIndex();
		});

	if (m_pPreOtherColliders.end() == iter)
	{
		CollisionEnter(pOtherCollider);
	}
	else
	{
		CollisionStay(pOtherCollider);
	}

	m_pOtherColliders.push_back(pOtherCollider);
}

void CCollider::End_CollisionCheck()
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
		CollisionExit(elem);
	}

	m_pPreOtherColliders = m_pOtherColliders;
	m_pOtherColliders.clear();
}

//_bool CCollider::Collision(weak_ptr<CCollider> pTargetCollider)
//{
//	if (COLLISION_TYPE::AABB == m_eType)
//	{
//		if (COLLISION_TYPE::AABB == pTargetCollider.lock()->m_eType)
//			m_isColl = m_pAABB->Intersects(*pTargetCollider.lock()->m_pAABB);
//		if (COLLISION_TYPE::OBB == pTargetCollider.lock()->m_eType)
//			m_isColl = m_pAABB->Intersects(*pTargetCollider.lock()->m_pOBB);
//		if (COLLISION_TYPE::SPHERE == pTargetCollider.lock()->m_eType)
//			m_isColl = m_pAABB->Intersects(*pTargetCollider.lock()->m_pSphere);
//	}
//
//	if (COLLISION_TYPE::OBB == m_eType)
//	{
//		if (COLLISION_TYPE::AABB == pTargetCollider.lock()->m_eType)
//			m_isColl = m_pOBB->Intersects(*pTargetCollider.lock()->m_pAABB);
//		if (COLLISION_TYPE::OBB == pTargetCollider.lock()->m_eType)
//			m_isColl = m_pOBB->Intersects(*pTargetCollider.lock()->m_pOBB);
//		if (COLLISION_TYPE::SPHERE == pTargetCollider.lock()->m_eType)
//			m_isColl = m_pOBB->Intersects(*pTargetCollider.lock()->m_pSphere);
//	}
//
//	if (COLLISION_TYPE::SPHERE == m_eType)
//	{
//		if (COLLISION_TYPE::AABB == pTargetCollider.lock()->m_eType)
//			m_isColl = m_pSphere->Intersects(*pTargetCollider.lock()->m_pAABB);
//		if (COLLISION_TYPE::OBB == pTargetCollider.lock()->m_eType)
//			m_isColl = m_pSphere->Intersects(*pTargetCollider.lock()->m_pOBB);
//		if (COLLISION_TYPE::SPHERE == pTargetCollider.lock()->m_eType)
//			m_isColl = m_pSphere->Intersects(*pTargetCollider.lock()->m_pSphere);
//	}
//
//	return m_isColl;
//}

_matrix CCollider::Remove_Rotation(_fmatrix TransformMatrix)
{
	_matrix			Transform = TransformMatrix;

	Transform.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[0]));
	Transform.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[1]));
	Transform.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[2]));

	return Transform;
}

void CCollider::Set_DebugColor(_fvector In_vColor)
{
	XMStoreFloat4(&m_vColor, In_vColor);

	m_bEditerDraw = true;
}

HRESULT CCollider::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (!GAMEINSTANCE->Is_Debug())
		return E_FAIL;

	if (!Get_Enable())
		return E_FAIL;

	DEVICECONTEXT->GSSetShader(nullptr, nullptr, 0);
	DEVICECONTEXT->IASetInputLayout(m_pInputLayout.Get());

	m_pEffect->SetWorld(XMMatrixIdentity());

	m_pEffect->SetView(GET_SINGLE(CPipeLine)->Get_Transform(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(GET_SINGLE(CPipeLine)->Get_Transform(CPipeLine::D3DTS_PROJ));

	m_pEffect->Apply(DEVICECONTEXT);

	_vector		vColor;

	if (m_bEditerDraw)
		vColor = XMLoadFloat4(&m_vColor);
	else
		vColor = (m_isColl == true) ? (XMVectorSet(1.f, 0.f, 0.f, 1.f)) : (XMVectorSet(0.f, 1.f, 0.f, 1.f));

	m_pBatch->Begin();

	switch (m_eType)
	{
	case COLLISION_TYPE::AABB:
		DX::Draw(m_pBatch, *m_pAABB.get(), vColor);
		break;
	case COLLISION_TYPE::OBB:
		DX::Draw(m_pBatch, *m_pOBB.get(), vColor);
		break;
	case COLLISION_TYPE::SPHERE:
		DX::Draw(m_pBatch, *m_pSphere.get(), vColor);
		break;
	}

	m_pBatch->End();

	return S_OK;
}

HRESULT CCollider::Render_IgnoreDebugCheck()
{
	if (!Get_Enable())
		return E_FAIL;

	DEVICECONTEXT->GSSetShader(nullptr, nullptr, 0);
	DEVICECONTEXT->IASetInputLayout(m_pInputLayout.Get());

	m_pEffect->SetWorld(XMMatrixIdentity());

	m_pEffect->SetView(GET_SINGLE(CPipeLine)->Get_Transform(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(GET_SINGLE(CPipeLine)->Get_Transform(CPipeLine::D3DTS_PROJ));

	m_pEffect->Apply(DEVICECONTEXT);

	m_pBatch->Begin();

	switch (m_eType)
	{
	case COLLISION_TYPE::AABB:
		DX::Draw(m_pBatch, *m_pAABB.get(), XMLoadFloat4(&m_vColor));
		break;
	case COLLISION_TYPE::OBB:
		DX::Draw(m_pBatch, *m_pOBB.get(), XMLoadFloat4(&m_vColor));
		break;
	case COLLISION_TYPE::SPHERE:
		DX::Draw(m_pBatch, *m_pSphere.get(), XMLoadFloat4(&m_vColor));
		break;
	}

	m_pBatch->End();

	return S_OK;
}


void CCollider::CollisionEnter(weak_ptr<CCollider> pOtherCollider)
{
	CallBack_CollisionEnter(pOtherCollider);
	m_pOwner.lock()->OnCollisionEnter(m_thisFromColliderCom, pOtherCollider);

}
void CCollider::CollisionStay(weak_ptr<CCollider> pOtherCollider)
{
	CallBack_CollisionStay(pOtherCollider);
	m_pOwner.lock()->OnCollisionStay(m_thisFromColliderCom, pOtherCollider);
}
void CCollider::CollisionExit(weak_ptr<CCollider> pOtherCollider)
{
	CallBack_CollisionExit(pOtherCollider);
	m_pOwner.lock()->OnCollisionExit(m_thisFromColliderCom, pOtherCollider);
}

void CCollider::OnEnable(void* _Arg)
{
	Reset_OtherColliders();
	GET_SINGLE(CCollision_Manager)->Add_Collision(m_ColliderDesc.iLayer, Weak_Cast<CCollider>(m_this));
}


void CCollider::OnDisable()
{
	
}



void CCollider::OnDestroy()
{
}

void CCollider::Free()
{
}
