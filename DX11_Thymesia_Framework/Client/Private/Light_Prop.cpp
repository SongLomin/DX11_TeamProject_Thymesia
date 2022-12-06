#include "stdafx.h"
#include "Light_Prop.h"
#include "VIBuffer_Cube.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Model.h"
#include "PhysXCollider.h"
#include "Client_Presets.h"

#include "GameInstance.h"
#include "GameManager.h"

GAMECLASS_C(CLight_Prop)
CLONE_C(CLight_Prop, CGameObject)

HRESULT CLight_Prop::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLight_Prop::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pModelCom.lock()->Init_Model("Torch", "", (_uint)TIMESCALE_LAYER::NONE);

	m_pShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxModel"),
		VTXMODEL_DECLARATION::Element,
		VTXMODEL_DECLARATION::iNumElements
	);

	GET_SINGLE(CGameManager)->Use_EffectGroup("TorchFire", m_pTransformCom, _uint(TIMESCALE_LAYER::NONE));

	m_eRenderGroup = RENDERGROUP::RENDER_NONALPHABLEND;

	ZeroMemory(&m_tLightDesc, sizeof(LIGHTDESC));
	m_tLightDesc.eActorType = LIGHTDESC::TYPE::TYPE_POINT;
	m_tLightDesc.bEnable    = true;

	XMStoreFloat4(&m_tLightDesc.vPosition, m_pTransformCom.lock()->Get_Position());
	m_tLightDesc.vDiffuse   = { 1.f ,  1.f,  0.8f, 0.f };
	m_tLightDesc.vAmbient   = { 0.6f, 0.6f, 0.48f, 0.f };
	m_tLightDesc.vSpecular  = { 0.7f, 0.7f, 0.58f, 1.f };
	m_tLightDesc.vLightFlag = {  1.f,  1.f,   1.f, 1.f };
	m_tLightDesc.fRange     = 5.f;

	m_tLightDesc = GAMEINSTANCE->Add_Light(m_tLightDesc);
	
	m_pPhysXColliderCom = Add_Component<CPhysXCollider>();
	m_pPhysXColliderCom.lock()->Init_ModelCollider(m_pModelCom.lock()->Get_ModelData(), true);

	PHYSXCOLLIDERDESC tPhysxColliderDesc;
	Preset::PhysXColliderDesc::TestLightPropSetting(tPhysxColliderDesc);
	m_pPhysXColliderCom.lock()->CreatePhysXActor(tPhysxColliderDesc);
	m_pPhysXColliderCom.lock()->Add_PhysXActorAtSceneWithOption({ 0.f, 0.f, 0.f }, 1.f);

	return S_OK;
}

HRESULT CLight_Prop::Start()
{
	return S_OK;
}

void CLight_Prop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

#ifdef _LIFEGUARD_FOR_FALL_
	_vector vPos = m_pTransformCom.lock()->Get_Position();
	if (vPos.m128_f32[1] < -50.f)
	{
		vPos.m128_f32[1] = 50.f;
		m_pTransformCom.lock()->Set_Position(vPos);
	}
#endif

}

void CLight_Prop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom, XMVectorSet(0.f, 0.13f, 0.f, 1.f));
}

void CLight_Prop::Before_Render(_float fTimeDelta)
{
	__super::Before_Render(fTimeDelta);
	
	m_pPhysXColliderCom.lock()->Synchronize_Transform(m_pTransformCom, XMVectorSet(0.f, -0.13f, 0.f, 1.f));
}

HRESULT CLight_Prop::Render()
{
	__super::SetUp_ShaderResource();

	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_HEIGHT);
	
		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
			m_iPassIndex = 3;
		else
			m_iPassIndex = 7;
	

		m_pShaderCom.lock()->Begin(m_iPassIndex);
		m_pModelCom.lock()->Render_Mesh(i);
	}

	CGameObject::Render();
	return S_OK;
}

void CLight_Prop::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);

	Out_Json.emplace("Light_Type" , (_int)m_tLightDesc.eActorType);
	Out_Json.emplace("Light_Range", m_tLightDesc.fRange);
	
	CJson_Utility::Write_Float4(Out_Json["Light_Position"] , m_tLightDesc.vPosition);
	CJson_Utility::Write_Float4(Out_Json["Light_Direction"], m_tLightDesc.vDirection);
	CJson_Utility::Write_Float4(Out_Json["Light_Diffuse"]  , m_tLightDesc.vDiffuse);
	CJson_Utility::Write_Float4(Out_Json["Light_Ambient"]  , m_tLightDesc.vAmbient);
	CJson_Utility::Write_Float4(Out_Json["Light_Specular"] , m_tLightDesc.vSpecular);
	CJson_Utility::Write_Float4(Out_Json["Light_Flag"]     , m_tLightDesc.vLightFlag);
}

void CLight_Prop::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);

	_int iLightTypeFromInt  = In_Json["Light_Type"];
	m_tLightDesc.eActorType = (LIGHTDESC::TYPE)iLightTypeFromInt;
	m_tLightDesc.fRange     = In_Json["Light_Range"];

	CJson_Utility::Load_Float4(In_Json["Light_Position"] , m_tLightDesc.vPosition);
	CJson_Utility::Load_Float4(In_Json["Light_Direction"], m_tLightDesc.vDirection);
	CJson_Utility::Load_Float4(In_Json["Light_Diffuse"]  , m_tLightDesc.vDiffuse);
	CJson_Utility::Load_Float4(In_Json["Light_Ambient"]  , m_tLightDesc.vAmbient);
	CJson_Utility::Load_Float4(In_Json["Light_Specular"] , m_tLightDesc.vSpecular);
	CJson_Utility::Load_Float4(In_Json["Light_Flag"]     , m_tLightDesc.vLightFlag);

	GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
}

void CLight_Prop::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
	{
		if (ImGui::CollapsingHeader("Light_Prop GameObject"))
		{
			const char* LightTypeItems[]  = { "Direction", "Point", "HalfPoint" };
			_int        iSelect_LightType = (_int)m_tLightDesc.eActorType;
			_bool       bChangeData       = false;

			if (ImGui::Combo("Light Type", &iSelect_LightType, LightTypeItems, IM_ARRAYSIZE(LightTypeItems)))
			{
				m_tLightDesc.eActorType = (LIGHTDESC::TYPE)iSelect_LightType;
				bChangeData = true;
			}

			switch (m_tLightDesc.eActorType)
			{
				case LIGHTDESC::TYPE_DIRECTIONAL:
				{
					bChangeData |= ImGui::DragFloat3("Light_Position" , &m_tLightDesc.vPosition.x , 0.01f);
					bChangeData |= ImGui::DragFloat3("Light_Direction", &m_tLightDesc.vDirection.x, 0.01f);
					bChangeData |= ImGui::DragFloat3("Light_Diffuse"  , &m_tLightDesc.vDiffuse.x  , 0.01f);
					bChangeData |= ImGui::DragFloat3("Light_Ambient"  , &m_tLightDesc.vAmbient.x  , 0.01f);
					bChangeData |= ImGui::DragFloat3("Light_Specular" , &m_tLightDesc.vSpecular.x , 0.01f);
					bChangeData |= ImGui::DragFloat4("Light_Flag"     , &m_tLightDesc.vLightFlag.x, 0.01f);
				}
				break;

				case LIGHTDESC::TYPE_POINT:
				{
					bChangeData |= ImGui::DragFloat3("Light_Position", &m_tLightDesc.vPosition.x , 0.01f);
					bChangeData |= ImGui::DragFloat3("Light_Diffuse" , &m_tLightDesc.vDiffuse.x  , 0.01f);
					bChangeData |= ImGui::DragFloat3("Light_Ambient" , &m_tLightDesc.vAmbient.x  , 0.01f);
					bChangeData |= ImGui::DragFloat3("Light_Specular", &m_tLightDesc.vSpecular.x , 0.01f);
					bChangeData |= ImGui::DragFloat("Light_Range"    , &m_tLightDesc.fRange      , 0.01f);
					bChangeData |= ImGui::DragFloat4("Light_Flag"    , &m_tLightDesc.vLightFlag.x, 0.01f);
				}
				break;
			
				case LIGHTDESC::TYPE_HALFPOINT:
				{
					bChangeData |= ImGui::DragFloat3("Light_Position" , &m_tLightDesc.vPosition.x , 0.01f);
					bChangeData |= ImGui::DragFloat3("Light_Direction", &m_tLightDesc.vDirection.x, 0.01f);
					bChangeData |= ImGui::DragFloat3("Light_Diffuse"  , &m_tLightDesc.vDiffuse.x  , 0.01f);
					bChangeData |= ImGui::DragFloat3("Light_Ambient"  , &m_tLightDesc.vAmbient.x  , 0.01f);
					bChangeData |= ImGui::DragFloat3("Light_Specular" , &m_tLightDesc.vSpecular.x , 0.01f);
					bChangeData |= ImGui::DragFloat("Light_Range"     , &m_tLightDesc.fRange      , 0.01f);
					bChangeData |= ImGui::DragFloat4("Light_Flag"     , &m_tLightDesc.vLightFlag.x, 0.01f);
				}
				break;
			}

			if (bChangeData)
				GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
		}
	}
}

void CLight_Prop::OnDestroy()
{
	GAMEINSTANCE->Remove_Light(m_tLightDesc.Get_LightIndex());
}

void CLight_Prop::Free()
{
	__super::Free();
}