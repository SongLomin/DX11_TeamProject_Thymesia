#include "..\Public\Renderer.h"
#include "GameObject.h"

GAMECLASS_C(CRenderer)
IMPLEMENT_CLONABLE(CRenderer, CComponent)

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, weak_ptr<CGameObject> pGameObject)
{
	/*if (eGroup >= RENDERGROUP::RENDER_END ||
		nullptr == pGameObject.lock().get())
		return E_FAIL;

	m_RenderObjects[(_uint)eGroup].push_back(pGameObject);*/

	GAMEINSTANCE->Add_RenderGroup(eGroup, pGameObject);

	return S_OK;
}

HRESULT CRenderer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRenderer::Initialize(void* pArg)
{
	return S_OK;
}

void CRenderer::Start()
{
}

//HRESULT CRenderer::Draw_RenderGroup()
//{
//	for (_uint i = 0; i < (_uint)RENDERGROUP::RENDER_END; ++i)
//	{
//		if (i == (_uint)RENDERGROUP::RENDER_ALPHABLEND)
//		{
//			m_RenderObjects[i].sort([](weak_ptr<CGameObject> pSour, weak_ptr<CGameObject> pDest)
//			{
//				return pSour.lock()->Get_CamDistance() > pDest.lock()->Get_CamDistance();
//			});			
//		}
//
//		for (auto& pGameObject : m_RenderObjects[i])
//		{
//			if (nullptr != pGameObject.lock().get())
//				pGameObject.lock().get()->Render(pDeviceContext);
//		}
//		m_RenderObjects[i].clear();
//	}
//
//	return S_OK;
//}

void CRenderer::Free()
{

}
