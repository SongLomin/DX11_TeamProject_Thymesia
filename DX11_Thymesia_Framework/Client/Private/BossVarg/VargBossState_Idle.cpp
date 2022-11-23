#include "stdafx.h"
#include "BossVarg/VargBossState_Idle.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"



GAMECLASS_C(CVargBossState_Idle);
CLONE_C(CVargBossState_Idle, CComponent)

HRESULT CVargBossState_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Idle::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_Idle");
	

	/*m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Idle::Call_AnimationEnd, this);*/
}

void CVargBossState_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CVargBossState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_Idle::OnStateEnd()
{
	__super::OnStateEnd();


}

void CVargBossState_Idle::Free()
{

}

_bool CVargBossState_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // �÷��̾�� ���� �Ÿ�

	//���� 8����Ŭ��

	if (fPToMDistance > 8.f)
	{
		int iRand = rand() % 3;

		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_RaidAttack>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_RunAttack>(0.05f);
			break;
		case 2:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_RunStart>(0.05f);
			break;
		}

		return true;

		/*
		* �Ÿ��� 8����ũ�� ������ ������
		* 1. ������������ �Ÿ����̴¹�� <�����ΰ������� ����ũ�������� �̰Ŵ·��̵����>
		* 2. �޷����� �ֵηΰ� ��� �������� <������>
		* 3. �޷����鼭 �ٷ� ���� <��-> ����->1 >
		*/
	}

	else if (4.f >= fPToMDistance || 8.f <= fPToMDistance)
	{
		int iRand = rand() % 5;

		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack3a>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidL>(0.05f);
			break;
		case 2:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidR>(0.05f);
			break;
		case 3:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidB>(0.05f);
			break;
		case 4:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidAttack>(0.05f);
			break;
		}

		return true;
		/*
		�Ÿ���5����ũ�ų����� 8���ڴ��۰ų�������
		1. ��� ����
		2. �翷���� ���ٰ� ��������
		3. �׳��������ݹٷ� ?�̰ǻ�����
		*/
	}

	else if (4.f < fPToMDistance)
	{
		int iRand = rand() % 2;

		//���ϸ� ���Ѵ��� �ű⼭ ���������ؾ��ϰ�
		// �Ͼ��ϰ� �ٷΰ����� ���⼭ó���Ҽ�����

		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack1a>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack2a>(0.05f);
			break;
		}

		return true;
		/*
		1. �������ͺ��� �ڿ������� ���� �׸��� ��ŷ
		2. �������ͺ��� �ڿ������� �ڷ�ȸ������
		3. �������� 1. ��� ��� ���
		4. �������� 2. �ձ��� ������ ��� ? 
		5. �������� 3. �ձ��� ��� 3������ 2-1 -> 2-2 , 2-1 -> 2-2B , 2-1->2-2B1 
		
		*/
	}


	return false;
}

