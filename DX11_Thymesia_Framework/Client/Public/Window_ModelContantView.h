#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
struct MODEL_DATA;
struct MATERIAL_DATA;
struct ANIMATION_DATA;
END

BEGIN(Client)
class CPreviewAnimationModel;
class CPreview_Prop;

class CWindow_ModelContantView final
	: public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_ModelContantView)

public:
	void Open_ModelFile(const MODEL_TYPE In_ModelType);

public:
	virtual HRESULT Initialize() override;
	virtual void	Start() override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
	void Save_Model();

private:
	void Load_PreviewAnimModel(shared_ptr<MODEL_DATA> pModelData);
	void Load_PreviewModel(shared_ptr<MODEL_DATA> pModelData);

	void Edit_Texture(const aiTextureType In_TextureType, shared_ptr<MATERIAL_DATA> pMaterialData, const _int In_iIndex);
	void Clip_AnimationName();
	void Edit_Animation(shared_ptr<ANIMATION_DATA> pAnimationData, const _int In_iIndex);

private:
	shared_ptr<MODEL_DATA>	m_pModelData;
	weak_ptr<CPreviewAnimationModel>	m_pPreviewAnimModel;
	weak_ptr<CPreview_Prop>				m_pPreviewModel;
	_bool								m_bAnimModel = false;
	_bool								m_bPlay = true;
	_int								m_iClipLength{};

private:
	virtual void OnDestroy() override;
	void Free();
};
END
