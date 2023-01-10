#include "stdafx.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Character.h"
#include "JavelinWeapon.h"
#include "BossUrd/Urd.h"

GAMECLASS_C(CJavelinWeapon);
CLONE_C(CJavelinWeapon, CGameObject);

void CJavelinWeapon::Set_JavelinState(const JAVELIN_STATE In_JavelinState)
{
	m_eCurrentState = In_JavelinState;

	switch (m_eCurrentState)
	{
	case Client::CJavelinWeapon::JAVELIN_STATE::BIND_HAND:
		m_pCurrentModelCom = m_pStakeModelCom;
		break;
	case Client::CJavelinWeapon::JAVELIN_STATE::THROW:
		m_pCurrentModelCom = m_pModelCom;
		cout << Weak_Cast<CUrd>( m_pParentTransformCom.lock()->Get_Owner()).lock()->Get_CurState().lock()->Get_StateIndex() << endl;
		LookAt_Player();
		break;
	case Client::CJavelinWeapon::JAVELIN_STATE::STAKE:
		break;
	case Client::CJavelinWeapon::JAVELIN_STATE::STATE_END:
		break;
	default:
		break;
	}


}

HRESULT CJavelinWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CJavelinWeapon::Initialize(void* pArg)
{
	CGameObject::Initialize(pArg);

	m_pModelCom = Add_Component<CModel>();
	m_pShaderCom = Add_Component<CShader>();
	m_pRendererCom = Add_Component<CRenderer>();

	m_vOffset = { 0.f, 0.f, 0.f };

	m_pModelCom.lock()->Init_Model("Boss_UrdWeapon2", "", (_uint)TIMESCALE_LAYER::MONSTER);

	m_pStakeModelCom = Add_Component<CModel>();
	m_pStakeModelCom.lock()->Init_Model("Boss_UrdWeapon", "", (_uint)TIMESCALE_LAYER::MONSTER);

	Set_Enable(false);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxModel"),
		VTXMODEL_DECLARATION::Element,
		VTXMODEL_DECLARATION::iNumElements);

	return S_OK;
}

HRESULT CJavelinWeapon::Start()
{
	

	return S_OK;
}

void CJavelinWeapon::Tick(_float fTimeDelta)
{
	switch (m_eCurrentState)
	{
	case Client::CJavelinWeapon::JAVELIN_STATE::BIND_HAND:
		Update_Matrix_Hand();
		break;
	case Client::CJavelinWeapon::JAVELIN_STATE::THROW:
		Update_Matrix_Throw(fTimeDelta);
		break;
	case Client::CJavelinWeapon::JAVELIN_STATE::STAKE:
		Update_Matrix_Stake();
		break;
	case Client::CJavelinWeapon::JAVELIN_STATE::STATE_END:
		break;
	default:
		break;
	}
}

void CJavelinWeapon::LateTick(_float fTimeDelta)
{
	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONALPHABLEND, Cast<CGameObject>(m_this));
}

HRESULT CJavelinWeapon::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (!m_pCurrentModelCom.lock())
	{
		// Use Set_javelinState function.
		DEBUG_ASSERT;
	}

	SetUp_ShaderResource();

	__super::Render(pDeviceContext);

	_int iPassIndex;
	_flag BindTextureFlag;
	m_iNumMeshContainers = m_pCurrentModelCom.lock()->Get_NumMeshContainers();
	if (m_bWeaponRenderOnOff)
	{
		for (_uint i(0); i < m_iNumMeshContainers; ++i)
		{
			BindTextureFlag = 0;


			if (SUCCEEDED(m_pCurrentModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			{
				BindTextureFlag |= (1 << aiTextureType_DIFFUSE);
			}

			if (SUCCEEDED(m_pCurrentModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			{
				BindTextureFlag |= (1 << aiTextureType_NORMALS);
			}

			if (SUCCEEDED(m_pCurrentModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
			{
				BindTextureFlag |= (1 << aiTextureType_SPECULAR);
			}


			if (!((1 << aiTextureType_DIFFUSE) & BindTextureFlag))
			{
#ifdef _DEBUG
				cout << "DiffuseTexture is not binded. : " << m_pModelCom.lock()->Get_ModelKey() << endl;
#endif // _DEBUG
				continue;
			}

			// NormalTexture	OK.
			// ORMTexture		OK.
			if (
				(1 << aiTextureType_NORMALS) & BindTextureFlag &&
				(1 << aiTextureType_SPECULAR) & BindTextureFlag
				)
			{
				iPassIndex = 7;
			}

			// NormalTexture	OK.
			// ORMTexture		NO.
			else if (
				(1 << aiTextureType_NORMALS) & BindTextureFlag &&
				!((1 << aiTextureType_SPECULAR) & BindTextureFlag)
				)
			{
				iPassIndex = 3;
			}

			// NormalTexture	NO.
			// ORMTexture		NO.
			else if (
				!((1 << aiTextureType_NORMALS) & BindTextureFlag) &&
				!((1 << aiTextureType_SPECULAR) & BindTextureFlag)
				)
			{
				iPassIndex = 0;
			}

			else
			{
#ifdef _DEBUG
				cout << "The correct pass does not define. : " << m_pModelCom.lock()->Get_ModelKey() << endl;
#endif // _DEBUG
				continue;
			}

			m_pShaderCom.lock()->Begin(iPassIndex, pDeviceContext);
			m_pCurrentModelCom.lock()->Render_Mesh(i, pDeviceContext);
		}
	}
	
	return S_OK;
}

void CJavelinWeapon::Init_Weapon(weak_ptr<CModel> In_pModelCom, weak_ptr<CTransform> In_ParentTransformCom, const string& szTargetNode)
{
	m_pParentTransformCom = In_ParentTransformCom;
	m_pTargetBoneNode = In_pModelCom.lock()->Find_BoneNode(szTargetNode);
	m_TransformationMatrix = In_pModelCom.lock()->Get_TransformationMatrix();

	weak_ptr<CCharacter> pParentFromCharacter = Weak_Cast<CCharacter>(In_ParentTransformCom.lock()->Get_Owner());

	if (!pParentFromCharacter.lock())
	{
		// 부모 객체가 캐릭터가 아니거나 삭제된 객체임.
		DEBUG_ASSERT;
	}

	m_pParentCharacter = pParentFromCharacter;
}

void CJavelinWeapon::Weapon_BoneChange(weak_ptr<CModel> In_pModelCom,const string& szTargetNode)
{
	m_pTargetBoneNode = In_pModelCom.lock()->Find_BoneNode(szTargetNode);
}

void CJavelinWeapon::Enable_Weapon()
{
	for (auto& elem : m_pHitColliderComs)
	{
		elem.lock()->Set_Enable(true);
#ifdef _DEBUG_COUT_
		cout << "Enable Weapon!" << endl;
#endif
	}
}

void CJavelinWeapon::Add_Collider(_fvector In_vOffset, const _float In_fScale, const COLLISION_LAYER In_Layer)
{
	m_pHitColliderComs.push_back(Add_Component<CCollider>());

	COLLIDERDESC tDesc;
	tDesc.iLayer = (_uint)In_Layer;
	tDesc.vRotation = { 0.f, 0.f, 0.f, 0.f };
	tDesc.vScale = { In_fScale, 0.f, 0.f};
	tDesc.vTranslation = {0.f, 0.f, 0.f};
	XMStoreFloat3(&tDesc.vOffset, In_vOffset);

	m_pHitColliderComs.back().lock()->Init_Collider(COLLISION_TYPE::SPHERE, tDesc);
}

void CJavelinWeapon::Disable_Weapon()
{
	for (auto& elem : m_pHitColliderComs)
	{
		elem.lock()->Set_Enable(false);
#ifdef _DEBUG_COUT_
		cout << "Disable Weapon!" << endl;
#endif
		m_iHitColliderIndexs.clear();
	}
}

weak_ptr<CCharacter> CJavelinWeapon::Get_ParentCharacter()
{
	return m_pParentCharacter;
}

void CJavelinWeapon::Set_WeaponDesc(const WEAPON_DESC& In_WeaponDesc)
{
	m_tWeaponDesc = In_WeaponDesc;
}

void CJavelinWeapon::Set_WeaponDesc(const HIT_TYPE In_eHitType, const _float In_fDamage, const ATTACK_OPTION In_eOptionType)
{
	m_tWeaponDesc.iHitType = (_int)In_eHitType;
	m_tWeaponDesc.iOptionType = (_int)In_eOptionType;
	m_tWeaponDesc.fDamage = In_fDamage;
}

void CJavelinWeapon::Update_Matrix_Hand()
{
	_matrix		BoneMatrix = m_pTargetBoneNode.lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&m_TransformationMatrix);

	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	m_pTransformCom.lock()->Set_WorldMatrix(BoneMatrix * m_pParentTransformCom.lock()->Get_WorldMatrix());

}

void CJavelinWeapon::Update_Matrix_Throw(_float fTimeDelta)
{
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (XMVectorGetY(m_pTransformCom.lock()->Get_Position()) <= XMVectorGetY(pCurrentPlayer.lock()->Get_WorldPosition()) + 0.56f)
	{
		m_pForEffectTransform = Add_Component<CTransform>();
		m_pForEffectTransform.lock()->Set_Position(m_pTransformCom.lock()->Get_Position());

		GET_SINGLE(CGameManager)->Use_EffectGroup("Urd_Skill_Impact", m_pForEffectTransform, _uint(TIMESCALE_LAYER::MONSTER));
		Set_JavelinState(JAVELIN_STATE::STAKE);
		return;
	}
	
	m_pTransformCom.lock()->Go_Straight(fTimeDelta * 60.f);
	
}

void CJavelinWeapon::Update_Matrix_Stake()
{
}

void CJavelinWeapon::LookAt_Player()
{
	_vector vPos = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Transform()->Get_Position();
	m_pTransformCom.lock()->LookAt(vPos);

}

void CJavelinWeapon::SetUp_ShaderResource()
{
	CallBack_Bind_SRV(m_pShaderCom, "");
	m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

	_float fCamFar = GAMEINSTANCE->Get_CameraFar();
	m_pShaderCom.lock()->Set_RawValue("g_fFar", &fCamFar, sizeof(_float));

	_vector	vShaderFlag = { 0.f,0.f,0.f,0.f };
	m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector));

}

void CJavelinWeapon::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	_uint iOtherColliderIndex = pOtherCollider.lock()->Get_ColliderIndex();
	list<_uint>::iterator iter = find(m_iHitColliderIndexs.begin(), m_iHitColliderIndexs.end(), iOtherColliderIndex);

	//이미 충돌했던적이 있음.
	if (m_iHitColliderIndexs.end() != iter)
		return;

	m_iHitColliderIndexs.push_back(iOtherColliderIndex);

	Weak_Cast<CCharacter>(pOtherCollider.lock()->Get_Owner()).lock()->OnHit(pOtherCollider, pMyCollider, (HIT_TYPE)m_tWeaponDesc.iHitType, m_tWeaponDesc.fDamage);

	if (m_bFirstAttack)
	{
		if (m_pParentCharacter.lock())
		{
			m_pParentCharacter.lock()->Call_WeaponFirstAttack(pMyCollider, pOtherCollider);
		}

		m_bFirstAttack = false;
	}
	else
	{
		if (m_pParentCharacter.lock())
		{
			m_pParentCharacter.lock()->Call_WeaponAttack(pMyCollider, pOtherCollider);
		}
	}
}

void CJavelinWeapon::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
}

void CJavelinWeapon::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
}

void CJavelinWeapon::Init_Model(const string& strWeaponName, TIMESCALE_LAYER eLayer)
{
	m_pModelCom.lock()->Init_Model(strWeaponName.c_str(), "", (_uint)eLayer);
	m_iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
}

void CJavelinWeapon::Free()
{
}