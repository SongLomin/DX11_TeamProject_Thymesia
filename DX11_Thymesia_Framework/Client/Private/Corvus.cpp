#include "stdafx.h"
#include "Corvus.h"
#include "Client_Components.h"
#include "CorvusStates/CorvusStates.h"
#include "Camera_Target.h"
#include "Corvus_DefaultSaber.h"
#include "Corvus_DefaultDagger.h"
#include "PhysXCollider.h"
#include "Light_Prop.h"
#include "Status_Player.h"
#include "PhysXController.h"

#include "Inventory.h"
#include "Talent_Effects.h"
#include "PlayerSkill_System.h"
#include "Skill_Axe.h"
#include "Skill_VargSword.h"
#include "StolenSkill.h"



#include "UI_BloodOverlay.h"
#include "UI_PauseMenu.h"
#include "UIManager.h"
#include "UI_Landing.h"
#include "UI_AppearEventVarg.h"
#include "Monster.h"
#include "PlayerSkillHeader.h"
#include "Talent_Effects.h"
#include "UI_Landing.h"
#include "BoneNode.h"



GAMECLASS_C(CCorvus)
CLONE_C(CCorvus, CGameObject)

HRESULT CCorvus::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvus::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	m_pShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_VtxAnimModel"), VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);

	m_pStatus = CGameObject::Add_Component<CStatus_Player>();


	json LoadedJson;
	string strCorvusComponentPath = m_szClientComponentPath + "Corvus.json";

	if (SUCCEEDED(CJson_Utility::Load_Json(strCorvusComponentPath.c_str(), LoadedJson)))
	{
		Load_FromJson(LoadedJson);
		//DEBUG_ASSERT;
	}
	//CStatus_Player::PLAYERDESC& pStatus_PlayerDesc = GET_SINGLE(CGameManager)->Get_PlayerStatusDesc();

	//m_pStatus.lock()->Set_Desc(&pStatus_PlayerDesc);
	//m_pStatus.lock()->Load_FromJson(m_szClientComponentPath + "Corvus/SaveData.json");

	CModel::NVCLOTH_MODEL_DESC NvClothDesc;
	Preset::NvClothMesh::CorvusSetting(NvClothDesc);

	//m_pModelCom.lock()->Init_Model("Corvus", "", (_uint)TIMESCALE_LAYER::PLAYER, &NvClothDesc);
	m_pModelCom.lock()->Init_Model("Corvus", "", (_uint)TIMESCALE_LAYER::PLAYER);
	//m_pModelCom.lock()->Init_Model("Corvus", "", (_uint)TIMESCALE_LAYER::PLAYER, (_flag)FLAG_INDEX::_2);

	m_TransformationMatrix = m_pModelCom.lock()->Get_TransformationMatrix();


	// Corvus_SD_Ladder_Climb_R_UP_End_Reverse
	m_pModelCom.lock()->Add_ReverseAnimation(m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Ladder_Climb_R_UP_End"), (_uint)TIMESCALE_LAYER::PLAYER);
	// Corvus_SD_Ladder_Climb_L_UP_End_Reverse
	m_pModelCom.lock()->Add_ReverseAnimation(m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Ladder_Climb_L_UP_End"), (_uint)TIMESCALE_LAYER::PLAYER);

	m_pModelCom.lock()->Set_RootNode("root_$AssimpFbx$_Translation", (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Z);


	m_iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();

	this->Ready_Weapon();
	this->Ready_States();
	Ready_Skills();


	GET_SINGLE(CGameManager)->Set_CurrentPlayer(Weak_StaticCast<CPlayer>(m_this));

#ifdef _CORVUS_EFFECT_
	// Key Frame Effect ON
	Bind_KeyEvent("Corvus");

	// TODO : need to disable at Destroy/Disable
	// Passive Effect ON
	m_EffectIndexList.push_back
	({
		"Corvus_PassiveFeather",
		GET_SINGLE(CGameManager)->Use_EffectGroup("Corvus_PassiveFeather", m_pTransformCom, (_uint)TIMESCALE_LAYER::PLAYER)
	});

	GET_SINGLE(CGameManager)->Set_CurrentPlayer(Weak_StaticCast<CPlayer>(m_this));

#endif // _CORVUS_EFFECT_
	LIGHTDESC LightDesc;

	LightDesc.eActorType = LIGHTDESC::TYPE_SPOTLIGHT;
	LightDesc.vDiffuse = { 1.f,0.95f,0.8f,1.f };
	LightDesc.vSpecular = { 1.f,0.95f,0.8f,1.f };
	LightDesc.vAmbient = { 1.f,0.95f,0.8f,1.f };
	LightDesc.fIntensity = 0.4f;
	LightDesc.fRange =5.f;
	LightDesc.fCutOff = cosf(XMConvertToRadians(40.f));
	LightDesc.fOuterCutOff = cosf(XMConvertToRadians(60.f));
	_vector vPlayerPos = m_pTransformCom.lock()->Get_Position();
	_vector vPlayerLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);

	_vector vLightPos = vPlayerPos + XMVectorSet(0.f, 1.5f, 0.f, 0.f) - vPlayerLook * 0.5f;
	_vector vLightLook = XMVector3Normalize(vPlayerPos - vLightPos + vPlayerLook*0.5f);

	XMStoreFloat4(&LightDesc.vPosition, vLightPos);
	XMStoreFloat4(&LightDesc.vDirection, vLightLook);

	LightDesc.bEnable = true;

	m_LightDesc = GAMEINSTANCE->Add_Light(LightDesc);

#ifdef _USE_THREAD_
	//Use_Thread(THREAD_TYPE::PRE_BEFORERENDER);
#endif // _USE_THREAD_
	

	return S_OK;
}

HRESULT CCorvus::Start()
{
	__super::Start();
	CCharacter::Change_State<CCorvusState_Idle>();

	m_pCamera = GET_SINGLE(CGameManager)->Get_TargetCamera();

	if (m_pCamera.lock())
		m_pCameraTransform = m_pCamera.lock()->Get_Component<CTransform>();


	Test_BindSkill();



#ifdef _CLOTH_
	// m_pModelCom.lock()->Set_NvClothMeshWithIndex(0);
#endif // _CLOTH_
	
	return S_OK;
}

void CCorvus::Tick(_float fTimeDelta)
{
	fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::PLAYER);

	__super::Tick(fTimeDelta);

	// TODO : get rid of this
	m_pSkillSystem.lock()->Tick(fTimeDelta);
	m_pStatus.lock()->Tick(fTimeDelta);

	_vector vPlayerPos = m_pTransformCom.lock()->Get_Position();
	_vector vPlayerLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);

	_vector vLightPos = vPlayerPos + XMVectorSet(0.f, 1.5f, 0.f, 0.f) - vPlayerLook * 0.5f;
	_vector vLightLook = XMVector3Normalize(vPlayerPos - vLightPos + vPlayerLook * 0.5f);

	XMStoreFloat4(&m_LightDesc.vPosition,vLightPos);
	XMStoreFloat4(&m_LightDesc.vDirection, vLightLook);

	GAMEINSTANCE->Set_LightDesc(m_LightDesc);

	// TODO : Test For RimLight Power
	if (KEY_INPUT(KEY::DELETEKEY, KEY_STATE::TAP))
	{
		Set_RimLightDesc(0.5f, { 0.6f,0.f,0.f }, 1.f);
	}
	
	Update_KeyInput(fTimeDelta);
	Debug_KeyInput(fTimeDelta);

}

void CCorvus::Thread_PreLateTick(_float fTimeDelta)
{
	__super::Thread_PreLateTick(fTimeDelta);
}

void CCorvus::LateTick(_float fTimeDelta)
{
	fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::PLAYER);

	__super::LateTick(fTimeDelta);
}

void CCorvus::Thread_PreBeforeRender(_float fTimeDelta)
{
	__super::Thread_PreBeforeRender(fTimeDelta);

	ID3D11DeviceContext* pDeferredContext = GAMEINSTANCE->Get_BeforeRenderContext();

	_matrix		BoneMatrix;
	_matrix		InverseMatrix;
	_vector		vGravity;


	//BoneMatrix = //m_pModelCom.lock()->Find_BoneNode("Bip001-Head").lock()->Get_OffsetMatrix() * 
	//	m_pModelCom.lock()->Find_BoneNode("Bip001-Head").lock()->Get_CombinedMatrix() *
	//	XMLoadFloat4x4(&m_TransformationMatrix) * m_pTransformCom.lock()->Get_WorldMatrix();

	//_vector vSpherePos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	//vSpherePos = XMVector3TransformCoord(vSpherePos, BoneMatrix);
	//PxVec4 spheres[2]{
	//	PxVec4(SMath::Convert_PxVec3(vSpherePos), 0.3f),
	//	PxVec4(999.f, 999.f, 999.f, 1.0f),
	//};
	//nv::cloth::Range<const physx::PxVec4> sphereRange(spheres, spheres + 2);

	//m_pModelCom.lock()->Get_MeshContainer(1).lock()->Get_NvCloth()->setSpheres(sphereRange, 0, m_pModelCom.lock()->Get_MeshContainer(1).lock()->Get_NvCloth()->getNumSpheres());

	//Bip001-Ponytail1

	/*BoneMatrix = m_pModelCom.lock()->Find_BoneNode("Bip001-L-Clavicle").lock()->Get_OffsetMatrix() *
		m_pModelCom.lock()->Find_BoneNode("Bip001-L-Clavicle").lock()->Get_CombinedMatrix() *
		XMLoadFloat4x4(&m_TransformationMatrix);

	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);*/

	//InverseMatrix = XMMatrixInverse(nullptr, BoneMatrix * m_pTransformCom.lock()->Get_WorldMatrix());

	//vGravity = XMVector3TransformNormal(XMVectorSet(0.f, -9.81f, 0.f, 0.f), XMMatrixRotationX(XMConvertToRadians(90.f)) * InverseMatrix);
	//vGravity = XMVector3TransformNormal(XMVectorSet(0.f, -9.81f, 0.f, 0.f), InverseMatrix * XMMatrixRotationX(XMConvertToRadians(-90.f)));

	m_pModelCom.lock()->Get_MeshContainer(2).lock()->Update_NvClothVertices(pDeferredContext,
		m_pTransformCom.lock()->Get_WorldMatrix(),
		XMVectorSet(0.f, -9.81f, 0.f, 0.f));


	GAMEINSTANCE->Release_BeforeRenderContext(pDeferredContext);

}


void CCorvus::Before_Render(_float fTimeDelta)
{
	__super::Before_Render(fTimeDelta);


	
	//GAMEINSTANCE->Enqueue_Job(bind(&CModel::Update_NvCloth, m_pModelCom.lock(), pDeferredContext));
}	



HRESULT CCorvus::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);
	 _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i(0); i < m_iNumMeshContainers; ++i)
	{
#ifdef _DEBUG
		//if (i == m_iContainerIndex)
		//	continue;
#endif // _DEBUG

		

		if (4 == i || 5 == i || 8 == i || 9 == i || 10 == i || 11 == i|| 12 == i|| 13 == i)
		{
			unordered_map<_uint, DISSOLVE_DESC>::iterator iter = m_DissolveDescs.find(i);

			if (iter == m_DissolveDescs.end())
			{
				continue;
			}

			m_pShaderCom.lock()->Set_RawValue("g_bGlow", &iter->second.bGlow, sizeof(_bool));
			m_pShaderCom.lock()->Set_RawValue("g_bBloom", &iter->second.bBloom, sizeof(_bool));
			m_pShaderCom.lock()->Set_RawValue("g_vGlowColor", &iter->second.vGlowColor, sizeof(_float4));


			m_pShaderCom.lock()->Set_RawValue("g_vDissolveDir", &iter->second.vDirection, sizeof(_float3));
			m_pShaderCom.lock()->Set_RawValue("g_vDissolveStartPos", &iter->second.vStartPos, sizeof(_float3));
			m_pShaderCom.lock()->Set_RawValue("g_fDissolveAmount", &iter->second.fAmount, sizeof(_float));

			_float4 vShaderFlag = { 0.f,0.f,1.f,1.f };
			_float4 vRimLightColor = { 0.f, 1.f, 0.408f, 1.f };

			m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_float4));
			m_pShaderCom.lock()->Set_RawValue("g_vRimLightColor", &vRimLightColor, sizeof(_float4));

			m_iPassIndex = 6;
		}
		else
		{

			_float4 vShaderFlag = { 0.f,0.f,0.f,0.f };

			m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_float4));

			_float4 vRimLightDesc = { 0.f, 0.f, 0.f, 0.f };
			vRimLightDesc.x = m_vRimLightColor.x;
			vRimLightDesc.y = m_vRimLightColor.y;
			vRimLightDesc.z = m_vRimLightColor.z;
			vRimLightDesc.w = m_fRimLightPower;

			m_pShaderCom.lock()->Set_RawValue("g_vRimLightColor", &vRimLightDesc, sizeof(_float4));

			if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
				m_iPassIndex = 4;
			else
			{
				if (i == 2)
					m_iPassIndex = 8;
				else
					m_iPassIndex = 5;
			}
		}
		m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);

		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			m_iPassIndex = 0;


		/*if (2 == i)
		{
			m_iPassIndex = 9;

			m_pShaderCom.lock()->Set_Matrix("g_WorldMatrix", XMMatrixIdentity());
		}
		else
		{
			m_pShaderCom.lock()->Set_Matrix("g_WorldMatrix", m_pTransformCom.lock()->Get_WorldMatrix());
		}*/

		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, m_iPassIndex, "g_Bones", pDeviceContext);

#ifdef _DEBUG
		//if (i == 2)
		//	continue;
#endif // _DEBUG
	}

	m_DissolveDescs.clear();

	return S_OK;
}

void CCorvus::OnStealMonsterSkill(MONSTERTYPE eMonstertype)
{
	__super::OnStealMonsterSkill(eMonstertype);

	m_pSkillSystem.lock()->OnStealMonsterSkill(eMonstertype);

}


void CCorvus::Debug_KeyInput(_float fTimeDelta)
{
	PxControllerFilters Filters;

	// TODO : test jump key R
	
	if (GET_SINGLE(CUIManager)->Is_OpenedMenu())
		return;

	if (KEY_INPUT(KEY::R, KEY_STATE::HOLD))
		m_pPhysXControllerCom.lock()->Move(_vector{ 0.f, 500.f * fTimeDelta, 0.f }, 0.f, fTimeDelta, Filters);

	if (KEY_INPUT(KEY::Y, KEY_STATE::HOLD))
		m_pPhysXControllerCom.lock()->Move(_vector{ 0.f, 0.f, 500.f * fTimeDelta }, 0.f, fTimeDelta, Filters);

	if (KEY_INPUT(KEY::NUM0, KEY_STATE::TAP))
	{
		static _bool bEnable;
		bEnable = !bEnable;
		m_pPhysXControllerCom.lock()->Set_EnableSimulation(bEnable);
	}
	//Item Debug
	if (KEY_INPUT(KEY::NUM3, KEY_STATE::TAP))
	{
		m_pInventory.lock()->Push_Item(ITEM_NAME::BASIL);

	}
	if (KEY_INPUT(KEY::NUM4, KEY_STATE::TAP))
	{
		m_pInventory.lock()->Push_Item(ITEM_NAME::THYME);
	}
	if (KEY_INPUT(KEY::NUM5, KEY_STATE::TAP))
	{
		m_pInventory.lock()->Push_Item(ITEM_NAME::GARDEN_KEY);
		m_pInventory.lock()->Push_Item(ITEM_NAME::VARG_KEY);
		m_pInventory.lock()->Push_Item(ITEM_NAME::CINNAMON);
		m_pInventory.lock()->Push_Item(ITEM_NAME::MEMORY02);
	}
	if (KEY_INPUT(KEY::NUM6, KEY_STATE::TAP))
	{
		m_pInventory.lock()->Push_Item(ITEM_NAME::MEMORY01);
		m_pInventory.lock()->Push_Item(ITEM_NAME::MEMORY02);
	}
	if (KEY_INPUT(KEY::NUM1, KEY_STATE::TAP))
	{
		json	CorvusJson;

		Write_Json(CorvusJson);
		
		string corvusComponentPath = m_szClientComponentPath + "Corvus.json";

		CJson_Utility::Save_Json(corvusComponentPath.c_str(), CorvusJson);


		//GAMEINSTANCE->Get_GameObjects<CUI_AppearEventVarg>(LEVEL_STATIC).front().lock()->Start_Event();
	}
	if (KEY_INPUT(KEY::BACKSPACE, KEY_STATE::TAP))
	{
		GAMEINSTANCE->Get_GameObjects<CUI_Landing>(LEVEL_STATIC).front().lock()->Call_Landing(
			CUI_Landing::LANDING_ENTER_STAGE);	
	}
	if (KEY_INPUT(KEY::INSERTKEY, KEY_STATE::TAP))
	{
		Change_State<CCorvusState_Die>();
	}
	if (KEY_INPUT(KEY::V, KEY_STATE::TAP))
	{
		OnStealMonsterSkill(MONSTERTYPE::AXEMAN);
	}
#ifdef _DEBUG
	//if (KEY_INPUT(KEY::UP, KEY_STATE::TAP))
	//{
	//	++m_iContainerIndex;
	//	if (m_iContainerIndex >= m_iNumMeshContainers)
	//		m_iContainerIndex = 0;
	//	cout << "m_iContainerIndex : " << m_iContainerIndex << endl;
	//}
#endif // _DEBUG
}

void CCorvus::Update_KeyInput(_float fTimeDelta)
{
	if (KEY_INPUT(KEY::C, KEY_STATE::TAP))
	{
		m_pSkillSystem.lock()->SwapSkillMaintoSub();
	}
	
}

void CCorvus::Ready_Weapon()
{
	m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CCorvus_DefaultSaber>(m_CreatedLevel));
	m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");
	m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CCorvus_DefaultDagger>(m_CreatedLevel));
	m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_l");
}

void CCorvus::Ready_States()
{
	// Macro for convenience to add state component
	// undefines after use
#define ADD_STATE_MACRO(state_class_name)\
		Add_Component<state_class_name>()

	m_pStandState = ADD_STATE_MACRO(CCorvusState_Idle);
	ADD_STATE_MACRO(CCorvusState_Jogging);
	ADD_STATE_MACRO(CCorvusState_JoggingStart);
	ADD_STATE_MACRO(CCorvusState_JoggingStartEnd);
	ADD_STATE_MACRO(CCorvusState_Run);
	ADD_STATE_MACRO(CCorvusState_Sprint);
	ADD_STATE_MACRO(CCorvusState_SprintStart);
	ADD_STATE_MACRO(CCorvusState_AVoid);
	ADD_STATE_MACRO(CVarg_Execution);
	ADD_STATE_MACRO(CCorvusState_SprintAttack);
	ADD_STATE_MACRO(CCorvusState_LAttack1);
	ADD_STATE_MACRO(CCorvusState_LAttack2);
	ADD_STATE_MACRO(CCorvusState_LAttack3);
	ADD_STATE_MACRO(CCorvusState_LAttack4);
	ADD_STATE_MACRO(CCorvusState_LAttack5);
	ADD_STATE_MACRO(CCorvusState_Parry1);
	ADD_STATE_MACRO(CCorvusState_Parry2);
	ADD_STATE_MACRO(CCorvusState_BasicHealing);
	ADD_STATE_MACRO(CCorvusState_ClawAttackTab);
	ADD_STATE_MACRO(CCorvusState_ClawAttackTab2);
	ADD_STATE_MACRO(CCorvusState_ClawAttackHold);
	ADD_STATE_MACRO(CCorvusState_ClawAttackAway);
	ADD_STATE_MACRO(CCorvusState_ClawAttackHoldLoop);
	ADD_STATE_MACRO(CCorvusState_Die);
	ADD_STATE_MACRO(CCorvusState_HurtL);
	ADD_STATE_MACRO(CCorvusState_HurtR);
	ADD_STATE_MACRO(CCorvusState_HurtXXL);
	ADD_STATE_MACRO(CCorvusState_HurtXL);
	ADD_STATE_MACRO(CCorvusState_NorMob_Execution);
	ADD_STATE_MACRO(CCorvusState_ParryDeflectLeft);
	ADD_STATE_MACRO(CCorvusState_ParryDeflectLeftup);
	ADD_STATE_MACRO(CCorvusState_ParryDeflectRight);
	ADD_STATE_MACRO(CCorvusState_ParryDeflectRightup);
	ADD_STATE_MACRO(CCorvusState_PS_Axe);
	ADD_STATE_MACRO(CCorvusState_PS_CaneSword);
	ADD_STATE_MACRO(CCorvusState_PS_Knife);
	ADD_STATE_MACRO(CCorvusState_PS_Scythe);
	ADD_STATE_MACRO(CCorvusState_PS_Scythe_Upgrade);
	ADD_STATE_MACRO(CCorvusState_PS_Hammer);
	ADD_STATE_MACRO(CCorvusState_PS_Hammer_Upgrade);
	ADD_STATE_MACRO(CCorvusState_PS_Magician);
	ADD_STATE_MACRO(CCorvusState_PS_UrdSword);
	ADD_STATE_MACRO(CCorvusState_PS_VargSword);
	ADD_STATE_MACRO(CCorvusState_AVoidSalsh);
	ADD_STATE_MACRO(CCorvusState_AVoidThrust);
	ADD_STATE_MACRO(CCorvusState_Climb_L_Down);
	ADD_STATE_MACRO(CCorvusState_Climb_L_Down_End);
	ADD_STATE_MACRO(CCorvusState_Climb_L_Idle);
	ADD_STATE_MACRO(CCorvusState_Climb_L_UP);
	ADD_STATE_MACRO(CCorvusState_Climb_L_UP_End);
	ADD_STATE_MACRO(CCorvusState_Climb_R_Down);
	ADD_STATE_MACRO(CCorvusState_Climb_R_Down_End);
	ADD_STATE_MACRO(CCorvusState_Climb_R_Idle);
	ADD_STATE_MACRO(CCorvusState_Climb_R_UP);
	ADD_STATE_MACRO(CCorvusState_Climb_R_UP_End);
	ADD_STATE_MACRO(CCorvusState_Climb_Start);
	ADD_STATE_MACRO(CCorvusState_Fall_End);
	ADD_STATE_MACRO(CCorvusState_Fall_Loop);
	ADD_STATE_MACRO(CCorvusState_Fall_Start);
	ADD_STATE_MACRO(CCorvusState_Climb_Start);
	ADD_STATE_MACRO(CCorvusState_Climb_Fall_Attack);
	ADD_STATE_MACRO(CCorvusState_Climb_L_Up_Down_End);
	ADD_STATE_MACRO(CCorvusState_Climb_R_Up_Down_End);
	ADD_STATE_MACRO(CCorvusState_RaidAttack1Hurt);
	ADD_STATE_MACRO(CCorvusState_RunB);
	ADD_STATE_MACRO(CCorvusState_RunL);
	ADD_STATE_MACRO(CCorvusState_RunR);
	ADD_STATE_MACRO(CCorvusState_RunFR);
	ADD_STATE_MACRO(CCorvusState_RunFL);
	ADD_STATE_MACRO(CCorvusState_RunBR);
	ADD_STATE_MACRO(CCorvusState_RunBL);
	ADD_STATE_MACRO(CCorvusState_AVoidB);
	ADD_STATE_MACRO(CCorvusState_AVoidL);
	ADD_STATE_MACRO(CCorvusState_AVoidR);
	ADD_STATE_MACRO(CCorvusState_AVoidF);
	ADD_STATE_MACRO(CCorvusState_Getup);
	ADD_STATE_MACRO(CCorvusState_Headache_End);
	ADD_STATE_MACRO(CCorvusState_Headache_Start);
	ADD_STATE_MACRO(CCorvusState_Headache_Loop);
	ADD_STATE_MACRO(CCorvusState_FeatherAttack);
	ADD_STATE_MACRO(CCorvusState_ClawPlunderAttack);
	ADD_STATE_MACRO(CCorvusState_LongHealing);
	ADD_STATE_MACRO(CCorvusState_ShortHealing);
	ADD_STATE_MACRO(CCorvusState_Short_AvoidF);
	ADD_STATE_MACRO(CCorvusState_Short_AvoidB);
	ADD_STATE_MACRO(CCorvusState_Short_AvoidL);
	ADD_STATE_MACRO(CCorvusState_Short_AvoidR);
	ADD_STATE_MACRO(CCorvusState_Long_AvoidF);
	ADD_STATE_MACRO(CCorvusState_Long_AvoidB);
	ADD_STATE_MACRO(CCorvusState_Long_AvoidR);
	ADD_STATE_MACRO(CCorvusState_Long_AvoidL);
	ADD_STATE_MACRO(CCorvusState_Short_Claw_Atk1);
	ADD_STATE_MACRO(CCorvusState_Short_Claw_Atk2);
	ADD_STATE_MACRO(CCorvusState_Short_Claw_Atk3);
	ADD_STATE_MACRO(CCorvusState_HurtBlown);
	ADD_STATE_MACRO(CCorvusState_Bat_Execution);
	ADD_STATE_MACRO(CCorvusState_HurtFallDown);
	ADD_STATE_MACRO(CCorvusState_HurtFallDownEnd);
	ADD_STATE_MACRO(CCorvusState_KnockBack);
	ADD_STATE_MACRO(CCorvusState_PS_VargSwordStart);





	ADD_STATE_MACRO(CCorvusState_CheckPointStart);
	ADD_STATE_MACRO(CCorvusState_CheckPointEnd);
	ADD_STATE_MACRO(CCorvusState_CheckPointLoop);
	ADD_STATE_MACRO(CCorvusState_Joker_Execution);
	ADD_STATE_MACRO(CCorvusState_Execution_Start);
	ADD_STATE_MACRO(CCorvusState_Varg_Execution);
	ADD_STATE_MACRO(CCorvusState_Execution_R_R);
	ADD_STATE_MACRO(CCorvusState_Urd_Execution);

#undef ADD_STATE_MACRO
}

void CCorvus::Ready_Skills()
{
	Add_Component<CSkill_VargSword>();
	Add_Component<CSkill_Axe>();
	Add_Component<CStolenSkill>();
	Add_Component<CSkill_Knife>();
	Add_Component<CSkill_Hammer>();
	Add_Component<CSkill_Scythe>();


	//스킬 추가입니다.
	m_pSkillSystem = Add_Component<CPlayerSkill_System>();

}

void CCorvus::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}

void CCorvus::Move_RootMotion_Internal()
{
	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation") * XMLoadFloat3(&m_vMoveScale);
	//m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviMeshCom);

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, GAMEINSTANCE->Get_DeltaTime(), Filters, nullptr, m_pTransformCom);
}

void CCorvus::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{

	__super::OnCollisionEnter(pMyCollider, pOtherCollider);


	switch ((COLLISION_LAYER)pOtherCollider.lock()->Get_CollisionLayer())
	{
	case Client::COLLISION_LAYER::LADDER_UP:
		m_CollisionObjectFlags |= (_flag)COLISIONOBJECT_FLAG::LADDERUP;
		m_bLadderCheck = true;
		break;
	case Client::COLLISION_LAYER::LADDER_DOWN:
		m_CollisionObjectFlags |= (_flag)COLISIONOBJECT_FLAG::LADDERDOWN;
		m_bLadderCheck = true;
		break;
	case Client::COLLISION_LAYER::ELEVATOR:
		m_CollisionObjectFlags |= (_flag)COLISIONOBJECT_FLAG::ELEVATOR;
		break;
	case Client::COLLISION_LAYER::DOOR:
		m_CollisionObjectFlags |= (_flag)COLISIONOBJECT_FLAG::DOOR;
		break;
	case Client::COLLISION_LAYER::CHECKPOINT:
		m_CollisionObjectFlags |= (_flag)COLISIONOBJECT_FLAG::CHECKPOINT;
		break;
	case Client::COLLISION_LAYER::ITEM:
		m_CollisionObjectFlags |= (_flag)COLISIONOBJECT_FLAG::ITEM;
		break;
	}

}

void CCorvus::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CCorvus::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider,pOtherCollider);

	switch ((COLLISION_LAYER)pOtherCollider.lock()->Get_CollisionLayer())
	{
	case Client::COLLISION_LAYER::LADDER_UP:
		m_CollisionObjectFlags &= !(_flag)COLISIONOBJECT_FLAG::LADDERUP;
		m_bLadderCheck = false;
		break;
	case Client::COLLISION_LAYER::LADDER_DOWN:
		m_CollisionObjectFlags &= !(_flag)COLISIONOBJECT_FLAG::LADDERDOWN;
		m_bLadderCheck = false;
		break;
	case Client::COLLISION_LAYER::ELEVATOR:
		m_CollisionObjectFlags &= !(_flag)COLISIONOBJECT_FLAG::ELEVATOR;
		break;
	case Client::COLLISION_LAYER::DOOR:
		m_CollisionObjectFlags &= !(_flag)COLISIONOBJECT_FLAG::DOOR;
		break;
	case Client::COLLISION_LAYER::CHECKPOINT:
		m_CollisionObjectFlags &= !(_flag)COLISIONOBJECT_FLAG::CHECKPOINT;
		break;
	case Client::COLLISION_LAYER::ITEM:
		m_CollisionObjectFlags &= !(_flag)COLISIONOBJECT_FLAG::ITEM;
		break;
	}

	
	
}

void CCorvus::OnBattleEnd()
{
}
 
void CCorvus::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);
}

void CCorvus::OnDisable()
{
	__super::OnDisable();
}


void CCorvus::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_VARGEXECUTION == iArg)
	{
		Change_State<CCorvusState_Execution_R_R>();
	}

	if ((_uint)EVENT_TYPE::ON_SITUP == iArg)
	{
		Change_State<CCorvusState_CheckPointEnd>();
	}

	if ((_uint)EVENT_TYPE::ON_JOKEREXECUTION == iArg)
	{
		Change_State<CCorvusState_Joker_Execution>();
	}

	if ((_uint)EVENT_TYPE::ON_DIE == iArg)
	{
		Change_State<CCorvusState_Die>();
	}

	if ((_uint)EVENT_TYPE::ON_BATEXECUTION == iArg)
	{
		Change_State<CCorvusState_Execution_Start>();
	}

	if (EVENT_TYPE::ON_STEALCORVUS == (EVENT_TYPE)iArg)
	{
		Change_State<CCorvusState_ClawPlunderAttack>();
	}

	if (EVENT_TYPE::ON_URDEXECUTON == (EVENT_TYPE)iArg)
	{
		Change_State<CCorvusState_Execution_R_R>();
	}

}

void CCorvus::Free()
{

}

void CCorvus::SetUp_Requirement()
{

}

void CCorvus::Test_BindSkill()
{
	m_pSkillSystem.lock()->OnChangeSkill(Get_Component<CSkill_Hammer>(), CPlayerSkill_System::SOCKET_TYPE::SOCKET_MAIN);
	m_pSkillSystem.lock()->OnChangeSkill(Get_Component<CSkill_Scythe>(), CPlayerSkill_System::SOCKET_TYPE::SOCKET_SUB);
}

void CCorvus::Save_ClientComponentData()
{
	json	CorvusJson;

	Write_Json(CorvusJson);

	string corvusComponentPath = m_szClientComponentPath + "Corvus.json";

	CJson_Utility::Save_Json(corvusComponentPath.c_str(), CorvusJson);
}

void CCorvus::WriteTalentFromJson(json& Out_Json)
{

}

void CCorvus::LoadTalentFromJson(const json& In_Json)
{
}

