#include "stdafx.h"
#include "InteriorProp.h"

#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "GameManager.h"
#include "Texture.h"
#include "VIBuffer_Ground.h"
#include "Collider.h"

#include "PhysXColliderObject_Interior.h"
#include "Static_InteriorProp.h"
#include "Interaction_CheckPoint.h"
#include "PreView_InteriorProp.h"
#include "Interaction_Chair.h"
#include "Interaction_MapTable.h"

#include "SMath.h"

GAMECLASS_C(CInteriorProp);
CLONE_C(CInteriorProp, CGameObject);

HRESULT CInteriorProp::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteriorProp::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom   = Add_Component<CShader>();
	m_pRendererCom = Add_Component<CRenderer>();
	m_pVIBufferCom = Add_Component<CVIBuffer_Ground>();

	m_pShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxGround"),
		VTXGROUND_DECLARATION::Element,
		VTXGROUND_DECLARATION::iNumElements
	);

	shared_ptr<MODEL_DATA> pModelData = GAMEINSTANCE->Get_ModelFromKey("Frame");
	m_pVIBufferCom.lock()->Init_Mesh(pModelData.get()->Mesh_Datas[0]);

	m_pPreviewProp = GAMEINSTANCE->Add_GameObject<CPreView_InteriorProp>(m_CreatedLevel);
	SetUp_PreviewPropMesh(m_ePropID);

    return S_OK;
}

HRESULT CInteriorProp::Start()
{
	Set_Enable(false);
	m_pPreviewProp.lock()->Set_Enable(false);

	string szBinFilePath = "../Bin/GroundInfo/Mesh_SubInfo/Frame.bin";

	ifstream is(szBinFilePath, ios::binary); 

	_int    iNumVertexX, iNumVertexZ;
	_float	fInterval;

	if (is.is_open())
	{
		read_typed_data(is, iNumVertexX);
		read_typed_data(is, iNumVertexZ);
		read_typed_data(is, fInterval);
	}

	_float3 vMyPos;
	XMStoreFloat3(&vMyPos, m_pTransformCom.lock()->Get_Position());

	m_vMaxRangeX = vMyPos.x + iNumVertexX * fInterval;
	m_vMaxRangeZ = vMyPos.z + iNumVertexZ * fInterval;

	GET_SINGLE(CGameManager)->Registration_Section(3000, Weak_Cast<CGameObject>(m_this));

    return S_OK;
}

void CInteriorProp::Tick(_float fTimeDelta)
{
	if (LEVEL::LEVEL_EDIT != m_CreatedLevel)
	{
		Select_Props();
		Edit_Props();
		Rotation_Prop();
	}

    __super::Tick(fTimeDelta);
}

void CInteriorProp::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONLIGHT, Weak_StaticCast<CGameObject>(m_this));
}

HRESULT CInteriorProp::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (FAILED(SetUp_ShaderResource()))
		DEBUG_ASSERT;

	m_pShaderCom.lock()->Begin(2, pDeviceContext);
	m_pVIBufferCom.lock()->Render(pDeviceContext);

	return __super::Render(pDeviceContext);
}

void CInteriorProp::OnEventMessage(_uint iArg)
{
	switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_ENTER_SECTION:
        {
			Set_Enable(true);
			m_pPreviewProp.lock()->Set_Enable(true);
			m_pPreviewProp.lock()->Get_Component<CCollider>().lock()->Set_Enable(true);
        }
        break;

        case EVENT_TYPE::ON_EXIT_SECTION:
        {
			Set_Enable(false);
			m_pPreviewProp.lock()->Set_Enable(false);
			m_pPreviewProp.lock()->Get_Component<CCollider>().lock()->Set_Enable(false);

			SaveJson_PropS();
        }
        break;
    }
}

void CInteriorProp::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);
}

void CInteriorProp::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);

	LoadJson_PropS();
}

HRESULT CInteriorProp::SetUp_ShaderResource()
{
	if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
		return E_FAIL;

	return S_OK;
}

void CInteriorProp::Select_Props()
{
	if (KEY_INPUT(KEY::RIGHT, KEY_STATE::TAP))
	{
		_int iNext = (_int)m_ePropID + 1;

		m_ePropID = (iNext >= (_int)PROP_ID::ID_END) ? ((PROP_ID)(0)) : ((PROP_ID)iNext);
		SetUp_PreviewPropMesh(m_ePropID);
		m_fRotationY = 0.f;
		m_pPreviewProp.lock()->Get_Transform().get()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.f);
	}
	
	else if (KEY_INPUT(KEY::LEFT, KEY_STATE::TAP))
	{
		_int iNext = (_int)m_ePropID - 1;

		m_ePropID = (iNext < 0) ? ((PROP_ID)((_int)PROP_ID::ID_END - 1)) : ((PROP_ID)iNext);
		SetUp_PreviewPropMesh(m_ePropID);
		m_fRotationY = 0.f;
		m_pPreviewProp.lock()->Get_Transform().get()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.f);
	}
}

void CInteriorProp::Rotation_Prop()
{
	if (KEY_INPUT(KEY::UP, KEY_STATE::HOLD))
	{
		m_fRotationY += 0.1f;
	}

	else if (KEY_INPUT(KEY::DOWN, KEY_STATE::HOLD))
	{
		m_fRotationY -= 0.1f;
	}

	RotationProp(m_pPreviewProp.lock()->Get_Transform());
}

void CInteriorProp::Edit_Props()
{
	weak_ptr<CGameObject> pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pPlayer.lock())
		return;

	_vector vCreatePos = pPlayer.lock()->Get_Transform().get()->Get_Position();
	_vector vOffset    = XMVector3Normalize(XMVectorSetY(GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2], 0.f)) * m_fOffsetLength;
	vCreatePos += vOffset;
	m_pPreviewProp.lock()->Set_Offset(vOffset);


	_bool IsInTerrin = Compute_IsInTerrain(vCreatePos);

	if (m_pPreviewProp.lock()->Get_CheckBuild() && IsInTerrin)
	{
		m_pPreviewProp.lock()->Set_Color(XMVectorSet(0.f, 1.f, 0.f, 1.f));

		if (!KEY_INPUT(KEY::Z, KEY_STATE::TAP))
			return;

		GAMEINSTANCE->PlaySound2D("EVM_Fantasy_Game_Item_Wooden_Chest_Open_or_Close.ogg", 1.f);
		Create_Prop(m_ePropID, m_pPreviewProp.lock()->Get_Transform()->Get_WorldMatrix());
	}
	else if (!m_pPreviewProp.lock()->Get_CheckBuild() && IsInTerrin)
	{
		m_pPreviewProp.lock()->Set_Color(XMVectorSet(1.f, 0.f, 0.f, 1.f));

		if (!KEY_INPUT(KEY::X, KEY_STATE::TAP))
			return;

		Delete_Prop(m_pPreviewProp.lock()->Get_CollisionObject());
	}
	else
	{
		m_pPreviewProp.lock()->Set_Color(XMVectorSet(1.f, 0.f, 0.f, 1.f));
	}

	if (!IsInTerrin)
	{
		GAMEINSTANCE->PlaySound2D("FootStep_MetalC-001.ogg", 1.f);
	}
}

void CInteriorProp::Create_Prop(PROP_ID _eItemID, _fmatrix _WorldMatrix)
{
	switch (_eItemID)
	{
		case PROP_ID::STATIC_FLOWERPOT:
		{
			weak_ptr<CGameObject> pObj = GAMEINSTANCE->Add_GameObject<CStatic_InteriorProp>(m_CreatedLevel);
			pObj.lock()->Get_Component<CModel>().lock()->Init_Model("SM_flowerpot_02");
			pObj.lock()->Get_Transform().get()->Set_WorldMatrix(_WorldMatrix);

			SaveJson_SingleProp(pObj, typeid(CStatic_InteriorProp).name(), typeid(CStatic_InteriorProp).hash_code());
		}
		break;

		case PROP_ID::STATIC_TABLE_01:
		{
			weak_ptr<CGameObject> pObj = GAMEINSTANCE->Add_GameObject<CStatic_InteriorProp>(m_CreatedLevel);
			pObj.lock()->Get_Component<CModel>().lock()->Init_Model("SM_table_01");
			pObj.lock()->Get_Transform().get()->Set_WorldMatrix(_WorldMatrix);

			SaveJson_SingleProp(pObj, typeid(CStatic_InteriorProp).name(), typeid(CStatic_InteriorProp).hash_code());
		}
		break;

		case PROP_ID::STATIC_STATIS_04:
		{
			weak_ptr<CGameObject> pObj = GAMEINSTANCE->Add_GameObject<CStatic_InteriorProp>(m_CreatedLevel);
			pObj.lock()->Get_Component<CModel>().lock()->Init_Model("SM_statue_04");
			pObj.lock()->Get_Transform().get()->Set_WorldMatrix(_WorldMatrix);

			SaveJson_SingleProp(pObj, typeid(CStatic_InteriorProp).name(), typeid(CStatic_InteriorProp).hash_code());
		}
		break;
		
		case PROP_ID::STATIC_BARREL_CLOSED:
		{
			weak_ptr<CGameObject> pObj = GAMEINSTANCE->Add_GameObject<CStatic_InteriorProp>(m_CreatedLevel);
			pObj.lock()->Get_Component<CModel>().lock()->Init_Model("SM_Barrel_Closed");
			pObj.lock()->Get_Transform().get()->Set_WorldMatrix(_WorldMatrix);

			SaveJson_SingleProp(pObj, typeid(CStatic_InteriorProp).name(), typeid(CStatic_InteriorProp).hash_code());
		}
		break;
		
		case PROP_ID::STATIC_BARREL_BIG:
		{
			weak_ptr<CGameObject> pObj = GAMEINSTANCE->Add_GameObject<CStatic_InteriorProp>(m_CreatedLevel);
			pObj.lock()->Get_Component<CModel>().lock()->Init_Model("SM_Barrel_Big");
			pObj.lock()->Get_Transform().get()->Set_WorldMatrix(_WorldMatrix);

			SaveJson_SingleProp(pObj, typeid(CStatic_InteriorProp).name(), typeid(CStatic_InteriorProp).hash_code());
		}
		break;

		case PROP_ID::STATIC_SHELF:
		{
			weak_ptr<CGameObject> pObj = GAMEINSTANCE->Add_GameObject<CStatic_InteriorProp>(m_CreatedLevel);
			pObj.lock()->Get_Component<CModel>().lock()->Init_Model("SM_Shelf");
			pObj.lock()->Get_Transform().get()->Set_WorldMatrix(_WorldMatrix);

			SaveJson_SingleProp(pObj, typeid(CStatic_InteriorProp).name(), typeid(CStatic_InteriorProp).hash_code());
		}
		break;

		case PROP_ID::INTERACTION_CHAIR:
		{
			weak_ptr<CGameObject> pObj = GAMEINSTANCE->Add_GameObject<CInteraction_Chair>(m_CreatedLevel);
			pObj.lock()->Get_Transform().get()->Set_WorldMatrix(_WorldMatrix);

			SaveJson_SingleProp(pObj, typeid(CInteraction_Chair).name(), typeid(CInteraction_Chair).hash_code());
		}
		break;
	}
}

void CInteriorProp::Delete_Prop(weak_ptr<CGameObject> _pCollisionObject)
{
	if (!_pCollisionObject.lock())
		return;
	
	_matrix DeleteObjWorldMatrix	= _pCollisionObject.lock()->Get_Transform()->Get_WorldMatrix();
	string  szModelKey				= _pCollisionObject.lock()->Get_Component<CModel>().lock()->Get_ModelKey();
	_pCollisionObject.lock()->Set_Dead();

	     if ("SM_flowerpot_02"     == szModelKey) m_ePropID = PROP_ID::STATIC_FLOWERPOT;
	else if ("SM_table_01"         == szModelKey) m_ePropID = PROP_ID::STATIC_TABLE_01;
	else if ("SM_statue_04"        == szModelKey) m_ePropID = PROP_ID::STATIC_STATIS_04;
	else if ("SM_Barrel_Closed"    == szModelKey) m_ePropID = PROP_ID::STATIC_BARREL_CLOSED;
	else if ("SM_Barrel_Big"       == szModelKey) m_ePropID = PROP_ID::STATIC_BARREL_BIG;
	else if ("P_ArchiveChair01"    == szModelKey) m_ePropID = PROP_ID::STATIC_SHELF;
	else if ("SM_ToolRackArranged" == szModelKey) m_ePropID = PROP_ID::INTERACTION_CHAIR;
	else 
		return;

	auto iter = m_PropSaveInfo.begin();
	while (m_PropSaveInfo.end() != iter)
	{
		if (iter->pObj.lock()->Get_Dead())
		{
			iter = m_PropSaveInfo.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	GAMEINSTANCE->PlaySound2D("FootStep_MetalC-001.ogg", 1.f);

	SetUp_PreviewPropMesh(m_ePropID);
	m_fRotationY = 0.f;
}

void CInteriorProp::SetUp_PreviewPropMesh(PROP_ID _eItemID)
{
	switch (_eItemID)
	{
		case PROP_ID::STATIC_FLOWERPOT:
		{
			m_pPreviewProp.lock()->Set_Model("SM_flowerpot_02");
			m_pPreviewProp.lock()->Get_Transform().get()->Set_Scaled(_float3(1.f, 1.f, 1.f));
			m_pPreviewProp.lock()->Get_Transform().get()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.f);
		}
		break;

		case PROP_ID::STATIC_TABLE_01:
		{
			m_pPreviewProp.lock()->Set_Model("SM_table_01");
			m_pPreviewProp.lock()->Get_Transform().get()->Set_Scaled(_float3(1.f, 1.f, 1.f));
			m_pPreviewProp.lock()->Get_Transform().get()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.f);
		}
		break;

		case PROP_ID::STATIC_STATIS_04:
		{
			m_pPreviewProp.lock()->Set_Model("SM_statue_04");
			m_pPreviewProp.lock()->Get_Transform().get()->Set_Scaled(_float3(1.f, 1.f, 1.f));
			m_pPreviewProp.lock()->Get_Transform().get()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.f);
		}
		break;
		
		case PROP_ID::STATIC_BARREL_CLOSED:
		{
			m_pPreviewProp.lock()->Set_Model("SM_Barrel_Closed");
			m_pPreviewProp.lock()->Get_Transform().get()->Set_Scaled(_float3(1.3f, 1.3f, 1.3f));
			m_pPreviewProp.lock()->Get_Transform().get()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.f);
		}
		break;
		
		case PROP_ID::STATIC_BARREL_BIG:
		{
			m_pPreviewProp.lock()->Set_Model("SM_Barrel_Big");
			m_pPreviewProp.lock()->Get_Transform().get()->Set_Scaled(_float3(1.5f, 1.5f, 1.5f));
			m_pPreviewProp.lock()->Get_Transform().get()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.f);
		}
		break;

		case PROP_ID::STATIC_SHELF:
		{
			m_pPreviewProp.lock()->Set_Model("SM_Shelf");
			m_pPreviewProp.lock()->Get_Transform().get()->Set_Scaled(_float3(1.5f, 1.5f, 1.5f));
			m_pPreviewProp.lock()->Get_Transform().get()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.f);
		}
		break;

		case PROP_ID::INTERACTION_CHAIR:
		{
			m_pPreviewProp.lock()->Set_Model("P_ArchiveChair01");
			m_pPreviewProp.lock()->Get_Transform().get()->Set_Scaled(_float3(1.f, 1.f, 1.f));
			m_pPreviewProp.lock()->Get_Transform().get()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.f);
		}
		break;
	}
}

_bool CInteriorProp::Compute_IsInTerrain(_fvector _vPos)
{
	_float3 vMyPos, vTargetPos;
	XMStoreFloat3(&vMyPos    , m_pTransformCom.lock()->Get_Position());
	XMStoreFloat3(&vTargetPos, _vPos);

	if ((vMyPos.x < vTargetPos.x && m_vMaxRangeX > vTargetPos.x) &&
		(vMyPos.z < vTargetPos.z && m_vMaxRangeZ > vTargetPos.z))
		return true;

	return false;
}

void CInteriorProp::LoadJson_PropS()
{
	if (LEVEL::LEVEL_EDIT == m_CreatedLevel)
		return;

	json LoadedJson;
	
	if (FAILED(CJson_Utility::Load_Json("../Bin/LevelData/HomeSave/HomeStagePropInfo.json", LoadedJson)))
		return;

	for (auto& Elem_GameObjects : LoadedJson["GameObject"])
	{
		weak_ptr<CGameObject> pGameObjectInstance = GAMEINSTANCE->Add_GameObject(Elem_GameObjects["Hash"], (_uint)m_CreatedLevel);

		if (pGameObjectInstance.lock())
		{
			pGameObjectInstance.lock()->Set_Enable(Elem_GameObjects["Setting"]["Enable"]);
			pGameObjectInstance.lock()->Load_FromJson(Elem_GameObjects);

			SAVE_PROP prop;
					
			prop.pObj = pGameObjectInstance;
			prop.json = Elem_GameObjects;

			m_PropSaveInfo.push_back(prop);
		}
	}
}

void CInteriorProp::SaveJson_PropS()
{
	json SaveJson;

	auto iter = m_PropSaveInfo.begin();
	for (_uint i = 0; i < (_uint)m_PropSaveInfo.size(); ++i)
	{
		SaveJson["GameObject"][i] = iter->json;
		iter++;
	}

	CJson_Utility::Save_Json("../Bin/LevelData/HomeSave/HomeStagePropInfo.json", SaveJson);
}

void CInteriorProp::SaveJson_SingleProp(weak_ptr<CGameObject> _pObj, string _szTypeName, _hashcode _hashcode)
{
	json SaveJson;
	SaveJson["Name"]				= _szTypeName;
	SaveJson["Hash"]				= _hashcode;
	SaveJson["Setting"]["Enable"]	= true;
	SaveJson["Component"]["Transform"].emplace();
	_pObj.lock()->Write_Json(SaveJson);

	SAVE_PROP prop;
	prop.pObj = _pObj;
	prop.json = SaveJson;

	m_PropSaveInfo.push_back(prop);
}

void CInteriorProp::RotationProp(weak_ptr<CTransform> _pTargetTransfomCom)
{
	weak_ptr<CGameObject> pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pPlayer.lock())
		return;

	_pTargetTransfomCom.lock()->LookAt2D(pPlayer.lock()->Get_Transform().get()->Get_Position());
	_pTargetTransfomCom.lock()->Rotation_Add(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fRotationY);
}

void CInteriorProp::OnDestroy()
{
	SaveJson_PropS();

	m_PropSaveInfo.clear();
}

void CInteriorProp::Free()
{
	__super::Free();
}
