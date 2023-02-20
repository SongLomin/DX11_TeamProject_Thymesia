#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
class CAnimation;
END

BEGIN(Client)
class CPreviewAnimationModel;


class CWindow_AnimationPlayerView final
	: public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_AnimationPlayerView)

public:
	_float Get_Speed() const { return m_fSpeed; }
	_bool Is_Stop() const { return m_bStop; }

public:
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
	void Call_UpdatePreViewModel();
	void Call_NextAnimationKey(const _uint& In_Key);

private:
	void Add_EffectKeyEvent();
	void Add_EnableWeaponEvent(const _bool In_bEnable);
	void Add_SoundKeyEvent();
	void Add_RandomSoundKeyEvent();
	
	void Save_KeyEvent();
	HRESULT Load_KeyEvent();
	void Clear_KeyEvent();
	void ClearBack_KeyEvent();
	void ClearAll_KeyEvent();
	void Clear_WeaponEvent();

	void Draw_Player();
	void Draw_KeyEventEditer();
	void Draw_AnimationList();

public:
	void OnUpdateSoundFile(const string& In_szFileName);

public:
	_bool								m_bKeyEventEditer = false;

private:
	weak_ptr<CPreviewAnimationModel>	m_pPreViewModel;
	weak_ptr<CAnimation>				m_pCurrentAnimation;

	_float m_fCurrentTime = 0.f;
	_float m_fMaxAnimationTime = 0.f;
	_float m_fSpeed = 1.f;

	_bool m_bStop = false;
	_bool m_bHold = false;

	vector<string>	m_AllAnimationKeys;
	_int			m_CurrentAnimationIndex = 0;

	string m_strKeyEventFileName = "";
	string m_strSoundFileName = "";

public:
	json Get_KeyEventJson() const { return m_KeyEventJson; }

private:
	json m_KeyEventJson;
	weak_ptr<CModel>		m_pCurrentModelCom;
	unordered_map<_int, list<string>>	m_KeyEventEffectGroupNames;

public:
	void Free();
};
END

