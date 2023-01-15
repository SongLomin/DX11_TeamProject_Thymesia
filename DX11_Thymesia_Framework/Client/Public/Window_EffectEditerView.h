#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CEffectGroup;
class CCustomEffectMesh;
class CPreviewAnimationModel;
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
		SOUND,
		TYPE_END
	};

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
	_float Get_SoundVolume() { return m_fVolume; }

public:
	FDelegate<> CallBack_RemoveCurrentEffect;

public:
	void Call_SetCurrentEffect(weak_ptr<CEffectGroup> pEffectGroup, _uint iIndex);
	void Call_UpdatePreViewModel();
	void Call_SyncAnimation();
	void Call_SelectSoundFile(const _char* In_szSoundFileName);
	

private:
	void Update_MeshInfo();
	void Update_ParticleInfo();
	void Update_Sound();

private:
	weak_ptr<CEffectGroup>		m_pCurrentEffectGroup;
	weak_ptr<CCustomEffectMesh> m_pCurrentEffectMesh;
	weak_ptr<CEffect_Rect>		m_pCurrentEffectParticle;

	EFFECTRESOURCE_TYPE			m_eCurrentEffectType = EFFECTRESOURCE_TYPE::TYPE_END;

	weak_ptr<CPreviewAnimationModel> m_pPreViewModel;

	_int m_iSoundSyncKey = 0;
	_bool m_bIsSound3d = true;

public:
	_int Get_SoundSyncKey() const { return m_iSoundSyncKey; }
	std::string Get_SoundFileName() const { return m_szSoundFileName; }
	_float Get_SoundVolume() const { return m_fVolume; }
	const _bool Is_Sound3D() const { return m_bIsSound3d; }

private: /* For. Sound */
	string						m_szSoundFileName;
	_float						m_fVolume{};


public:
	void Free();
};
END

