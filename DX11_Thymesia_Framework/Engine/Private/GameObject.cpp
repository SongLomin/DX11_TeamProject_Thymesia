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

	//�ڽ� ��ü�鵵 ���ó��

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
