#include "stdafx.h"
#include "NorMonState/NorMonState_TakeExecution.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "Status_Monster.h"
#include "PhysXController.h"
#include "Weapon.h"
#include "MobWeapon.h"
#include "Inventory.h"
#include "PhysXController.h"
#include "PhysXCharacterController.h"


GAMECLASS_C(CNorMonState_TakeExecution);
CLONE_C(CNorMonState_TakeExecution, CComponent)

void CNorMonState_TakeExecution::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;


	switch (In_KeyIndex)
	{
	case 18:
		m_bTargetLook = false;
		break;
	case 60:
		switch (m_eMonType)
		{
		case Client::MONSTERTYPE::ARMORSPEARMAN:
			m_bAnimEnd = true;
			break;
		case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
			m_bAnimEnd = true;
			break;
		default:
			break;
		}
		break;
	}

}

HRESULT CNorMonState_TakeExecution::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_TakeExecution::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_TakeExecution::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmorLV1_Halberds_VS_TakeExecution");
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmorLV1_Halberds_VS_TakeExecution");
		break;
	}

	m_fDissolveTime = 4.f;

	m_bAnimEnd = false;

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_TakeExecution::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_TakeExecution::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	if (!m_bAnimEnd)
		m_pModelCom.lock()->Play_Animation(fTimeDelta);
	else
	{
		Get_OwnerMonster()->Set_PassIndex(7);
		m_fDissolveTime -= fTimeDelta;

		_float fDissolveAmount = SMath::Lerp(1.f, -0.1f, m_fDissolveTime / 4.f);
		Get_OwnerMonster()->Set_DissolveAmount(fDissolveAmount);
	}

	LIGHTDESC LightDesc = Get_OwnerMonster()->Get_LightDesc();

	LightDesc.fIntensity = LightDesc.fIntensity - fTimeDelta;
	if (LightDesc.fIntensity <= 0.f)
	{
		LightDesc.bEnable = false;
	}
	Get_OwnerMonster()->Set_LightDesc(LightDesc);

}

void CNorMonState_TakeExecution::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_fDissolveTime < 0.f)
	{
		m_pOwner.lock()->Set_Enable(false);
		weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
		list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();
		for (auto& elem : pWeapons)
			elem.lock()->Set_Enable(false);
	}

	Check_AndChangeNextState();


}

void CNorMonState_TakeExecution::OnStateStart(const _float& In_fAnimationBlendTime)
{

	//UI안까지고 디졸브해야되요 무기;까지포함해서
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_bTargetLook = true;

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		Weak_StaticCast<CNorMonster>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(0.75f, 0.75f, 0.75f));
		m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
		m_fOffSetX = -0.5f;
		m_fOffSetZ = 1.1f;
		break;
		Weak_StaticCast<CNorMonster>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		Weak_StaticCast<CNorMonster>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(0.75f, 0.75f, 0.75f));
		m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
		m_fOffSetX = -0.5f;
		m_fOffSetZ = 1.1f;
		break;
	}


	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CNorMonState_TakeExecution::Call_NextKeyFrame, this, placeholders::_1);

	m_pOwner.lock()->Get_ComponentByType<CStatus_Monster>().lock()->CallBack_UI_Disable();


	PxControllerFilters Filters;
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	weak_ptr<CCharacter> pOtherCharacter = Weak_StaticCast<CCharacter>(pCurrentPlayer);
	_matrix Monstermatrix = Get_OwnerMonster()->Get_Transform()->Get_WorldMatrix();

	_matrix PlayerPosMatrix = pCurrentPlayer.lock()->Get_Transform()->Get_WorldMatrix();
	//_matrix vOtherWorldMatrix = pCurrentPlayer.lock()->Get_Transform()->Get_WorldMatrix();
	_matrix                    vResultOtherWorldMatrix;
	vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(PlayerPosMatrix, XMVectorSet(m_fOffSetX, 0.f, m_fOffSetZ, 0.f));
	Weak_StaticCast<CCharacter>(Get_OwnerMonster()).lock()->Get_PhysX().lock()->Set_Position(
		vResultOtherWorldMatrix.r[3],
		GAMEINSTANCE->Get_DeltaTime(),
		Filters);



	pCurrentPlayer.lock()->Get_Transform()->Set_Look2D(-Monstermatrix.r[2]);

	Get_OwnerMonster()->Release_Monster();
	if (Check_RequirementIsTargeted())
		GET_SINGLE(CGameManager)->Release_Focus();


		
	



#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: Stop -> StopStop" << endl;
#endif
#endif


}

void CNorMonState_TakeExecution::OnStateEnd()
{
	__super::OnStateEnd();

	Weak_StaticCast<CNorMonster>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));


	m_fOffSetX = 0.f;
	m_fOffSetZ = 0.f;

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
	m_pPhysXControllerCom.lock()->Set_EnableSimulation(true);

	__super::OnStateEnd();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CNorMonState_TakeExecution::Call_NextKeyFrame, this, placeholders::_1);
}


void CNorMonState_TakeExecution::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_TakeExecution::Call_AnimationEnd, this, placeholders::_1);
}


void CNorMonState_TakeExecution::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	m_bAnimEnd = true;

	//Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}



_bool CNorMonState_TakeExecution::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	////올라가는애니메이션부터해야될듯
	if (m_bTargetLook)
	{
		switch (m_eMonType)
		{
		case Client::MONSTERTYPE::ARMORSPEARMAN:
			m_pPhysXControllerCom.lock()->Set_EnableSimulation(false);
			//Rotation_TargetToLookDir();
			break;
		case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
			m_pPhysXControllerCom.lock()->Set_EnableSimulation(false);
			//Rotation_TargetToLookDir();
			break;
		default:
			break;
	
		}  
	}




	return false;
}

void CNorMonState_TakeExecution::Free()
{

}