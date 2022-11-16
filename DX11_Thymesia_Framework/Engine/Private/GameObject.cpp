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
	USE_START(CGameObject);
	m_pTransformCom = Add_Component<CTransform>();
	m_iGameObjectIndex = g_iGameObjectIndex++;
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
	CallBack_Tick(fTimeDelta);
}

void CGameObject::LateTick(_float fTimeDelta)
{
	CallBack_LateTick(fTimeDelta);
}

void CGameObject::Before_Render(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	CallBack_Render();

	return S_OK;
}

HRESULT CGameObject::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix)
{
	return S_OK;
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
	m_ThreadFlag |= (1 << (_uint)In_Type);

}

void CGameObject::UnUse_Thread(const THREAD_TYPE In_Type)
{
	if (m_ThreadFlag & (1 << (_uint)In_Type)) // 이미 그 자리가 1이면
	{
		// 빼준다.
		m_ThreadFlag &= ~(1 << (_uint)In_Type);
	}

}

_bool CGameObject::Is_Thread(const THREAD_TYPE In_Type)
{
	return m_ThreadFlag & (1 << (_uint)In_Type);
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
