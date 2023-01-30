#include "stdafx.h"
#include "Light_Prop.h"
#include "VIBuffer_Cube.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Model.h"
#include "PhysXCollider.h"
#include "Collider.h"
#include "Client_Presets.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "imgui.h"

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

#ifdef _DEBUG
	m_pColliderCom = Add_Component<CCollider>();
	m_pColliderCom.lock()->Set_DebugColor(XMVectorSet(1.f, 1.f, 0.f, 1.f));
#endif

	m_pShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxModel"),
		VTXMODEL_DECLARATION::Element,
		VTXMODEL_DECLARATION::iNumElements
	);

	m_tLightDesc.eActorType = LIGHTDESC::TYPE::TYPE_POINT;
	m_tLightDesc.bEnable    = true;

	m_tLightDesc.vPosition  = { 0.f, 5.f, 0.f, 1.f };
	m_tLightDesc.vDiffuse   = { 1.f, 1.f, 1.f, 1.f };
	m_tLightDesc.vAmbient   = { 1.f, 1.f, 1.f, 1.f };
	m_tLightDesc.vSpecular  = { 1.f, 1.f, 1.f, 1.f };
	m_tLightDesc.vLightFlag = { 1.f, 1.f, 1.f, 1.f };
	m_tLightDesc.fRange     = 5.f;
	m_tLightDesc.fIntensity = 1.f;

	m_eRenderGroup = RENDERGROUP::RENDER_NONALPHABLEND;
	
	return S_OK;
}

HRESULT CLight_Prop::Start()
{
	/*if (LIGHTDESC::TYPE::TYPE_POINT == m_tLightDesc.eActorType)
	{
		m_tLightDesc.fRange     = 15.f;
		m_tLightDesc.fIntensity = 1.f;
		m_tLightDesc.vDiffuse   = { 0.3f, 0.3f, 0.3f, 1.f };
		m_tLightDesc.vAmbient   = { 0.3f, 0.3f, 0.3f, 1.f };
		m_tLightDesc.vSpecular  = { 0.3f, 0.3f, 0.3f, 1.f };
	}*/
	
	//Set_Enable(false);

	m_fTargetIntensity      = m_tLightDesc.fIntensity;
	m_fTargetRange          = m_tLightDesc.fRange;

	m_tLightDesc = GAMEINSTANCE->Add_Light(m_tLightDesc);

	if (-1 == m_iSectionIndex && "" != m_szEffectTag)
		m_iEffectIndex = GET_SINGLE(CGameManager)->Use_EffectGroup(m_szEffectTag, m_pTransformCom, _uint(TIMESCALE_LAYER::NONE));

#ifdef _DEBUG
	if (LEVEL::LEVEL_EDIT == m_CreatedLevel)
	{
		SetUpColliderDesc();

		m_pColliderCom.lock()->Set_DebugColor
		(
			("" == m_pModelCom.lock()->Get_ModelKey()) ? (XMVectorSet(1.f, 1.f, 0.f, 1.f)) : (XMVectorSet(0.5f, 0.5f, 0.f, 1.f))
		);
	}
	else
	{
		m_pColliderCom.lock()->Set_Enable(false);
	}
#endif

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
	if (!m_pModelCom.lock()->Get_ModelData().lock().get())
		return;

	__super::LateTick(fTimeDelta);
}

void CLight_Prop::Before_Render(_float fTimeDelta)
{	
	if (!m_pModelCom.lock()->Get_ModelData().lock().get())
		return;

	__super::Before_Render(fTimeDelta);
}

HRESULT CLight_Prop::Render(ID3D11DeviceContext* pDeviceContext)
{

	if (FAILED(__super::SetUp_ShaderResource()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		_flag BindTextureFlag = 0;

		if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{
			BindTextureFlag |= (1 << aiTextureType_DIFFUSE);
		}

		if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			BindTextureFlag |= (1 << aiTextureType_NORMALS);
		}

		if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
		{
			BindTextureFlag |= (1 << aiTextureType_SPECULAR);
		}

		if (LEVEL::LEVEL_EDIT == m_CreatedLevel)
			m_iPassIndex = Preset::ShaderPass::ModelShaderPass(BindTextureFlag, false, false, false);
		else
			m_iPassIndex = Preset::ShaderPass::ModelShaderPass(BindTextureFlag, true, false, false);

		if ((_uint)-1 == m_iPassIndex)
		{
			continue;
		}

		m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
		m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

	CGameObject::Render(pDeviceContext);

	return S_OK;
}

void  CLight_Prop::Set_LightDesc(const LIGHTDESC& In_Desc)
{
	m_tLightDesc.vDiffuse   = In_Desc.vDiffuse;
	m_tLightDesc.vAmbient   = In_Desc.vAmbient;
	m_tLightDesc.vSpecular  = In_Desc.vSpecular;
	m_tLightDesc.fIntensity = In_Desc.fIntensity;

	if (LIGHTDESC::TYPE::TYPE_DIRECTIONAL == In_Desc.eActorType)
	{
		m_tLightDesc.vDirection = In_Desc.vDirection;
	}
	
	else if (LIGHTDESC::TYPE::TYPE_POINT == In_Desc.eActorType)
	{
		m_tLightDesc.fRange = In_Desc.fRange;
	}

	GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
}

void CLight_Prop::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);

	Out_Json["Light_Type"]    = (_int)m_tLightDesc.eActorType;
	Out_Json["Light_Range"]   = m_tLightDesc.fRange;
	Out_Json["SectionIndex"]  = m_iSectionIndex;
	Out_Json["DelayTime"]     = m_fDelayTime;
	Out_Json["IntensityTime"] = m_fIntensityTime;
	Out_Json["DisableTime"]   = m_fDisableTime;

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

	m_iSectionIndex         = In_Json["SectionIndex"];
	m_fDelayTime            = In_Json["DelayTime"];


	if (In_Json.end() != In_Json.find("EffectTag"))
		m_szEffectTag = In_Json["EffectTag"];

	if (In_Json.end() != In_Json.find("Offset"))
		CJson_Utility::Load_Float3(In_Json["Offset"], m_vOffset);

	if (In_Json.end() != In_Json.find("DisableTime"))
		m_fDisableTime = In_Json["DisableTime"];

	if (In_Json.end() != In_Json.find("IntensityTime"))
		m_fIntensityTime = In_Json["IntensityTime"];

	m_tLightDesc.eActorType = (LIGHTDESC::TYPE)iLightTypeFromInt;
	m_tLightDesc.fRange     = In_Json["Light_Range"];
	m_tLightDesc.fIntensity = In_Json["Light_Intensity"];
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

		if ("Env_Torch_Fire" == m_szEffectTag)
		{
			Callback_OnActivate += bind(&CLight_Prop::Call_Activate_Sound, this);
		}
	}

	_vector vPos = m_pTransformCom.lock()->Get_Position();

	if (1.f != XMVectorGetW(vPos))
	{
		m_pTransformCom.lock()->Set_Position(XMVectorSetW(vPos, 1.f));
	}

}

void CLight_Prop::OnEventMessage(_uint iArg)
{
	switch ((EVENT_TYPE)iArg)
	{
		case EVENT_TYPE::ON_ENTER_SECTION:
		{
			if (!Callback_ActUpdate.empty())
				return;

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

		case EVENT_TYPE::ON_RESET_OBJ:
		{
			m_tLightDesc.bEnable = false;

			GAMEINSTANCE->Set_LightDesc(m_tLightDesc);

			if ((!m_szEffectTag.empty()) && (0 <= m_iEffectIndex))
				GET_SINGLE(CGameManager)->UnUse_EffectGroup(m_szEffectTag, m_iEffectIndex);
		}
		break;

		case EVENT_TYPE::ON_EDIT_UDATE:
		{
			XMStoreFloat4(&m_tLightDesc.vPosition, m_pTransformCom.lock()->Get_Position() + XMLoadFloat3(&m_vOffset));
			GAMEINSTANCE->Set_LightDesc(m_tLightDesc);

#ifdef _DEBUG
			m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
#endif
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

				_char szEffectTag[MAX_PATH] = "";
				strcpy_s(szEffectTag, m_szEffectTag.c_str());

				if (ImGui::InputText("Effect", szEffectTag, MAX_PATH))
					m_szEffectTag = szEffectTag;

				ImGui::InputInt("Section Index"   , &m_iSectionIndex);
				ImGui::InputFloat("DelayTime"     , &m_fDelayTime);
				ImGui::InputFloat("Intensity Time", &m_fIntensityTime);
				ImGui::InputFloat("Disable Time"  , &m_fDisableTime);

				ImGui::Separator();

				switch (m_tLightDesc.eActorType)
				{
					case LIGHTDESC::TYPE_DIRECTIONAL:
					{
						ImGui::InputFloat4("Light_Direction", &m_tLightDesc.vDirection.x);
						ImGui::InputFloat4("Light_Diffuse"  , &m_tLightDesc.vDiffuse.x);
						ImGui::InputFloat4("Light_Ambient"  , &m_tLightDesc.vAmbient.x);
						ImGui::InputFloat4("Light_Specular" , &m_tLightDesc.vSpecular.x);

						ImGui::InputFloat("Light_Range"    , &m_tLightDesc.fRange);
						ImGui::InputFloat("Light_Intensity", &m_tLightDesc.fIntensity);
					}
					break;

					case LIGHTDESC::TYPE_POINT:
					{
						_bool       bChangeData = false;

						ImGui::InputFloat3("Offset", &m_vOffset.x);

						ImGui::InputFloat4("Light_Position" , &m_tLightDesc.vPosition.x);
						ImGui::InputFloat4("Light_Diffuse"  , &m_tLightDesc.vDiffuse.x);
						ImGui::InputFloat4("Light_Ambient"  , &m_tLightDesc.vAmbient.x);
						ImGui::InputFloat4("Light_Specular" , &m_tLightDesc.vSpecular.x);
						
						bChangeData |= ImGui::InputFloat ("Light_Range"    , &m_tLightDesc.fRange);
						bChangeData |= ImGui::InputFloat ("Light_Intensity", &m_tLightDesc.fIntensity);

						if (bChangeData)
						{
							m_fTargetIntensity = m_tLightDesc.fIntensity;
							m_fTargetRange     = m_tLightDesc.fRange;
						}
					}
					break;
			
					case LIGHTDESC::TYPE_SPOTLIGHT:
					{
						ImGui::DragFloat4("Light_Position" , &m_tLightDesc.vPosition.x , 0.01f);
						ImGui::DragFloat4("Light_Direction", &m_tLightDesc.vDirection.x, 0.01f);
						ImGui::DragFloat4("Light_Diffuse"  , &m_tLightDesc.vDiffuse.x  , 0.01f);
						ImGui::DragFloat4("Light_Ambient"  , &m_tLightDesc.vAmbient.x  , 0.01f);
						ImGui::DragFloat4("Light_Specular" , &m_tLightDesc.vSpecular.x , 0.01f);
						ImGui::DragFloat ("Light_Range"    , &m_tLightDesc.fRange      , 0.01f);
						ImGui::DragFloat ("Light_Intensity", &m_tLightDesc.fIntensity  , 0.01f);
						ImGui::DragFloat("Light_CutOff"    , &m_tLightDesc.fCutOff, 0.01f);
						ImGui::DragFloat("Light_OuterCutoff", &m_tLightDesc.fOuterCutOff, 0.01f);
					}
					break;
				}
			}

			ImGui::Text("");
			ImGui::Separator();
		}
		break;

		case EVENT_TYPE::ON_EDIT_RENDER_ACTIVATE:
		{
			m_tLightDesc.bEnable = true;

			GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
		}
		break;

		case EVENT_TYPE::ON_EDIT_RENDER_DISABLE:
		{
			m_tLightDesc.bEnable = false;

			GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
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

			Callback_OnActivate();
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

		if (!m_szEffectTag.empty())
			m_iEffectIndex = GET_SINGLE(CGameManager)->Use_EffectGroup(m_szEffectTag, m_pTransformCom, _uint(TIMESCALE_LAYER::NONE));
	}
}

void CLight_Prop::Act_LightTurnOffEvent(_float fTimeDelta, _bool& Out_End)
{
	m_fAccTime += fTimeDelta;

	m_tLightDesc.fIntensity = (0.f < m_fDisableTime) ? (fabs(1.f - (m_fAccTime / m_fDisableTime)) * m_fTargetIntensity) : (0.f);
	m_tLightDesc.fRange     = (0.f < m_fDisableTime) ? (fabs(1.f - (m_fAccTime / m_fDisableTime)) * m_fTargetRange)     : (0.f);

	if (m_fDisableTime <= m_fAccTime)
	{
		m_fAccTime              = 0.f;
		m_tLightDesc.fIntensity = 0.f;
		m_tLightDesc.fRange     = 0.f;
		m_tLightDesc.bEnable    = false;

		if ((!m_szEffectTag.empty()) && (0 <= m_iEffectIndex))
			GET_SINGLE(CGameManager)->UnUse_EffectGroup(m_szEffectTag, m_iEffectIndex);

		Out_End = true;
	}

	GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
}

void CLight_Prop::Call_Activate_Sound()
{
	GAMEINSTANCE->PlaySound2D("Spotlight.mp3", 1.f);
}

#ifdef _DEBUG
void CLight_Prop::SetUpColliderDesc()
{
	COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)COLLISION_LAYER::ONLY_VIEW;
    ColliderDesc.vScale       = _float3(2.f, 0.f, 0.f);
    ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}
#endif

void CLight_Prop::OnDestroy()
{
	__super::OnDestroy();

	GAMEINSTANCE->Remove_Light(m_tLightDesc.Get_LightIndex());

	if (0 <= m_iEffectIndex)
		GET_SINGLE(CGameManager)->UnUse_EffectGroup(m_szEffectTag, m_iEffectIndex);
	GET_SINGLE(CGameManager)->Remove_SectionLight(m_iSectionIndex, Weak_Cast<CGameObject>(m_this));
}

void CLight_Prop::Free()
{
	__super::Free();
}