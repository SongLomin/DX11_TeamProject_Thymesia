#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
struct MODEL_DATA;
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

private:
	shared_ptr<MODEL_DATA>	m_pModelData;
	weak_ptr<CPreviewAnimationModel>	m_pPreviewAnimModel;
	weak_ptr<CPreview_Prop>				m_pPreviewModel;

private:
	virtual void OnDestroy() override;
	void Free();
};
END
