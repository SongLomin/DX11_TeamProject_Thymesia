#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
class CAnimation;
END

BEGIN(Client)
class CPreViewAnimationModel;


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
	virtual HRESULT Render() override;

public:
	void Call_UpdatePreViewModel();
	void Call_NextAnimationKey(const _uint& In_Key);

private:
	void Add_EffectKeyEvent();
	void Add_EnableColliderEvent();
	void Add_DisableColliderEvent();
	
	void Save_KeyEvent();
	void Load_KeyEvent();
	void Clear_KeyEvent();
	void ClearBack_KeyEvent();
	void ClearAll_KeyEvent();

	void Draw_Player();
	void Draw_KeyEventEditer();
	void Draw_AnimationList();

public:
	_bool								m_bKeyEventEditer = false;

private:
	weak_ptr<CPreViewAnimationModel>	m_pPreViewModel;
	weak_ptr<CAnimation>				m_pCurrentAnimation;

	_float m_fCurrentTime = 0.f;
	_float m_fMaxAnimationTime = 0.f;
	_float m_fSpeed = 1.f;

	_bool m_bStop = false;
	_bool m_bHold = false;

	vector<string>	m_AllAnimationKeys;
	_int			m_CurrentAnimationIndex = 0;

private:
	json m_KeyEventJson;
	map<_int, list<string>>	m_KeyEventEffectGroupNames;

public:
	void Free();
};
END

