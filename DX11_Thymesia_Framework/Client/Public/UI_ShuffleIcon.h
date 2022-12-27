#pragma once
#include "CustomUI.h"


BEGIN(Client)

class CItem;
class CEasingComponent_Transform;
class CEasingComponent_Float;
class CEasingComponent_Bezier;

class CUI_ItemSlot;

class CUI_ShuffleIcon : public  CCustomUI
{
public:
	GAMECLASS_H(CUI_ShuffleIcon)
	CLONE_H(CUI_ShuffleIcon, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

	void			Bind_Item(weak_ptr<CItem> pItem);
	weak_ptr<CItem>	Get_BindItem() { return m_pBindItem; }
	void			Unbind_Item();


	void							Bind_Target(weak_ptr<CUI_ItemSlot> pItemSlot);
	weak_ptr<CUI_ItemSlot> 			Get_Target() { return m_pTargetItemSlot; }

	void			Pull_Lerp();
	void			Start_Lerp(_float fLerpTime);



	void			Start_SwapLerp(_float2 TargetPos, _float fWeight, _float fLerpTime);
	
private:
	void			Call_TransformLerpEnd();

	void			Call_ScaleUpEnd();
	void			Call_ScaleDownEnd();

private:
	void			SetUp_Component() override;

private:
	weak_ptr<CItem>	m_pBindItem;
	weak_ptr<CUI_ItemSlot>	m_pTargetItemSlot;
	weak_ptr<CEasingComponent_Transform>	m_pEasingTransform;
	weak_ptr<CEasingComponent_Bezier>		m_pEasingBezier;

	weak_ptr<CEasingComponent_Float>		m_pEasingScale;


private:
	void	Free();

};

END

