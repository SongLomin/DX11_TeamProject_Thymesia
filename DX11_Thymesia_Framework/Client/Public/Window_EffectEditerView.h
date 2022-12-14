#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CEffectGroup;
class CCustomEffectMesh;
class CPreViewAnimationModel;
class CEffect_Rect;

class CWindow_EffectEditerView final
	: public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_EffectEditerView)

public:
	enum class EFFECTRESOURCE_TYPE
	{
		MESH = 0,
		PARTICLE,
		TYPE_END
	};

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:

public:
	FDelegate<> CallBack_RemoveCurrentEffect;

public:
	void Call_SetCurrentEffect(weak_ptr<CEffectGroup> pEffectGroup, _uint iIndex);
	void Call_UpdatePreViewModel();
	void Call_SyncAnimation();
	

private:
	void Update_MeshInfo();
	void Update_ParticleInfo();

private:
	weak_ptr<CEffectGroup>		m_pCurrentEffectGroup;
	weak_ptr<CCustomEffectMesh> m_pCurrentEffectMesh;
	weak_ptr<CEffect_Rect>		m_pCurrentEffectParticle;

	EFFECTRESOURCE_TYPE			m_eCurrentEffectType = EFFECTRESOURCE_TYPE::TYPE_END;

	weak_ptr<CPreViewAnimationModel> m_pPreViewModel;

public:
	void Free();
};
END

