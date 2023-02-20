#include "Collision_Manager.h"
#include "Collider.h"
#include "GameInstance.h"
#include "CollisionCallBack.h"
#include "CollisionSimulationEventCallBack.h"

IMPLEMENT_SINGLETON(CCollision_Manager)

void CCollision_Manager::Initialize(const _uint& In_iNumLayer)
{
	m_pColliderList.reserve(In_iNumLayer);

	for (_uint i(0); i < In_iNumLayer; ++i)
	{
		m_pColliderList.push_back({});
		m_pReservedColliderList.push_back({});
		m_arrCheck.push_back(0);
	}

	m_pCollisionCallBack = DBG_NEW CollisionCallBack();
	m_pCollisionSimulationEventCallBack = DBG_NEW CollisionSimulationEventCallBack();
}

void CCollision_Manager::Tick()
{
	std::unique_lock<std::mutex> lock(m_JobMutex);

	Update_ReservedColliderList();
	Remove_DeadCollision();

	for (size_t iRow(0); iRow < m_arrCheck.size(); ++iRow)
	{
		for (size_t iCol(iRow); iCol < m_arrCheck.size(); ++iCol) // 체크 박스 반만 해야되기 때문에 컬럼은 row부터
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				//auto Handle = async(&CCollision_Manager::CollisionGroupUpdate, this, (COLLISION_TYPE)iRow, (COLLISION_TYPE)iCol);
				Update_CollisionGroup((_uint)iRow, (_uint)iCol);
			}
		}
	}

	End_CollisionCheck();

	lock.unlock();

}

void CCollision_Manager::Add_Collision(const _uint& In_iLayer, weak_ptr<CCollider> In_pCollider)
{
	//std::unique_lock<std::mutex> lock(m_JobMutex);

	list<weak_ptr<CCollider>>::iterator iter = find_if(m_pColliderList[In_iLayer].begin(),
		m_pColliderList[In_iLayer].end(),
		[&](weak_ptr<CCollider> pPreOtherCollider)
		{
			if (!In_pCollider.lock().get() || !pPreOtherCollider.lock().get())
				return false;

			return pPreOtherCollider.lock()->Get_ColliderIndex() == In_pCollider.lock()->Get_ColliderIndex();
		});

	if (m_pColliderList[In_iLayer].end() == iter)
	{

		m_pReservedColliderList[In_iLayer].push_back(In_pCollider);
#ifdef _DEBUG_COUT_
		cout << In_pCollider.lock()->Get_ColliderIndex() << ": Add Collider" << endl;
#endif // _DEBUG_COUT_
	}
	//lock.unlock();
}

void CCollision_Manager::Check_Group(const _uint& In_iLeftLayer, const _uint& In_iRightLayer)
{
	_uint iRow((_uint)In_iLeftLayer); // 행
	_uint iCol((_uint)In_iRightLayer); // 열

	if (iRow > iCol)
	{
		iRow = In_iRightLayer;
		iCol = In_iLeftLayer;
	}

	if (m_arrCheck[iRow] & (1 << iCol)) // 이미 그 자리가 1이면
		m_arrCheck[iRow] &= ~(1 << iCol); // 빼주고
	else
		m_arrCheck[iRow] |= (1 << iCol);

}

void CCollision_Manager::Render_Collider()
{
	for (auto& elem_List : m_pColliderList)
	{
		for (auto& elem : elem_List)
			elem.lock()->Render(DEVICECONTEXT);
	}

}

void CCollision_Manager::Remove_DeadCollision()
{
	for (_uint i(0); i < m_pColliderList.size(); i++)
	{
		for (auto iter = m_pColliderList[i].begin(); iter != m_pColliderList[i].end();)
		{
			if (!(*iter).lock().get())
			{
				iter = m_pColliderList[i].erase(iter);
			}
			else if(!(*iter).lock().get()->Get_Enable())
			{
#ifdef _DEBUG_COUT_
				cout << (*iter).lock()->Get_ColliderIndex() << ": Remove Disable Collider" << endl;
#endif // _DEBUG_COUT_
				iter = m_pColliderList[i].erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

}

void CCollision_Manager::Update_ReservedColliderList()
{
	for (_size_t i = 0; i < m_pReservedColliderList.size(); ++i)
	{
		for (auto& elem : m_pReservedColliderList[i])
		{
			m_pColliderList[i].push_back(elem);
		}
		m_pReservedColliderList[i].clear();
	}

}

void CCollision_Manager::Update_CollisionGroup(const _uint& In_iLeftLayer, const _uint& In_iRightLayer)
{
	for (auto& elem_Left : m_pColliderList[In_iLeftLayer])
	{
		for (auto& elem_Right : m_pColliderList[In_iRightLayer])
		{
			//Nullptr인 객체가 있음.
			if (!elem_Left.lock().get() || !elem_Right.lock().get())
				DEBUG_ASSERT;

			if (elem_Left.lock().get() == elem_Right.lock().get())
			{
				continue;
			}

			if (Is_Collision(elem_Left, elem_Right))
			{
				elem_Left.lock()->OnCollision(elem_Right);
				elem_Right.lock()->OnCollision(elem_Left);
			}
		}
	}
}

_bool CCollision_Manager::Is_Collision(weak_ptr<CCollider> In_pLeft, weak_ptr<CCollider> In_pRight)
{
	_bool isCollision(false);

	if (!In_pLeft.lock()->Get_Enable() || !In_pRight.lock()->Get_Enable())
		return false;

	const COLLISION_TYPE& eLeftType = In_pLeft.lock().get()->Get_CollisionType();
	const COLLISION_TYPE& eRightType = In_pRight.lock().get()->Get_CollisionType();

	if (COLLISION_TYPE::AABB == eLeftType)
	{
		if (COLLISION_TYPE::AABB == eRightType)
			isCollision = In_pLeft.lock().get()->Get_BoundingBox().lock()->Intersects(*In_pRight.lock()->Get_BoundingBox().lock());
		if (COLLISION_TYPE::OBB == eRightType)
			isCollision = In_pLeft.lock().get()->Get_BoundingBox().lock()->Intersects(*In_pRight.lock()->Get_BoundingOrientedBox().lock());
		if (COLLISION_TYPE::SPHERE == eRightType)
			isCollision = In_pLeft.lock().get()->Get_BoundingBox().lock()->Intersects(*In_pRight.lock()->Get_BoundingSphere().lock());
	}

	if (COLLISION_TYPE::OBB == eLeftType)
	{
		if (COLLISION_TYPE::AABB == eRightType)
			isCollision = In_pLeft.lock().get()->Get_BoundingOrientedBox().lock()->Intersects(*In_pRight.lock()->Get_BoundingBox().lock());
		if (COLLISION_TYPE::OBB == eRightType)
			isCollision = In_pLeft.lock().get()->Get_BoundingOrientedBox().lock()->Intersects(*In_pRight.lock()->Get_BoundingOrientedBox().lock());
		if (COLLISION_TYPE::SPHERE == eRightType)
			isCollision = In_pLeft.lock().get()->Get_BoundingOrientedBox().lock()->Intersects(*In_pRight.lock()->Get_BoundingSphere().lock());
	}

	if (COLLISION_TYPE::SPHERE == eLeftType)
	{
		if (COLLISION_TYPE::AABB == eRightType)
			isCollision = In_pLeft.lock().get()->Get_BoundingSphere().lock()->Intersects(*In_pRight.lock()->Get_BoundingBox().lock());
		if (COLLISION_TYPE::OBB == eRightType)
			isCollision = In_pLeft.lock().get()->Get_BoundingSphere().lock()->Intersects(*In_pRight.lock()->Get_BoundingOrientedBox().lock());
		if (COLLISION_TYPE::SPHERE == eRightType)
			isCollision = In_pLeft.lock().get()->Get_BoundingSphere().lock()->Intersects(*In_pRight.lock()->Get_BoundingSphere().lock());
	}

	return isCollision;

}

void CCollision_Manager::End_CollisionCheck()
{
	for (_uint i(0); i < m_pColliderList.size(); i++)
	{
		for (auto& elem : m_pColliderList[i])
		{
			elem.lock()->End_CollisionCheck();
		}
	}

}

PxContactModifyCallback* CCollision_Manager::Get_CollisionCallBack() const
{
	return m_pCollisionCallBack;
}

PxSimulationEventCallback* CCollision_Manager::Get_CollisionSimulationEventCallBack()
{
	return m_pCollisionSimulationEventCallBack;
}

void CCollision_Manager::Free()
{
	m_pCollisionCallBack->Release();
	m_pCollisionSimulationEventCallBack->Release();
}

