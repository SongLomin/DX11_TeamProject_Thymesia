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

	m_pShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxModel"),
		VTXMODEL_DECLARATION::Element,
		VTXMODEL_DECLARATION::iNumElements
	);

	m_tLightDesc.eActorType = LIGHTDESC::TYPE::TYPE_POINT;
	m_tLightDesc.bEnable    = true;

	m_tLightDesc.vPosition  = { 0.f, 5.f, 0.f, 1.f };
	m_tLightDesc.vDiffuse   = { 1.f, 1.f, 1.f, 0.f };
	m_tLightDesc.vAmbient   = { 1.f, 1.f, 1.f, 0.f };
	m_tLightDesc.vSpecular  = { 1.f, 1.f, 1.f, 1.f };
	m_tLightDesc.vLightFlag = { 1.f, 1.f, 1.f, 1.f };
	m_tLightDesc.fRange     = 5.f;
	m_tLightDesc.fIntensity = 1.f;

	// m_pModelCom.lock()->Init_Model("Torch", "", (_uint)TIMESCALE_LAYER::NONE);
	// GET_SINGLE(CGameManager)->Use_EffectGroup("TorchFire", m_pTransformCom, _uint(TIMESCALE_LAYER::NONE));

	m_eRenderGroup = RENDERGROUP::RENDER_NONALPHABLEND;

	return S_OK;
}

HRESULT CLight_Prop::Start()
{
	m_tLightDesc = GAMEINSTANCE->Add_Light(m_tLightDesc);

	m_fTargetIntensity = m_tLightDesc.fIntensity;
	m_fTargetRange     = m_tLightDesc.fRange;

	return S_OK;
}

void CLight_Prop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_bool bCheckEnd = false;
	Callback_ActUpdate(fTimeDelta, bCheckEnd);

	if (bCheckEnd)
		Callback_ActUpdate.Clear();
}

void CLight_Prop::LateTick(_float fTimeDelta)
{
	if (m_bRendering && !m_pModelCom.lock()->Get_ModelData().lock().get())
		m_bRendering = false;

	__super::LateTick(fTimeDelta);
}

void CLight_Prop::Before_Render(_float fTimeDelta)
{
	if (!m_bRendering)
		return;

	__super::Before_Render(fTimeDelta);
}

HRESULT CLight_Prop::Render(ID3D11DeviceContext* pDeviceContext)
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

		m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
		m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

	CGameObject::Render(pDeviceContext);
	return S_OK;
}

void CLight_Prop::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);

	Out_Json["Light_Type"]    = (_int)m_tLightDesc.eActorType;
	Out_Json["Light_Range"]   = m_tLightDesc.fRange;
	Out_Json["SectionIndex"]  = m_iSectionIndex;
	Out_Json["DelayTime"]     = m_fDelayTime;
	Out_Json["IntensityTime"] = m_fIntensityTime;

	if ("" != m_szEffectTag)
		Out_Json["EffectTag"] = m_szEffectTag;
	
	Out_Json["Light_Intensity"] = m_fTargetIntensity;
	Out_Json["Light_Range"]     = m_fTargetRange;
	CJson_Utility::Write_Float3(Out_Json["Offset"]         , m_vOffset);
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
	m_iSectionIndex         = In_Json["SectionIndex"];
	m_fDelayTime            = In_Json["DelayTime"];

	if (In_Json.end() != In_Json.find("IntensityTime"))
		m_fIntensityTime = In_Json["IntensityTime"];

	if (In_Json.end() != In_Json.find("EffectTag"))
		m_szEffectTag = In_Json["EffectTag"];

	if (In_Json.end() != In_Json.find("Offset"))
		CJson_Utility::Load_Float3(In_Json["Offset"], m_vOffset);

	if (In_Json.end() != In_Json.find("Light_Intensity"))
		m_fTargetIntensity = In_Json["Light_Intensity"];

	if (In_Json.end() != In_Json.find("Light_Range"))
		m_fTargetIntensity = In_Json["Light_Range"];

	CJson_Utility::Load_Float4(In_Json["Light_Position"] , m_tLightDesc.vPosition);
	CJson_Utility::Load_Float4(In_Json["Light_Direction"], m_tLightDesc.vDirection);
	CJson_Utility::Load_Float4(In_Json["Light_Diffuse"]  , m_tLightDesc.vDiffuse);
	CJson_Utility::Load_Float4(In_Json["Light_Ambient"]  , m_tLightDesc.vAmbient);
	CJson_Utility::Load_Float4(In_Json["Light_Specular"] , m_tLightDesc.vSpecular);
	CJson_Utility::Load_Float4(In_Json["Light_Flag"]     , m_tLightDesc.vLightFlag);

	if (0 <= m_iSectionIndex)
	{
		m_tLightDesc.bEnable = false;
		GET_SINGLE(CGameManager)->Registration_SectionLight(m_iSectionIndex, Weak_Cast<CLight_Prop>(m_this));
	}
	else
	{
		if ("" != m_szEffectTag)
			GET_SINGLE(CGameManager)->Use_EffectGroup(m_szEffectTag, m_pTransformCom, _uint(TIMESCALE_LAYER::NONE));
	}
}

void CLight_Prop::OnEventMessage(_uint iArg)
{
	switch ((EVENT_TYPE)iArg)
	{
		case EVENT_TYPE::ON_EDITINIT:
		{
		}
		break;

		case EVENT_TYPE::ON_ENTER_SECTION:
		{
			if (!Callback_ActUpdate.empty())
				return;

			if ("" != m_szEffectTag && 0 <= m_iSectionIndex)
				GET_SINGLE(CGameManager)->Use_EffectGroup(m_szEffectTag, m_pTransformCom, _uint(TIMESCALE_LAYER::NONE));

			Callback_ActUpdate += bind(&CLight_Prop::Act_LightTurnOnEvent, this, placeholders::_1, placeholders::_2);
		}
		break;

		case EVENT_TYPE::ON_EXIT_SECTION:
		{
			if (!Callback_ActUpdate.empty())
				return;

			Callback_ActUpdate += bind(&CLight_Prop::Act_LightTurnOffEvent, this, placeholders::_1, placeholders::_2);
		}
		break;

		case EVENT_TYPE::ON_EDIT_UDATE:
		{
			XMStoreFloat4(&m_tLightDesc.vPosition, m_pTransformCom.lock()->Get_Position() + XMLoadFloat3(&m_vOffset));
			GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
		}
		break;

		case EVENT_TYPE::ON_EDITDRAW:
		{
			if (ImGui::CollapsingHeader("Light_Prop GameObject"))
			{
				const char* LightTypeItems[]  = { "Direction", "Point", "HalfPoint" };
				_int        iSelect_LightType = (_int)m_tLightDesc.eActorType;

				if (ImGui::Combo("Light Type", &iSelect_LightType, LightTypeItems, IM_ARRAYSIZE(LightTypeItems)))
				{
					m_tLightDesc.eActorType = (LIGHTDESC::TYPE)iSelect_LightType;
					GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
				}


				ImGui::InputInt("Section Index", &m_iSectionIndex);
				ImGui::InputFloat("DelayTime", &m_fDelayTime);
				ImGui::InputFloat("Intensity Time", &m_fIntensityTime);

				ImGui::Separator();

				switch (m_tLightDesc.eActorType)
				{
					case LIGHTDESC::TYPE_DIRECTIONAL:
					{
						ImGui::DragFloat4("Light_Direction", &m_tLightDesc.vDirection.x, 0.01f);
						ImGui::DragFloat4("Light_Diffuse"  , &m_tLightDesc.vDiffuse.x  , 0.01f);
						ImGui::DragFloat4("Light_Ambient"  , &m_tLightDesc.vAmbient.x  , 0.01f);
						ImGui::DragFloat4("Light_Specular" , &m_tLightDesc.vSpecular.x , 0.01f);
						ImGui::DragFloat ("Light_Intensity", &m_tLightDesc.fIntensity  , 0.01f);
					}
					break;

					case LIGHTDESC::TYPE_POINT:
					{
						_bool       bChangeData = false;

						ImGui::DragFloat3("Offset", &m_vOffset.x, 0.01f);

						ImGui::DragFloat4("Light_Position" , &m_tLightDesc.vPosition.x , 0.01f);
						ImGui::DragFloat4("Light_Diffuse"  , &m_tLightDesc.vDiffuse.x  , 0.01f);
						ImGui::DragFloat4("Light_Ambient"  , &m_tLightDesc.vAmbient.x  , 0.01f);
						ImGui::DragFloat4("Light_Specular" , &m_tLightDesc.vSpecular.x , 0.01f);

						bChangeData |= ImGui::DragFloat ("Light_Range"    , &m_tLightDesc.fRange      , 0.01f);
						bChangeData |= ImGui::DragFloat ("Light_Intensity", &m_tLightDesc.fIntensity  , 0.01f);

						if (bChangeData)
						{
							m_fTargetIntensity = m_tLightDesc.fIntensity;
							m_fTargetRange     = m_tLightDesc.fRange;
						}
					}
					break;
			
					case LIGHTDESC::TYPE_HALFPOINT:
					{
						ImGui::DragFloat4("Light_Position" , &m_tLightDesc.vPosition.x , 0.01f);
						ImGui::DragFloat4("Light_Direction", &m_tLightDesc.vDirection.x, 0.01f);
						ImGui::DragFloat4("Light_Diffuse"  , &m_tLightDesc.vDiffuse.x  , 0.01f);
						ImGui::DragFloat4("Light_Ambient"  , &m_tLightDesc.vAmbient.x  , 0.01f);
						ImGui::DragFloat4("Light_Specular" , &m_tLightDesc.vSpecular.x , 0.01f);
						ImGui::DragFloat ("Light_Range"    , &m_tLightDesc.fRange      , 0.01f);
						ImGui::DragFloat ("Light_Intensity", &m_tLightDesc.fIntensity  , 0.01f);
					}
					break;
				}
			}

			ImGui::Text("");
			ImGui::Separator();
		}
		break;
	}
}

void CLight_Prop::Act_LightTurnOnEvent(_float fTimeDelta, _bool& Out_End)
{
	m_fAccTime += fTimeDelta;

	if (m_tLightDesc.bEnable)
	{
		m_tLightDesc.fIntensity = (0.f < m_fIntensityTime) ? ((m_fAccTime / m_fIntensityTime) * m_fTargetIntensity) : (m_fTargetIntensity);
		m_tLightDesc.fRange     = (0.f < m_fIntensityTime) ? ((m_fAccTime / m_fIntensityTime) * m_fTargetRange)     : (m_fTargetRange);

		if (m_fIntensityTime <= m_fAccTime)
		{
			m_fAccTime              = 0.f;
			m_tLightDesc.fIntensity = m_fTargetIntensity;
			m_tLightDesc.fRange     = m_fTargetRange;
			Out_End                 = true;
		}

		GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
	}

	else if (m_fDelayTime <= m_fAccTime)
	{
		m_fAccTime              = 0.f;
		m_tLightDesc.fRange     = 0.f;
		m_tLightDesc.fIntensity = 0.f;
		m_tLightDesc.bEnable    = true;

		GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
	}
}

void CLight_Prop::Act_LightTurnOffEvent(_float fTimeDelta, _bool& Out_End)
{
	m_fAccTime += fTimeDelta;

	m_tLightDesc.fIntensity = (0.f < m_fIntensityTime) ? (fabs(1.f - (m_fAccTime / m_fIntensityTime)) * m_fTargetIntensity) : (0.f);
	m_tLightDesc.fRange     = (0.f < m_fIntensityTime) ? (fabs(1.f - (m_fAccTime / m_fIntensityTime)) * m_fTargetRange)     : (0.f);

	if (m_fIntensityTime <= m_fAccTime)
	{
		m_fAccTime              = 0.f;
		m_tLightDesc.fIntensity = 0.f;
		m_tLightDesc.fRange     = 0.f;
		m_tLightDesc.bEnable    = false;

		Out_End = true;
	}

	GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
}

void CLight_Prop::OnDestroy()
{
	GAMEINSTANCE->Remove_Light(m_tLightDesc.Get_LightIndex());
}

void CLight_Prop::Free()
{
	__super::Free();
}