#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider :
    public CComponent
{

    GAMECLASS_H(CCollider);
	SHALLOW_COPY(CCollider);
    DECLARE_CLONABLE(CCollider, CComponent);

public:
	COLLISION_TYPE Get_CollisionType() const { return m_eType; }
	_uint Get_CollisionLayer() const { return m_ColliderDesc.iLayer; }
	weak_ptr<BoundingSphere> Get_BoundingSphere() const { return m_pSphere; }
	weak_ptr<BoundingBox> Get_BoundingBox() const { return m_pAABB; }
	weak_ptr<BoundingOrientedBox> Get_BoundingOrientedBox() const { return m_pOBB; }
	_uint Get_ColliderIndex() const { return m_iColliderIndex; }
	COLLIDERDESC Get_ColliderDesc() const;

	_vector Get_CurrentPosition() const; 

	void Set_ColliderScale(_fvector In_vScale);

private:

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

	_bool Intersect_Ray(const RAY& In_Ray, float* Out_pDistance = nullptr) const;

	void Init_Collider(const COLLISION_TYPE& In_ColliderType, const COLLIDERDESC& In_ColliderDesc);
	void Reset_OtherColliders();

public:
	void Update(_fmatrix TransformMatrix);
	void Update_AddPosition(const _float3& In_vPosition);
	void OnCollision(weak_ptr<CCollider> pOtherCollider);
	void End_CollisionCheck();
	

	//_bool Collision(weak_ptr<CCollider> pTargetCollider);
	_matrix Remove_Rotation(_fmatrix TransformMatrix);

public:
	FDelegate<weak_ptr<CCollider>> CallBack_CollisionEnter;
	FDelegate<weak_ptr<CCollider>> CallBack_CollisionStay;
	FDelegate<weak_ptr<CCollider>> CallBack_CollisionExit;

private:
	void CollisionEnter(weak_ptr<CCollider> pOtherCollider);
	void CollisionStay(weak_ptr<CCollider> pOtherCollider);
	void CollisionExit(weak_ptr<CCollider> pOtherCollider);

public:
	void	Set_DebugColor(_fvector In_vColor);
	HRESULT Render(ID3D11DeviceContext* pDeviceContext);
	HRESULT Render_IgnoreDebugCheck();
	HRESULT Render_IgnoreDebugCheck_Color();

private:
	_float4	m_vColor = { 1.f, 1.f, 1.f, 1.f };
	_bool	m_bEditerDraw = false;

private:
	static	_uint							m_iClonedColliderIndex;
	_uint									m_iColliderIndex;

	weak_ptr<CCollider>						m_thisFromColliderCom;

	shared_ptr<BoundingSphere>				m_pSphere_Original;
	shared_ptr<BoundingBox>					m_pAABB_Original;
	shared_ptr<BoundingOrientedBox>			m_pOBB_Original;

	shared_ptr<BoundingSphere>				m_pSphere;
	shared_ptr<BoundingBox>					m_pAABB;
	shared_ptr<BoundingOrientedBox>			m_pOBB;
	COLLISION_TYPE							m_eType = COLLISION_TYPE::TYPE_END;
	COLLIDERDESC							m_ColliderDesc;

	list<weak_ptr<CCollider>>				m_pPreOtherColliders;
	list<weak_ptr<CCollider>>				m_pOtherColliders;

	_bool									m_isColl = false;

	_float3									m_vCurrentPosition;

private: /* For. Debug_Collider */
	shared_ptr<BasicEffect> m_pEffect;
	shared_ptr<PrimitiveBatch<VertexPositionColor>> m_pBatch;
	ComPtr<ID3D11InputLayout> m_pInputLayout;

protected:
	//객체의 상태가 활성화 상태로 변경될 때, 호출되는 이벤트입니다.
	virtual void OnEnable(void* _Arg = nullptr) override;

	//객체의 상태가 비활성화 상태로 변경될 때, 호출되는 이벤트입니다.
	virtual void OnDisable() override;
	

protected:
	virtual void OnDestroy() override;
	void Free();

};

END