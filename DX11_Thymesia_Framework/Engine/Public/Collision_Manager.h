#pragma once
#include "Base.h"



BEGIN(Engine)

class CCollider;
class CollisionCallBack;
class CollisionSimulationEventCallBack;

class CCollision_Manager final : public CBase
{
	DECLARE_SINGLETON(CCollision_Manager)

public: /* For. MyCollision */
	void Initialize(const _uint& In_iNumLayer);
	void Tick();
	void Add_Collision(const _uint& In_iLayer, weak_ptr<CCollider> In_pCollider);
	void Check_Group(const _uint& In_iLeftLayer, const _uint& In_iRightLayer);
	void Render_Collider();

private:
	void Remove_DeadCollision();
	void Update_ReservedColliderList();
	void Update_CollisionGroup(const _uint& In_iLeftLayer, const _uint& In_iRightLayer);
	_bool Is_Collision(weak_ptr<CCollider> In_pLeft, weak_ptr<CCollider> In_pRight);
	void End_CollisionCheck();
	
private:
	vector<list<weak_ptr<CCollider>>>	m_pColliderList; // 충돌 컴포넌트들만 충돌 타입별로 모아놓은 리스트
	vector<list<weak_ptr<CCollider>>>	m_pReservedColliderList; // 충돌 컴포넌트들만 충돌 타입별로 모아놓은 리스트
	vector<_uint>						m_arrCheck;

public: /* For. PhysxCollision */
	PxContactModifyCallback*	Get_CollisionCallBack() const;
	PxSimulationEventCallback* Get_CollisionSimulationEventCallBack();

private:
	CollisionCallBack* m_pCollisionCallBack = nullptr;
	CollisionSimulationEventCallBack* m_pCollisionSimulationEventCallBack = nullptr;

	std::mutex m_JobMutex;

public:
	void Free();
};

END
