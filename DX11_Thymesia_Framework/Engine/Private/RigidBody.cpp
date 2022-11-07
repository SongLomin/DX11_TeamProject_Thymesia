#include "RigidBody.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"

GAMECLASS_C(CRigidBody)
CLONE_C(CRigidBody, CComponent)

CRigidBody::CRigidBody(const CRigidBody& rhs)
{
	*this = rhs;

}

HRESULT CRigidBody::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRigidBody::Initialize(void* pArg)
{
	USE_START(CRigidBody);

	m_fDrag = 4.5f;
	m_fMass = 0.f;
	m_fMaxSpeed = 20.f;
	m_bUseGravity = false;

	return S_OK;
}

void CRigidBody::Start()
{
	m_pMyTransform = m_pOwner.lock()->Get_Component<CTransform>();

	USE_LATETICK(CRigidBody);
}

void CRigidBody::Tick(_float fTimeDelta)
{
}

void CRigidBody::LateTick(_float fTimeDelta)
{
	_vector vMoveValue;

	vMoveValue = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	for (_uint i = 0; i < FORCE_MODE::FORCE_MODE_END; i++)
	{
		for (auto iter = m_Forces[i].begin(); iter != m_Forces[i].end();)
		{
			(*iter).fPower -= (m_fDrag + m_fMass) * fTimeDelta;
			//(((*iter).fPower * 0.001f) + 

			//이동 값이 없는 경우
			if ((*iter).fPower < 0.f)
			{
				iter = m_Forces[i].erase(iter);
			}

			// 이동 값이 아직 있는 경우
			else
			{
				vMoveValue += XMLoadFloat3(&(*iter).vDir) * (*iter).fPower;
				iter++;
			}
		}
	}

	m_pMyTransform.lock()->Add_Position(vMoveValue);
}

void CRigidBody::Add_Force(_vector In_vDir, const _float& In_fPower, const FORCE_MODE& In_ForceMode)
{
	_float3 vDirFloat3;
	XMStoreFloat3(&vDirFloat3, In_vDir);

	if (fabsf(vDirFloat3.x + vDirFloat3.y + vDirFloat3.z) < DBL_EPSILON)
	{
		return;
	}

	FORCE tForce{ vDirFloat3, In_fPower };

	if (MODE_FORCE == In_ForceMode)
	{
		m_Forces[MODE_FORCE].clear();
		m_Forces[MODE_FORCE].push_back(tForce);
	}

	else if(MODE_IMPURSE == In_ForceMode)
	{
		m_Forces[MODE_IMPURSE].push_back(tForce);
	}

}

void CRigidBody::OnDestroy()
{
	UNUSE_START(CRigidBody);
	UNUSE_LATETICK(CRigidBody);

}

void CRigidBody::Free()
{
}
