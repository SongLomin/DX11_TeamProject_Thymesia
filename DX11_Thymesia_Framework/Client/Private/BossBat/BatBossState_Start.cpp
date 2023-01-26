#include "stdafx.h"
#include "BossBat/BatBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "BoneNode.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXCharacterController.h"
#include "UIManager.h"
#include "UI_ScriptQueue.h"
#include "UI_FadeMask.h"
#include "GameManager.h"


GAMECLASS_C(CBatBossState_Start);
CLONE_C(CBatBossState_Start, CComponent)

HRESULT CBatBossState_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Start::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_Seq_BossFightStart_V1");

	m_pLeftHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_l");
	m_pRightHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_r");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root");

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CBatBossState_Start::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;
	
	switch (In_iKeyIndex)
	{
	case 1249:
	{
		GET_SINGLE(CGameInstance)->PlaySound2D("BossBat_WingFlap_02.ogg", 1.f);
		GET_SINGLE(CGameInstance)->PlaySound2D("BossBat_JumpSmashLR.ogg", 1.f);
	}
	return;
	case 1436:
	{
		_matrix CombinedMatrix = Get_LeftHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.4f, 9.f, 3.f);
		Print_Vector(vPosition);

	}
	return;
	case 1439:
	{
		_vector vPosition = m_pOwner.lock()->Get_Transform()->Get_Position();
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.6f, 9.f, 3.f);
		Print_Vector(vPosition);
	}
	return;
	case 1420:
		//m_pPhysXControllerCom.lock()->Enable_Gravity(true);
		return;
	case 1442:
	{
		_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.5f, 9.f, 3.f);
	}
	return;
	case 1745:
	{
		_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.3f, 9.f, 3.f);
	}
	return;
	case 1839:
	{
		GET_SINGLE(CGameInstance)->PlaySound2D("Yarg_Yell_01.ogg", 1.f);
	}
		return;
	case 2041:
	{
		weak_ptr<CUI_FadeMask> pFadeMask = GAMEINSTANCE->Add_GameObject<CUI_FadeMask>(m_pOwner.lock()->Get_CreatedLevel());
		pFadeMask.lock()->Set_Fade_Delay(0.f, 1.f, 0.5f,0.5f, EASING_TYPE::LINEAR);
	}
	return;
	}
}

void CBatBossState_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	//m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X | (_byte)ROOTNODE_FLAG::Y| (_byte)ROOTNODE_FLAG::Z);

	weak_ptr<CUI_FadeMask> pFadeMask = GAMEINSTANCE->Add_GameObject<CUI_FadeMask>(m_pOwner.lock()->Get_CreatedLevel());
	pFadeMask.lock()->Set_Fade(1.f, 0.f, 1.5f, EASING_TYPE::LINEAR);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex,500);

	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CBatBossState_Start::Call_NextAnimationKey, this, placeholders::_1);

	//m_bStopAnimation = false;
	//m_fTimeAcc = 0.f;
	//m_bFadeOutTrigger = true;

	_matrix LocalMat = XMMatrixIdentity();
	LocalMat *= XMMatrixRotationX(XMConvertToRadians(-90.f));
	LocalMat *= XMMatrixRotationAxis(LocalMat.r[1], XMConvertToRadians(90.f));

	GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera", LocalMat, CINEMATIC_TYPE::CINEMATIC);
	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::PLAYERHUD);


#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif

}	

void CBatBossState_Start::OnStateEnd()
{
	__super::OnStateEnd();
	//m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X |  (_byte)ROOTNODE_FLAG::Z);

	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CBatBossState_Start::Call_NextAnimationKey, this, placeholders::_1);
	

	weak_ptr<CUI_FadeMask> pFadeMask = GAMEINSTANCE->Add_GameObject<CUI_FadeMask>(m_pOwner.lock()->Get_CreatedLevel());
	pFadeMask.lock()->Set_Fade(1.f, 0.f, 0.5f,EASING_TYPE::LINEAR);

}

void CBatBossState_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	GET_SINGLE(CGameManager)->End_Cinematic();
	GET_SINGLE(CGameManager)->Enable_Layer(OBJECT_LAYER::PLAYERHUD);

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);

	GAMEINSTANCE->Get_GameObjects<CUI_ScriptQueue>(LEVEL::LEVEL_STATIC).front().lock()->Call_SetScript_Bat_Appear();

	GAMEINSTANCE->PlayBGM("STAGE2_BOSS_0.mp3", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_BGM), 1.f);

	//GET_SINGLE(CUIManager)->Set_CloseCurtain(1.f);
}

void CBatBossState_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Start::Call_AnimationEnd, this, placeholders::_1);
}



void CBatBossState_Start::Free()
{

}

_bool CBatBossState_Start::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	
	
	return false;
}

