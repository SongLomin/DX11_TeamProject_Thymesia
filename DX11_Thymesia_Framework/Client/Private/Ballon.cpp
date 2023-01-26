#include "stdafx.h"
#include "Ballon.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "MobWeapon.h"
#include "ActorDecor.h"
#include "NorMonStateS.h"
#include "Status_Monster.h"
//#include "MonsterWeapon.h"
//#include "Monster1States/Monster1States.h"
#include "Client_Components.h"
#include "MonsterHPBar_Base.h"
#include "VIBuffer_Trail.h"


GAMECLASS_C(CBallon);
CLONE_C(CBallon, CGameObject);

HRESULT CBallon::Initialize_Prototype()
{
	CMonster::Initialize_Prototype();

	return S_OK;
}

HRESULT CBallon::Initialize(void* pArg)
{
	CMonster::Initialize(pArg);

	m_pStatus = Add_Component<CStatus_Monster>(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxAnimModel"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	m_pStandState = Add_Component<CNorMonState_Idle>();
	Add_Component<CNorMonState_Die>();

	return S_OK;
}

HRESULT CBallon::Start()
{
	CMonster::Start();

	Change_State<CNorMonState_Idle>();

	Bind_HPBar();

	return S_OK;
}

void CBallon::Init_Desc()
{
	__super::Init_Desc();

	switch (m_tLinkStateDesc.eMonType)
	{
	case MONSTERTYPE::BALLOON:
		m_pModelCom.lock()->Init_Model("Balloon", "", (_uint)TIMESCALE_LAYER::MONSTER);
		m_pPhysXControllerCom.lock()->Enable_Gravity(false);
		break;
	}


	//TODO 여기서하는 이유는 몬스터가 배치되고 원점에서 우리가 피킹한위치만큼더해지고 난뒤에 그월드포지션값저장하기위해서 여기서함
	switch (m_tLinkStateDesc.eMonType)
	{
	case MONSTERTYPE::BALLOON:
		m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Z);

	}
	_vector vecStartPositon = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
	XMStoreFloat4(&m_tLinkStateDesc.m_fStartPositon, vecStartPositon);

	INIT_STATE(CNorMonState_Idle);
	INIT_STATE(CNorMonState_Die);


	string	strModelKey = Weak_StaticCast<CStatus_Monster>(m_pStatus).lock()->Get_Desc().m_szModelKey;
	Bind_KeyEvent(strModelKey.c_str());

	m_pPhysXControllerCom.lock()->Init_Controller(Preset::PhysXControllerDesc::PlayerSetting(m_pTransformCom), (_uint)PHYSX_COLLISION_LAYER::MONSTER);
}

void CBallon::Tick(_float fTimeDelta)
{
	CMonster::Tick(fTimeDelta);


}

void CBallon::LateTick(_float fTimeDelta)
{
	CMonster::LateTick(fTimeDelta);
}

HRESULT CBallon::Render(ID3D11DeviceContext* pDeviceContext)
{
	CMonster::Render(pDeviceContext);

	_int iPassIndex = 0;
	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();


	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (i == 2 || i == 3)
			continue;

		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			iPassIndex = 0;
		}
		else
		{
			if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture",i, aiTextureType_SPECULAR)))
			{
				iPassIndex = 4;
			}
			else
			{
				iPassIndex = 5;
			}
		}
		if (0 < m_iPassIndex)
			iPassIndex = m_iPassIndex;
		//m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);

		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, iPassIndex, "g_Bones", pDeviceContext);
		//m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

	return S_OK;
}

void CBallon::SetUp_ShaderResource()
{
	CMonster::SetUp_ShaderResource();

#ifndef _USE_THREAD_
	m_pModelCom.lock()->Update_BoneMatrices();
#endif // !_USE_THREAD_

}

void CBallon::Move_RootMotion_Internal()
{
	PxControllerFilters Filters;
	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	switch (m_tLinkStateDesc.eMonType)
	{
	case MONSTERTYPE::BALLOON:
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root");
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
		break;
	}

}

void CBallon::Respawn_Monster(_fvector In_vPosition)
{

}



void CBallon::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	CMonster::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CBallon::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	CMonster::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CBallon::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	CMonster::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CBallon::OnEventMessage(_uint iArg)
{
	CMonster::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		PxControllerFilters Filters;
		m_pPhysXControllerCom.lock()->Set_Position(XMLoadFloat4(&m_tLinkStateDesc.m_fStartPositon), 0.f, Filters);

		Change_State<CNorMonState_Idle>();
		Set_Enable(false);
		m_pStatus.lock()->Full_Recovery();
	}
}

void CBallon::OnEnable(void* _Arg)
{
	CMonster::OnEnable(_Arg);
}

void CBallon::OnDisable()
{
	CMonster::OnDisable();
}

void CBallon::OnDestroy()
{
	CMonster::OnDestroy();
}

void CBallon::Free()
{
}
