#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Transform.h"

_uint CGameObject::g_iGameObjectIndex;

GAMECLASS_C(CGameObject)

CGameObject::CGameObject(const CGameObject & Prototype)
{

}

_uint CGameObject::Get_GameObjectIndex()
{
	return m_iGameObjectIndex;
}


HRESULT CGameObject::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	//USE_START(CGameObject);
	m_pTransformCom = Add_Component<CTransform>();
	m_iGameObjectIndex = g_iGameObjectIndex++;
	m_thisToGameObject = Weak_StaticCast<CGameObject>(m_this);

	return S_OK;
}

HRESULT CGameObject::Start()
{
	/*for (auto& elem : m_pComponents)
	{
		elem.second->Start();
	}*/

	return S_OK;
}

void CGameObject::Tick(_float fTimeDelta)
{
}

void CGameObject::LateTick(_float fTimeDelta)
{
}

void CGameObject::Before_Render(_float fTimeDelta)
{
}

void CGameObject::Thread_PreTick(_float fTimeDelta)
{
}

void CGameObject::Thread_PreLateTick(_float fTimeDelta)
{
}

void CGameObject::Thread_PreBeforeRender(_float fTimeDelta)
{
}

void CGameObject::Thread_PreRender(_float fTimeDelta)
{
}

HRESULT CGameObject::Render(ID3D11DeviceContext* pDeviceContext)
{
	CallBack_Render();

	return S_OK;
}

HRESULT CGameObject::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext)
{
	return S_OK;
}

const unordered_map<_hashcode, list<shared_ptr<CComponent>>>& CGameObject::Get_AllComponents() const
{
	return m_pComponents;
}

void CGameObject::Write_Json(json& Out_Json)
{
	for (auto& elem_List : m_pComponents)
	{
		for (auto& elem_Com : elem_List.second)
		{
			elem_Com->Write_Json(Out_Json["Component"]);
		}
	}
}

void CGameObject::Load_FromJson(const json& In_Json)
{
	for (auto& elem_List : m_pComponents)
	{
		for (auto& elem_Com : elem_List.second)
		{
			elem_Com->Load_FromJson(In_Json["Component"]);
		}
	}
}

void CGameObject::OnDestroy()
{
	for (auto& elem_List : m_pComponents)
	{
		for (auto& elem_Com : elem_List.second)
		{
			elem_Com->OnDestroy();
		}
	}
	UNUSE_START(CGameObject);
}

void CGameObject::Set_OwnerForMyComponents()
{
	for (auto& elem_List : m_pComponents)
	{
		for (auto& elem_Com : elem_List.second)
		{
			elem_Com->Set_Owner(dynamic_pointer_cast<CGameObject>(m_this.lock()));
		}
	}
}

void CGameObject::Set_Dead()
{
	m_bDead = true;
	OnSetDead();

}

void CGameObject::Use_Thread(const THREAD_TYPE In_Type)
{
	if (m_ThreadFlag & (1 << (_flag)In_Type))
		return;

	m_ThreadFlag |= (1 << (_flag)In_Type);

	GET_SINGLE(CThread_Manager)->Bind_ThreadObject(In_Type, Weak_StaticCast<CGameObject>(m_this));
}

void CGameObject::UnUse_Thread(const THREAD_TYPE In_Type)
{
	if (m_ThreadFlag & (1 << (_flag)In_Type)) // 이미 그 자리가 1이면
	{
		// 빼준다.
		m_ThreadFlag &= ~(1 << (_flag)In_Type);
	}

}

_bool CGameObject::Is_Thread(const THREAD_TYPE In_Type)
{
	return m_ThreadFlag & (1 << (_flag)In_Type);
}

_bool CGameObject::LastestDataFromThread(const THREAD_TYPE In_Type)
{
	return m_UpatedFromThreadFlag & (1 << (_flag)In_Type);
}

void CGameObject::OnEventMessage(_uint iArg)
{
	for (auto& elem_List : m_pComponents)
	{
		for (auto& elem_Com : elem_List.second)
		{
			elem_Com->OnEventMessage(iArg);
		}
	}

}

void CGameObject::OnEngineEventMessage(const ENGINE_EVENT_TYPE In_eEngineEvent)
{
	GET_SINGLE(CComponent_Manager)->Receive_EngineEventMessage(m_thisToGameObject, In_eEngineEvent);
}

void CGameObject::Free()
{
	__super::Free();
	m_pComponents.clear();
}

void CGameObject::Remove_Component(weak_ptr<CComponent> pComponent)
{
	for (auto& elem_list : m_pComponents)
	{
		for (auto iter = elem_list.second.begin(); iter != elem_list.second.end();)
		{
			if ((*iter) == pComponent.lock())
			{
				(*iter)->OnDestroy();
				elem_list.second.erase(iter);
				return;
			}
			else
			{
				++iter;
			}
		}

	}
	
}
