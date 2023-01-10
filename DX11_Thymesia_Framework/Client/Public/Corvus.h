#pragma once
#include "Player.h"

BEGIN(Client)
class CTalent_Sword;
class CPlayerSkill_System;
class CCamera_Target;
class CNvClothCollider;

class CCorvus final :
	public CPlayer
{
	GAMECLASS_H(CCorvus);
	CLONE_H(CCorvus, CGameObject);

private:
	virtual HRESULT Initialize_Prototype()         override;
	virtual HRESULT Initialize(void* pArg)         override;
	virtual HRESULT Start()                        override;
	virtual void Tick(_float fTimeDelta)           override;
	virtual void Thread_PreLateTick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta)       override;
	virtual void Thread_PreBeforeRender(_float fTimeDelta) override;
	virtual void Before_Render(_float fTimeDelta)  override;
	virtual void SetUp_ShaderResource() override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;


private:

	void Update_KeyInput(_float fTimeDelta);
	void Debug_KeyInput(_float fTimeDelta);

	virtual void Move_RootMotion_Internal() override;

	void Test_BindSkill();

	void Ready_Weapon();
	void Ready_States();
	void Ready_Skills();

	void WriteTalentFromJson(json& Out_Json);
	void LoadTalentFromJson(const json& In_Json);
	virtual void Save_ClientComponentData() override;

public:
	void Set_MoveScale(const _float3& In_vMoveScale) { m_vMoveScale = In_vMoveScale; }
private:
	void Calculate_Inversion(_float In_fTimeDelta, _bool& In_bEnd);

private:
	weak_ptr<CNvClothCollider> m_pNvClothColliderCom;

private:
	LIGHTDESC m_LightDesc;
	_float3 m_vMoveScale{ 1.f,1.f,1.f };

	weak_ptr<CTalent_Sword>  m_pSword;
	_uint                    m_iContainerIndex = 0;

	weak_ptr<CPlayerSkill_System> m_pSkillSystem;
	weak_ptr<CCamera_Target> m_pCamera;
	weak_ptr<CTransform> m_pCameraTransform;

	_float4x4 m_TransformationMatrix;
	
	_float m_fInversionStrength = 1.f;
	_float m_fInversionRatio = 0.f;

private:
	FDelegate<_float, _bool&> CallBack_ColorInversion;
	FDelegate< _float, _bool&> CallBack_LightEvent;

private:
	void TurnOn_Lignt(_float fTimeDelta, _bool& Out_End);

private:
	virtual void OnEnable(void* pArg) override;
	virtual void OnDisable() override;

	virtual void OnStealMonsterSkill(MONSTERTYPE eMonstertype) override;
	virtual void OnEventMessage(_uint iArg) override;

	virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
	virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
	virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

	void Free();
};

END