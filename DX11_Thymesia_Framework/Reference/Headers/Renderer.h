#pragma once

/* 화면에 그려져야할 객체들을 그리는 순서대로 모아서 관리하낟.  */
/* 가지고 있는 객체들의 렌더함수를 호출해주낟. */

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer : public CComponent
{
	GAMECLASS_H(CRenderer)
	DECLARE_CLONABLE(CRenderer, CComponent)
	 
public:
	HRESULT Add_RenderGroup(RENDERGROUP	eGroup, weak_ptr<class CGameObject> pGameObject);
	//텍스처 처리
	/*HRESULT Set_Textures_From_Key(const _tchar* _Str_Key, MEMORY_TYPE _eType = MEMORY_TYPE::MEMORY_END);
	HRESULT Bind_Texture(_uint _iIndex);
	HRESULT UnBind_Texture();
	LPDIRECT3DBASETEXTURE9 Get_Texture(_uint _iIndex);*/

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

protected:
	void Free();
};

END