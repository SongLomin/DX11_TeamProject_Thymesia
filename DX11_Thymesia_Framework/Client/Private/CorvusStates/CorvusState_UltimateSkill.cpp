#include "stdafx.h"
#include "LuxiyaStates/LuxiyaState_UltimateSkill.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "LuxiyaStates/LuxiyaStates.h"
#include "GameManager.h"
#include "LuxiyaUltimateUI.h"
#include "LuxiyaUltimateEffect.h"
#include "Camera_Target.h"

GAMECLASS_C(CLuxiyaState_UltimateSkill);
CLONE_C(CLuxiyaState_UltimateSkill, CComponent)

HRESULT CLuxiyaState_UltimateSkill::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	m_iAnimIndex = 13;
	m_fFixedPlayRatio = 0.5f;

	return S_OK;
}

HRESULT CLuxiyaState_UltimateSkill::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pLuxiyaUltimateUIs.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateUI>(m_CreatedLevel));
	m_pLuxiyaUltimateUIs.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateUI>(m_CreatedLevel));
	m_pLuxiyaUltimateUIs.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateUI>(m_CreatedLevel));
	m_pLuxiyaUltimateUIs.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateUI>(m_CreatedLevel));
	m_pLuxiyaUltimateUIs.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateUI>(m_CreatedLevel));
	m_pLuxiyaUltimateUIs.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateUI>(m_CreatedLevel));
	m_pLuxiyaUltimateUIs.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateUI>(m_CreatedLevel));
	m_pLuxiyaUltimateUIs.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateUI>(m_CreatedLevel));
	m_pLuxiyaUltimateUIs.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateUI>(m_CreatedLevel));
	m_pLuxiyaUltimateUIs.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateUI>(m_CreatedLevel));
	m_pLuxiyaUltimateUIs.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateUI>(m_CreatedLevel));

	m_pLuxiyaUltimateEffects.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateEffect>(m_CreatedLevel));
	m_pLuxiyaUltimateEffects.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateEffect>(m_CreatedLevel));
	m_pLuxiyaUltimateEffects.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateEffect>(m_CreatedLevel));
	m_pLuxiyaUltimateEffects.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateEffect>(m_CreatedLevel));
	m_pLuxiyaUltimateEffects.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateEffect>(m_CreatedLevel));
	m_pLuxiyaUltimateEffects.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateEffect>(m_CreatedLevel));
	m_pLuxiyaUltimateEffects.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateEffect>(m_CreatedLevel));
	m_pLuxiyaUltimateEffects.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateEffect>(m_CreatedLevel));
	m_pLuxiyaUltimateEffects.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateEffect>(m_CreatedLevel));
	m_pLuxiyaUltimateEffects.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateEffect>(m_CreatedLevel));
	m_pLuxiyaUltimateEffects.push_back(GAMEINSTANCE->Add_GameObject<CLuxiya_UltimateEffect>(m_CreatedLevel));

	return S_OK;
}

void CLuxiyaState_UltimateSkill::Start()
{
	__super::Start();

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CLuxiyaState_UltimateSkill::Call_AnimationEnd, this);



}

void CLuxiyaState_UltimateSkill::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("Bip001");
	m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviCom);

	if (KEY_INPUT(KEY::N, KEY_STATE::TAP))
	{
		if (m_fDebugAnimationSpeed < 0.5f)
		{
			m_fDebugAnimationSpeed = 1.f;
		}

		else
		{
			m_fDebugAnimationSpeed = 0.1f;
		}
	}

	Attack();
}

void CLuxiyaState_UltimateSkill::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	if (Check_AndChangeNextState())
	{
		Get_OwnerCharacter().lock()->Set_RigidColliderEnable(true);

	}
}

void CLuxiyaState_UltimateSkill::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CLuxiyaState_Stand>();

}

void CLuxiyaState_UltimateSkill::Call_OnNextKeyFrame(const _uint& In_iKeyIndex)
{
	TCHAR;

	switch (In_iKeyIndex)
	{
	case 39:
		Divide_Display();
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_UltiHit.wav"), 1.f);
		break;

	case 43:
		Divide_Display();
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_UltiHit2.wav"), 1.f);
		break;

	case 48:
		Divide_Display();
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_UltiHit.wav"), 1.f);
		break; 

	case 50:
		Divide_Display();
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_UltiHit2.wav"), 1.f);
		break;

	case 51:
		Divide_Display();
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_UltiHit.wav"), 1.f);
		break;

	case 53:
		Divide_Display();
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_UltiHit2.wav"), 1.f);
		break;

	case 55:
		Divide_Display();
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_UltiHit.wav"), 1.f);
		break;

	case 59:
		Divide_Display();
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_UltiHit2.wav"), 1.f);
		break;

	case 64:
		Divide_Display();
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_UltiHit.wav"), 1.f);
		break;

	case 67:
		Divide_Display();
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_UltiHit2.wav"), 1.f);
		break;

	case 70:
		Divide_Display();
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_UltiHit.wav"), 1.f);
		break;

	case 95:
		Divide_Display();
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_UltiHit2.wav"), 1.f);
		break;


	}

}


void CLuxiyaState_UltimateSkill::Attack()
{

}

void CLuxiyaState_UltimateSkill::Divide_Display()
{
	for (auto& elem : m_pLuxiyaUltimateUIs)
	{
		if (!elem.lock()->Get_Enable())
		{
			elem.lock()->Start_Effect(m_iCurrentDivideMaskIndex);
			break;
		}
	}

	for (auto& elem : m_pLuxiyaUltimateEffects)
	{
		if (!elem.lock()->Get_Enable())
		{
			elem.lock()->Start_Effect(m_iCurrentDivideMaskIndex);
			break;
		}
	}

	++m_iCurrentDivideMaskIndex;

	GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_AddHit.wav"), 1.f);
}

void CLuxiyaState_UltimateSkill::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	Get_OwnerPlayer()->Set_DodgeTime(4.5f);

	m_iCurrentDivideMaskIndex = 3;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pModelCom.lock()->Get_CurrentAnimation().lock()->CallBack_NextChannelKey +=
		bind(&CLuxiyaState_UltimateSkill::Call_OnNextKeyFrame, this, placeholders::_1);
	
	GET_SINGLE(CGameManager)->Get_TargetCamera().lock()->Cinema_LuxiyaUltimate();
	GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_Ulti.wav"), 1.f);
	GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_UltiWeapon.wav"), 1.f);



#ifdef _DEBUG
	cout << "LuxiyaState: ReSkill -> OnStateStart" << endl;

#endif
}

void CLuxiyaState_UltimateSkill::OnStateEnd()
{
	__super::OnStateEnd();

	Disable_Weapons();
	m_IsNextAttack = false;
	m_iAttackIndex = 0;
	Get_OwnerCharacter().lock()->Set_RigidColliderEnable(true);
	Set_OriginalWeaponsScale();

	m_pModelCom.lock()->Get_CurrentAnimation().lock()->CallBack_NextChannelKey -=
		bind(&CLuxiyaState_UltimateSkill::Call_OnNextKeyFrame, this, placeholders::_1);
}

void CLuxiyaState_UltimateSkill::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CLuxiyaState_UltimateSkill::Call_AnimationEnd, this);
}

void CLuxiyaState_UltimateSkill::Free()
{

}

_bool CLuxiyaState_UltimateSkill::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;


	return false;
}



