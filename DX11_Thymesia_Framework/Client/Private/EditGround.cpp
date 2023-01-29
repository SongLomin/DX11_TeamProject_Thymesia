#include "stdafx.h"

#include "GameInstance.h"
#include "EditGround.h"
#include "SMath.h"

#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_Ground.h"

#include "Ground.h"

#include "imgui.h"

GAMECLASS_C(CEditGround)
CLONE_C(CEditGround, CGameObject)

#define D3DCOLOR_ABGR(a,r,g,b) \
    ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

#define FILTER_TEXTURE_SIZE 512

static const char* TexVarDesc[] =
{
	"g_Texture_Sorc",
	"g_Texture_AddNo1",
	"g_Texture_AddNo2",
	"g_Texture_AddNo3",
};

HRESULT CEditGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEditGround::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom = Add_Component<CShader>();
	m_pShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxGround"),
		VTXGROUND_DECLARATION::Element,
		VTXGROUND_DECLARATION::iNumElements
	);

	m_pRendererCom		 = Add_Component<CRenderer>();

	TEXTURES_INFO Desc;
	Desc.pDiffTex = Add_Component<CTexture>();
	Desc.pNormTex = Add_Component<CTexture>();
	Desc.pORMTex  = Add_Component<CTexture>();
	m_pTextureCom.emplace(TexVarDesc[0], Desc);

	Load_ResourceList(m_MeshNames   , "../Bin/GroundInfo/Mesh/", ".bin");
	Load_ResourceList(m_TextureNames, "../Bin/GroundInfo/Texture/");
	Load_ResourceList(m_FilterNames , "../Bin/GroundInfo/Filter_SubInfo/", ".bin");

	return S_OK;
}

HRESULT CEditGround::Start()
{
	return S_OK;
}

void CEditGround::Tick(_float fTimeDelta)
{
	if (!m_bCreate)
		return;
}

void CEditGround::LateTick(_float fTimeDelta)
{
	if (!m_bCreate)
		return;

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONALPHABLEND, Cast<CGameObject>(m_this));
}

HRESULT CEditGround::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (!m_bCreate)
		return S_OK;

	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom.lock()->Begin(m_iShaderPass, pDeviceContext);
	m_pVIBufferCom.lock()->Render(pDeviceContext);

	return S_OK;
}

HRESULT CEditGround::SetUp_ShaderResource()
{
	if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
		return E_FAIL;

	for (auto& iter : m_pTextureCom)
	{
		string szDiffTextureName = iter.first + "_Diff";
		string szNormTextureName = iter.first + "_Norm";
		string szORMTextureName  = iter.first + "_ORM";
		string szDensityName	 = "g_f" + iter.first.substr(string("g_Texture").length() + 1) + "_Density";
	
		if (iter.second.pDiffTex.lock() && "" != iter.second.pDiffTex.lock()->Get_TextureKey())
		{
			if (FAILED(iter.second.pDiffTex.lock()->Set_ShaderResourceView(m_pShaderCom, szDiffTextureName.c_str(), 0)))
				return E_FAIL;
		}

		if (iter.second.pNormTex.lock() && "" != iter.second.pNormTex.lock()->Get_TextureKey())
		{
			if (FAILED(iter.second.pNormTex.lock()->Set_ShaderResourceView(m_pShaderCom, szNormTextureName.c_str(), 0)))
				return E_FAIL;
		}

		if (iter.second.pORMTex.lock() && "" != iter.second.pORMTex.lock()->Get_TextureKey())
		{
			if (FAILED(iter.second.pORMTex.lock()->Set_ShaderResourceView(m_pShaderCom, szORMTextureName.c_str(), 0)))
				return E_FAIL;
		}

		if (FAILED(m_pShaderCom.lock()->Set_RawValue(szDensityName.c_str(), &iter.second.fDensity, sizeof(_float))))
			return E_FAIL;
	}

	if (m_pFilterTexture.Get())
	{
		if (FAILED(m_pShaderCom.lock()->Set_ShaderResourceView("g_FilterTexture", m_pFilterTexture)))
			return E_FAIL;
	}

	_vector vShaderFlag = { 0.f, 0.f, 0.f, 0.f };

	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
		return E_FAIL;

	_float fCamFar = GAMEINSTANCE->Get_CameraFar();
	m_pShaderCom.lock()->Set_RawValue("g_fFar", &fCamFar, sizeof(_float));

	return S_OK;
}

void CEditGround::Load_ResourceList(vector<string>& In_List, const filesystem::path& In_Path, string _szCutName)
{
	if (!In_Path.filename().extension().string().empty())
		return;

	fs::directory_iterator itr(In_Path);
	string szFileName;

	while (itr != fs::end(itr)) 
	{
		const fs::directory_entry& entry = *itr;

		szFileName = entry.path().filename().string();

		if ("" != _szCutName && string::npos != szFileName.find(_szCutName))
		{
			szFileName = szFileName.substr(0, szFileName.find(_szCutName));
		}

		In_List.push_back(szFileName);

		itr++;
	}
}

_bool CEditGround::Check_File(const string& In_Path)
{
	WIN32_FIND_DATAA data;
	HANDLE hFind = FindFirstFileA(In_Path.c_str(), &data);
	CloseHandle(hFind);

	return (data.cFileName);
}

void CEditGround::SetUp_EditMode()
{
	static const char* items_EditMode[] =
	{
		"HEIGHT_FLAT",
		"HEIGHT_LERP",
		"HEIGHT_SET",
		"FILLTER",
		"NON"
	};

	if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
	{
		if (KEY_INPUT(KEY::NUM1, KEY_STATE::HOLD))
			m_eEditMode = EDIT_MODE::HEIGHT_FLAT;
		else if (KEY_INPUT(KEY::NUM2, KEY_STATE::HOLD))
			m_eEditMode = EDIT_MODE::HEIGHT_LERP;
		else if (KEY_INPUT(KEY::NUM3, KEY_STATE::HOLD))
			m_eEditMode = EDIT_MODE::HEIGHT_SET;
		else if (KEY_INPUT(KEY::NUM4, KEY_STATE::HOLD))
			m_eEditMode = EDIT_MODE::FILLTER;
		else if (KEY_INPUT(KEY::NUM5, KEY_STATE::HOLD))
			m_eEditMode = EDIT_MODE::NON;
	}

	if (ImGui::BeginListBox("##Editer Type", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items_EditMode); n++)
		{
			const bool is_selected = ((_uint)m_eEditMode == n);
			if (ImGui::Selectable(items_EditMode[n], is_selected))
			{
				m_eEditMode = (EDIT_MODE)n;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
	}

	ImGui::Separator();
	ImGui::Text("");

	if (EDIT_MODE::FILLTER != m_eEditMode)
		return;

	static const char* items_BrushMode[] =
	{
		"AddTex_No1",
		"AddTex_No2",
		"AddTex_No3",
		"Clear"
	};

	if (KEY_INPUT(KEY::G, KEY_STATE::HOLD))
	{
		if (KEY_INPUT(KEY::NUM1, KEY_STATE::TAP))
			m_eBrushMode = BRUSH_MODE::BRUSH_NO1;

		if (KEY_INPUT(KEY::NUM2, KEY_STATE::TAP))
			m_eBrushMode = BRUSH_MODE::BRUSH_NO2;

		if (KEY_INPUT(KEY::NUM3, KEY_STATE::TAP))
			m_eBrushMode = BRUSH_MODE::BRUSH_NO3;

		if (KEY_INPUT(KEY::NUM4, KEY_STATE::TAP))
			m_eBrushMode = BRUSH_MODE::CLEAR;
	}

	if (ImGui::BeginListBox("##Brush Type", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items_BrushMode); n++)
		{
			const bool is_selected = ((_uint)m_eBrushMode == n);
			if (ImGui::Selectable(items_BrushMode[n], is_selected))
			{
				m_eBrushMode = (BRUSH_MODE)n;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	ImGui::Separator();
	ImGui::Text("");
}

void CEditGround::SetUp_Info()
{
	ImGui::Text("Size X, Z, Pitch");
	ImGui::DragFloat3("##Size", &m_vBufferInfo.x, 1.f);
	ImGui::Text("");

	if (ImGui::Button("Create"))
	{
		CreateBuffer();
	}

	ImGui::Separator();
}

void CEditGround::SetUp_ShaderComponent()
{
	ImGui::DragInt("##Pass", &m_iShaderPass, 1.f);
	ImGui::SameLine();
	ImGui::Text("Shader Pass");

	ImGui::Separator();
}

void CEditGround::SetUp_PinckingInfo()
{
	if (KEY_INPUT(KEY::RIGHT, KEY_STATE::TAP))
		m_fBufferDrawRadious += KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) ? (1.f) : (0.1f);
	else if (KEY_INPUT(KEY::LEFT, KEY_STATE::TAP))
		m_fBufferDrawRadious -= KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) ? (1.f) : (0.1f);

	if (KEY_INPUT(KEY::UP, KEY_STATE::TAP))
		m_fBufferPower += KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) ? (0.5f) : (0.1f);
	else if (KEY_INPUT(KEY::DOWN, KEY_STATE::TAP))
		m_fBufferPower -= KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD) ? (0.5f) : (0.1f);

	ImGui::DragFloat("Radious", &m_fBufferDrawRadious, 1.f);
	ImGui::DragFloat("Power"  , &m_fBufferPower, 1.f);

	ImGui::Separator();
}

void CEditGround::SetUp_Textures()
{
	static int iSelect_TexDesc = 0;

	ImGui::Combo("Tex Var", &iSelect_TexDesc, TexVarDesc, IM_ARRAYSIZE(TexVarDesc));

	if (ImGui::Button("Add"))
	{
		if (m_pTextureCom.end() == m_pTextureCom.find(TexVarDesc[iSelect_TexDesc]))
		{
			TEXTURES_INFO	Desc;
			Desc.pDiffTex = Add_Component<CTexture>();
			Desc.pNormTex = Add_Component<CTexture>();

			m_pTextureCom.emplace(TexVarDesc[iSelect_TexDesc], Desc);
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Del"))
	{
		auto iter_fine = m_pTextureCom.find(TexVarDesc[iSelect_TexDesc]);

		if (iter_fine != m_pTextureCom.end())
		{
			m_pTextureCom.erase(iter_fine);
		}
	}

	ImGui::Text("");
	ImGui::Separator();

	static int			iSelect_Texture_Com			= 0;
	static string		szSelect_Texture_Com_Tag	= "";

	ImGui::Text(string("Select Com : " + szSelect_Texture_Com_Tag).c_str());

	auto iter_find = m_pTextureCom.find(szSelect_Texture_Com_Tag);

	if (m_pTextureCom.end() != iter_find)
	{
		ImGui::Text("");
		ImGui::Text(string("[ Diff ] : " + iter_find->second.pDiffTex.lock()->Get_TextureKey()).c_str());
		ImGui::Text(string("[ Norm ] : " + iter_find->second.pNormTex.lock()->Get_TextureKey()).c_str());

		ImGui::InputFloat("Density", &iter_find->second.fDensity);
		ImGui::Text("");
	}

	if (ImGui::BeginListBox("##TexCom List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		_uint iIndex = 0;

		for (auto& iter : m_pTextureCom)
		{
			const bool is_selected = (iSelect_Texture_Com == iIndex);

			if (ImGui::Selectable(iter.first.c_str(), is_selected))
			{
				iSelect_Texture_Com			= iIndex;
				szSelect_Texture_Com_Tag	= iter.first;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();

			++iIndex;
		}

		ImGui::EndListBox();
	}

	ImGui::Text("");
	ImGui::Separator();

	static const char* TexTypeInfo[] =
	{
		"Diff_and_Norm",
		"Diff",
		"Norm"
	};

	static int		iSelect_TexType				= 0;
	static int		iSelect_Texture_Desc		= 0;
	static string   szClickTexureTag			= "";
	static _char    szFindTextureTag[MAX_PATH]	= "";

	if (!m_TextureNames.empty())
		ImGui::Text(string("Select Tex : " + m_TextureNames[iSelect_Texture_Desc]).c_str());

	ImGui::InputText("Find", szFindTextureTag, MAX_PATH);

	if (ImGui::BeginListBox("##Tex List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (_uint i = 0; i < m_TextureNames.size(); ++i)
		{
			const bool is_selected = (iSelect_Texture_Desc == i);

			if (0 < strlen(szFindTextureTag))
			{
				if (string::npos == m_TextureNames[i].find(szFindTextureTag))
					continue;
			}

			string szFind = m_TextureNames[i].substr(m_TextureNames[i].length() - 5, m_TextureNames[i].length());

			if (0 == iSelect_TexType || 1 == iSelect_TexType)
			{
				if ("D.png" != szFind)
					continue;
			}
			else if (1 == iSelect_TexType)
			{
				if ("N.png" != szFind)
					continue;
			}

			if (ImGui::Selectable(m_TextureNames[i].c_str(), is_selected))
			{
				iSelect_Texture_Desc = i;
				szClickTexureTag     = m_TextureNames[iSelect_Texture_Desc];
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
	}

	ImGui::Combo("Tex Info", &iSelect_TexType, TexTypeInfo, 2);

	if (ImGui::Button("Edit Texture"))
	{
		switch (iSelect_TexType)
		{
			case 0 :
			{
				auto iter_find = m_pTextureCom.find(szSelect_Texture_Com_Tag);

				if (m_pTextureCom.end() != iter_find)
				{
					string szDiffTex = szClickTexureTag.substr(0, szClickTexureTag.length() - 5) + "D";
					string szNormTex = szClickTexureTag.substr(0, szClickTexureTag.length() - 5) + "N";

					iter_find->second.pDiffTex.lock()->Use_Texture(szDiffTex.c_str());
					iter_find->second.pNormTex.lock()->Use_Texture(szNormTex.c_str());
				}
			}
			break;

			case 1:
			{
				auto iter_find = m_pTextureCom.find(szSelect_Texture_Com_Tag);

				if (m_pTextureCom.end() != iter_find)
				{
					string szDiffTex = szClickTexureTag.substr(0, szClickTexureTag.length() - 5) + "D";

					iter_find->second.pDiffTex.lock()->Use_Texture(szDiffTex.c_str());
				}
			}
			break;

			case 2:
			{
				auto iter_find = m_pTextureCom.find(szSelect_Texture_Com_Tag);

				if (m_pTextureCom.end() != iter_find)
				{
					string szDiffTex = szClickTexureTag.substr(0, szClickTexureTag.length() - 5) + "N";

					iter_find->second.pDiffTex.lock()->Use_Texture(szDiffTex.c_str());
				}
			}
			break;
		}
	}
}

void CEditGround::SetUp_File()
{
	_char szMeshName[64];
	strcpy_s(szMeshName, m_szMeshName.c_str());
	
	ImGui::Text("[ Mesh ]");
	if (ImGui::InputText("MeshName", szMeshName, 64))
	{
		m_szMeshName = szMeshName;
	}

	static int	iSelect_MeshData = 0;

	if (ImGui::BeginListBox("##TexCom List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (_uint i = 0; i < m_MeshNames.size(); ++i)
		{
			const bool is_selected = (iSelect_MeshData == i);

			if ("" != m_szMeshName)
			{
				if (string::npos == m_MeshNames[i].find(m_szMeshName))
					continue;
			}

			if (ImGui::Selectable(m_MeshNames[i].c_str(), is_selected))
			{
				iSelect_MeshData	= i;
				m_szMeshName		= m_MeshNames[i];
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
	}

	if (ImGui::Button("Save", ImVec2(100.f, 25.f)))
	{
		Bake_Mesh();
		MSG_BOX("Save Done : Bake_Mesh()");
	}

	ImGui::SameLine();

	if (ImGui::Button("Load", ImVec2(100.f, 25.f)))
	{
		Load_Mesh();
	}

	ImGui::Separator();
	ImGui::Text("");

	ImGui::Text("[ Filter Texture ]");

	static int	iSelect_FilterData		= 0;

	_char szFilterName[64];
	strcpy_s(szFilterName, m_szSaveTextureTag.c_str());

	if (ImGui::InputText("Filter Name", szFilterName, MAX_PATH))
		m_szSaveTextureTag = szFilterName;

	if (ImGui::BeginListBox("##Filter List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		_uint iIndex = 0;

		for (auto& iter : m_FilterNames)
		{
			const bool is_selected = (iSelect_FilterData == iIndex);

			if ("" != string(szFilterName))
			{
				if (string::npos == m_FilterNames[iIndex].find(szFilterName))
				{
					++iIndex;
					continue;
				}
			}

			if (ImGui::Selectable(iter.c_str(), is_selected))
			{
				iSelect_FilterData = iIndex;
				m_szSaveTextureTag = iter;
				strcpy_s(szFilterName, m_szSaveTextureTag.c_str());
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();

			++iIndex;
		}

		ImGui::EndListBox();
	}


	if (ImGui::Button("Save_Fltr", ImVec2(100.f, 25.f)))
	{
		Bake_FilterTexture();
		MSG_BOX("Save Done : Bake_FilterTexture()");
	}

	ImGui::SameLine();

	if (ImGui::Button("Load_Fltr", ImVec2(100.f, 25.f)))
	{
		if (0 < (_int)strlen(szFilterName))
		{
			Load_FilterTexture();
		}
	}

	ImGui::Separator();
}

void CEditGround::PickingFillterTextureDraw()
{
	if (!m_pVIBufferCom.lock().get())
		return;

	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::HOLD) || !KEY_INPUT(KEY::Z, KEY_STATE::HOLD))
		return;

	RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

	_float2		Out;

	if (!m_pVIBufferCom.lock()->Compute_MouseRatio
	(
		MouseRayInWorldSpace,
		m_pTransformCom.lock()->Get_WorldMatrix(),
		&Out
	))
	{
		return;
	}
	
	D3D11_MAPPED_SUBRESOURCE		SubResource;
	DEVICECONTEXT->Map(m_pTexture2D.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	_int2	iPickIndex = { _int(Out.x * FILTER_TEXTURE_SIZE), _int(Out.y * FILTER_TEXTURE_SIZE) };
	_int	iRoundIndx = _int(m_fBufferDrawRadious / m_vBufferInfo.z);

	_int2	iBeginIndex, iEndIndex;
	iBeginIndex.x = (0 > iPickIndex.x - iRoundIndx) ? (0) : (iPickIndex.x - iRoundIndx);
	iBeginIndex.y = (0 > iPickIndex.y - iRoundIndx) ? (0) : (iPickIndex.y - iRoundIndx);

	iEndIndex.x = (_int(FILTER_TEXTURE_SIZE) < iPickIndex.x + iRoundIndx) ? (FILTER_TEXTURE_SIZE) : (iPickIndex.x + iRoundIndx);
	iEndIndex.y = (_int(FILTER_TEXTURE_SIZE) < iPickIndex.y + iRoundIndx) ? (FILTER_TEXTURE_SIZE) : (iPickIndex.y + iRoundIndx);

	for (_uint iZ(iBeginIndex.y); iZ < (_uint)iEndIndex.y; ++iZ)
	{
		for (_uint iX(iBeginIndex.x); iX < (_uint)iEndIndex.x; ++iX)
		{
			_ulong	iIndex		= iZ * FILTER_TEXTURE_SIZE + iX;
			
			_float2 vMousePos   = _float2((_float)iPickIndex.x, (_float)iPickIndex.y);
			_float2 vPixelPos	= _float2((_float)iX, (_float)iZ);
			_float	fLength		= XMVectorGetX((XMVector2Length(XMLoadFloat2(&vPixelPos) - XMLoadFloat2(&vMousePos))));

			if ((_float)iRoundIndx < fLength)
				continue;

			switch (m_eBrushMode)
			{
				case BRUSH_MODE::BRUSH_NO1:
				{
					m_vColors[iIndex] = D3DCOLOR_ABGR(255, 0, 0, 255);
				}
				break;

				case BRUSH_MODE::BRUSH_NO2:
				{
					m_vColors[iIndex] = D3DCOLOR_ABGR(255, 0, 255, 0);
				}
				break;

				case BRUSH_MODE::BRUSH_NO3:
				{
					m_vColors[iIndex] = D3DCOLOR_ABGR(255, 255, 0, 0);
				}
				break;

				case BRUSH_MODE::CLEAR:
				{
					m_vColors[iIndex] = D3DCOLOR_ABGR(0, 0, 0, 0);
				}
				break;
			}
		}
	}

	for (_uint iZ = 0; iZ < FILTER_TEXTURE_SIZE; ++iZ)
	{
		for (_uint iX = 0; iX < FILTER_TEXTURE_SIZE; ++iX)
		{
			_ulong	iIndex = iZ * FILTER_TEXTURE_SIZE + iX;

			((_uint*)SubResource.pData)[iIndex] = m_vColors[iIndex];
		}
	}

	DEVICECONTEXT->Unmap(m_pTexture2D.Get(), 0);
}

void CEditGround::CreateBuffer()
{
	Remove_Components<CVIBuffer_Ground>();

	m_pVIBufferCom = Add_Component<CVIBuffer_Ground>();
	m_pVIBufferCom.lock()->Init_Mesh(m_vBufferInfo);

	CreateFilterTexture();

	m_bCreate = true;
}

void CEditGround::CreateFilterTexture()
{
	m_vColors.clear();

	if (!m_pTexture2D.Get())
	{ 
		D3D11_TEXTURE2D_DESC	TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		TextureDesc.Width				= FILTER_TEXTURE_SIZE;
		TextureDesc.Height				= FILTER_TEXTURE_SIZE;
		TextureDesc.MipLevels			= 1;
		TextureDesc.ArraySize			= 1;
		TextureDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;

		TextureDesc.SampleDesc.Quality	= 0;
		TextureDesc.SampleDesc.Count	= 1;

		TextureDesc.Usage				= D3D11_USAGE_DYNAMIC;
		TextureDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
		TextureDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		TextureDesc.MiscFlags			= 0;

		if (FAILED(DEVICE->CreateTexture2D(&TextureDesc, nullptr, m_pTexture2D.GetAddressOf())))
			return;
	}

	D3D11_MAPPED_SUBRESOURCE		SubResource;
	DEVICECONTEXT->Map(m_pTexture2D.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	for (_uint i = 0; i < FILTER_TEXTURE_SIZE; ++i)
	{
		for (_uint s = 0; s < FILTER_TEXTURE_SIZE; ++s)
		{
			_uint iIndex = i * FILTER_TEXTURE_SIZE + s;

			((_uint*)SubResource.pData)[iIndex] = D3DCOLOR_ABGR(0, 0, 0, 0);
			m_vColors.push_back(D3DCOLOR_ABGR(0, 0, 0, 0));
		}
	}

	DEVICECONTEXT->Unmap(m_pTexture2D.Get(), 0);

	if (!m_pFilterTexture.Get())
	{
		if (FAILED(DEVICE->CreateShaderResourceView(m_pTexture2D.Get(), nullptr, m_pFilterTexture.GetAddressOf())))
			return;
	}
}

void CEditGround::PickingGround()
{
	if (!m_pVIBufferCom.lock().get())
		return;

	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
		return;

	_bool bInputZ = KEY_INPUT(KEY::Z, KEY_STATE::HOLD);
	_bool bInputX = KEY_INPUT(KEY::X, KEY_STATE::HOLD);

	if (!bInputZ && !bInputX)
		return;

	RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

	if (m_pVIBufferCom.lock())
	{
		_float3		Out = _float3(0.f, 0.f, 0.f);

		if (m_pVIBufferCom.lock()->Compute_MousePos
		(
			MouseRayInWorldSpace,
			m_pTransformCom.lock()->Get_WorldMatrix(),
			&Out
		))
		{
			if (bInputZ)
			{
				m_pVIBufferCom.lock()->Update
				(
					XMLoadFloat3(&Out),
					m_fBufferDrawRadious,
					m_fBufferPower,
					(_uint)m_eEditMode
				);
			}
			else if (bInputX)
			{
				m_pVIBufferCom.lock()->Update
				(
					XMLoadFloat3(&Out),
					m_fBufferDrawRadious,
					m_fBufferPower * -1.f,
					(_uint)m_eEditMode
				);
			}
		}
	}
}

void CEditGround::Bake_Mesh()
{
	if ("" == m_szMeshName)
		return;

	/* --- Mesh Data --- */

	MODEL_DATA tModelData;

	tModelData.eModelType		= MODEL_TYPE::GROUND;
	tModelData.iNumAnimations	= 0;
	tModelData.iNumMaterials	= 0;
	tModelData.iNumMeshs		= 1;
	tModelData.szModelFileName	= m_szMeshName;
	tModelData.szModelFilePath	= "../Bin/GroundInfo/Mesh/" + m_szMeshName + ".bin";
	XMStoreFloat4x4(&tModelData.TransformMatrix, m_pTransformCom.lock()->Get_WorldMatrix());

	tModelData.RootNode = make_shared<NODE_DATA>();
	tModelData.RootNode->iNumChildren = 0;
	XMStoreFloat4x4(&tModelData.RootNode->TransformationMatrix, XMMatrixIdentity());

	_float fVtxX = m_pVIBufferCom.lock()->Get_NumVerticesX();
	_float fVtxZ = m_pVIBufferCom.lock()->Get_NumVerticesZ();

	shared_ptr<MESH_DATA> pMeshData = make_shared<MESH_DATA>();
	pMeshData->eModelType		= MODEL_TYPE::GROUND;
	pMeshData->iMaterialIndex	= 0;
	pMeshData->iNumBones		= 0;
	pMeshData->iNumFaces		= _uint(fVtxX - 1) * _uint(fVtxZ - 1) * 2;
	pMeshData->iNumVertices		= _uint(fVtxX * fVtxZ);

	pMeshData->pGroundVertices = shared_ptr<VTXGROUND[]>(DBG_NEW VTXGROUND[pMeshData->iNumVertices]);

	for (_uint i(0); i < pMeshData->iNumVertices; ++i)
	{
		VTXGROUND vOut;

		if (m_pVIBufferCom.lock()->Get_Vertex(i, &vOut))
			memcpy(&pMeshData->pGroundVertices[i], &vOut, sizeof(VTXGROUND));
		else
			DEBUG_ASSERT;
	}

	pMeshData->pIndices = shared_ptr<FACEINDICES32[]>(DBG_NEW FACEINDICES32[pMeshData->iNumFaces]);

	for (_uint i(0); i < pMeshData->iNumFaces; ++i)
	{
		_uint3 Out = { 0, 0, 0 };

		if (m_pVIBufferCom.lock()->Get_Indices(i, &Out))
			memcpy(&pMeshData->pIndices[i], &Out, sizeof(_uint3));
		else
			DEBUG_ASSERT;
	}


	tModelData.Mesh_Datas.push_back(pMeshData);
	tModelData.Bake_Binary();

	if (Load_AddMeshInfo(m_szMeshName))
		m_MeshNames.push_back(m_szMeshName);

	/* --- Buffer Data --- */
	
	string szBinFilePath = "../Bin/GroundInfo/Mesh_SubInfo/" + m_szMeshName + ".bin";

	ofstream os(szBinFilePath, ios::binary);

	if (!os.is_open())
		return;

	write_typed_data(os, m_pVIBufferCom.lock()->Get_NumVerticesX());
	write_typed_data(os, m_pVIBufferCom.lock()->Get_NumVerticesZ());
	write_typed_data(os, m_pVIBufferCom.lock()->Get_Interval());

	os.close();
}

void CEditGround::Load_Mesh()
{
	/* --- Mesh Data --- */

	shared_ptr<MODEL_DATA> pModelData = GAMEINSTANCE->Get_ModelFromKey(m_szMeshName.c_str());

	if (!pModelData.get() || !pModelData.get()->Mesh_Datas[0].get())
		return;

	/* --- Buffer Data --- */

	string szBinFilePath = "../Bin/GroundInfo/Mesh_SubInfo/" + m_szMeshName + ".bin";

	ifstream is(szBinFilePath, ios::binary);

	if (!is.is_open())
		return;

	_int	iNumVertexX, iNumVertexZ;
	_float	fInterval;

	read_typed_data(is, iNumVertexX);
	read_typed_data(is, iNumVertexZ);
	read_typed_data(is, fInterval);

	/* --- Create Buffer --- */

	Remove_Components<CVIBuffer_Ground>();
	m_pVIBufferCom = Add_Component<CVIBuffer_Ground>();
	m_pVIBufferCom.lock()->Init_Mesh(pModelData.get()->Mesh_Datas[0], iNumVertexX, iNumVertexZ, fInterval);

	m_vBufferInfo.x = _float(iNumVertexX);
	m_vBufferInfo.y = _float(iNumVertexZ);
	m_vBufferInfo.z = fInterval;

	m_bCreate   = true;
}

void CEditGround::Bake_FilterTexture()
{
	_tchar szFullTag[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, m_szSaveTextureTag.c_str(), (_int)m_szSaveTextureTag.length(), szFullTag, MAX_PATH);

	wstring szTexturePath = TEXT("../Bin/GroundInfo/Filter/") + wstring(szFullTag) + TEXT(".dds");

	SaveDDSTextureToFile(DEVICECONTEXT, m_pTexture2D.Get(), szTexturePath.c_str());
	
	string szBinFilePath = "../Bin/GroundInfo/Filter_SubInfo/" + m_szSaveTextureTag + ".bin";

	ofstream os(szBinFilePath, ios::binary);

	if (!os.is_open())
		return;

	_uint iSize = (_uint)m_vColors.size();
	write_typed_data(os, iSize);

	for (auto& iter : m_vColors)
		write_typed_data(os, iter);

	os.close();

	if (m_pTextureCom.empty())
		return;

	string szTextureInfoPath = "../Bin/GroundInfo/Filter_SubTextureInfo/" + m_szSaveTextureTag + ".json";

	json TexInfo;

	for (auto& iter : m_pTextureCom)
	{
		json Texture;

		if (iter.second.pDiffTex.lock())
			Texture.emplace("Diff"   , iter.second.pDiffTex.lock()->Get_TextureKey());
		if (iter.second.pNormTex.lock())
			Texture.emplace("Norm"   , iter.second.pNormTex.lock()->Get_TextureKey());
		if (iter.second.pORMTex.lock())
			Texture.emplace("ORM"    , iter.second.pORMTex.lock()->Get_TextureKey());
		Texture.emplace("Density", iter.second.fDensity);

		TexInfo.emplace(iter.first, Texture);
	}

	if (FAILED(CJson_Utility::Save_Json(szTextureInfoPath.c_str(), TexInfo)))
		return;
}

void CEditGround::Load_FilterTexture()
{
	if (!m_pTexture2D.Get())
		CreateFilterTexture();

	_tchar szFullTag[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, m_szSaveTextureTag.c_str(), (_int)m_szSaveTextureTag.length(), szFullTag, MAX_PATH);

	wstring szFilePath = TEXT("../Bin/GroundInfo/Filter_SubInfo/") + wstring(szFullTag) + TEXT(".bin");

	ifstream is(szFilePath, ios::binary);

	if (!is.is_open())
		return;

	_uint iSize, iData;
	read_typed_data(is, iSize);

	m_vColors.clear();

	for (_uint i = 0; i < iSize; ++i)
	{
		read_typed_data(is, iData);
		m_vColors.push_back(iData);
	}

	D3D11_MAPPED_SUBRESOURCE		SubResource;
	DEVICECONTEXT->Map(m_pTexture2D.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	for (_uint iZ = 0; iZ < FILTER_TEXTURE_SIZE; ++iZ)
	{
		for (_uint iX = 0; iX < FILTER_TEXTURE_SIZE; ++iX)
		{
			_ulong	iIndex = iZ * FILTER_TEXTURE_SIZE + iX;

			((_uint*)SubResource.pData)[iIndex] = m_vColors[iIndex];
		}
	}

	DEVICECONTEXT->Unmap(m_pTexture2D.Get(), 0);

	string szTexturePath = "../Bin/GroundInfo/Filter_SubTextureInfo/" + m_szSaveTextureTag + ".json";

	json json_info;
	
	if (FAILED(CJson_Utility::Load_Json(szTexturePath.c_str(), json_info)))
		return;

	for (auto& iter : m_pTextureCom)
	{
		iter.second.pDiffTex.lock().reset();
		iter.second.pNormTex.lock().reset();
		iter.second.pORMTex.lock().reset();
	}

	m_pTextureCom.clear();

	for (auto& iter : json_info.items())
	{
		string szDatakey = iter.key();
		json   json_tex  = iter.value();

		auto iter_find = m_pTextureCom.find(szDatakey);

		if (iter_find == m_pTextureCom.end())
		{
			TEXTURES_INFO Desc;
			Desc.pDiffTex = Add_Component<CTexture>();
			Desc.pNormTex = Add_Component<CTexture>();
			Desc.pORMTex  = Add_Component<CTexture>();

			m_pTextureCom.emplace(szDatakey, Desc);
			iter_find = m_pTextureCom.find(szDatakey);
		}

		for (auto& iter_item : json_tex.items())
		{
			string szitemkey = iter_item.key();

			if ("Diff" == szitemkey)
			{
				string szTextureName = iter_item.value();
				iter_find->second.pDiffTex.lock()->Use_Texture(szTextureName.c_str());
			}

			if ("Norm" == szitemkey)
			{
				string szTextureName = iter_item.value();
				iter_find->second.pNormTex.lock()->Use_Texture(szTextureName.c_str());
			}

			if ("ORM" == szitemkey)
			{
				string szTextureName = iter_item.value();
				iter_find->second.pORMTex.lock()->Use_Texture(szTextureName.c_str());
			}

			if ("Density" == szitemkey)
			{
				iter_find->second.fDensity = iter_item.value();
			}
		}
	}
}

void CEditGround::Write_Json(json& Out_Json)
{
	if ("" == m_szMeshName)
	{
		MSG_BOX("Check : m_szMeshName value is None");
		return;
	}

	__super::Write_Json(Out_Json);

	json TexInfo;

	for (auto& iter : m_pTextureCom)
	{
		json Texture;

		Texture.emplace("Diff", iter.second.pDiffTex.lock()->Get_TextureKey());
		Texture.emplace("Norm", iter.second.pNormTex.lock()->Get_TextureKey());
		Texture.emplace("ORM" , iter.second.pORMTex.lock()->Get_TextureKey());
		Texture.emplace("Density", iter.second.fDensity);

		TexInfo.emplace(iter.first, Texture);
	}

	if (!Check_File("../bin/GroundInfo/Filter" + m_szSaveTextureTag + ".dds"))
		Bake_FilterTexture();

	Out_Json.emplace("g_FilterTexture", string(m_szSaveTextureTag));
	Out_Json.emplace("TextureInfo"    , TexInfo);
	Out_Json.emplace("VIBufferCom"    , m_szMeshName);
	Out_Json.emplace("ShaderPass"     , m_iShaderPass);

	if (!Check_File("../bin/GroundInfo/Mesh/" + m_szSaveTextureTag + ".bin"))
		Bake_Mesh();

	if (Out_Json.end() != Out_Json.find("Hash"))
		Out_Json["Hash"] = typeid(CGround).hash_code();
	
	if (Out_Json.end() != Out_Json.find("Name"))
		Out_Json["Name"] = typeid(CGround).name();

	//Bake_Mesh();  
	//Bake_FilterTexture();
}

void CEditGround::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);

	if (In_Json.find("TextureInfo") != In_Json.end())
	{
		json TexInfo = In_Json["TextureInfo"];

		for (auto& iter : TexInfo.items())
		{
			string szkey  = iter.key();
			json Textures = iter.value();

			TEXTURES_INFO Desc;

			for (auto& iter_data : Textures.items())
			{
				string szDatakey = iter.key();
				json   json_tex  = iter.value();

				auto iter_find = m_pTextureCom.find(szDatakey);

				if (iter_find == m_pTextureCom.end())
				{
					TEXTURES_INFO Desc;
					Desc.pDiffTex = Add_Component<CTexture>();
					Desc.pNormTex = Add_Component<CTexture>();
					Desc.pORMTex  = Add_Component<CTexture>();

					m_pTextureCom.emplace(szDatakey, Desc);
					iter_find = m_pTextureCom.find(szDatakey);
				}

				for (auto& iter_item : json_tex.items())
				{
					string szitemkey = iter_item.key();

					if ("Diff" == szitemkey)
					{
						string szTextureName = iter_item.value();
						iter_find->second.pDiffTex.lock()->Use_Texture(szTextureName.c_str());
					}

					if ("Norm" == szitemkey)
					{
						string szTextureName = iter_item.value();
						iter_find->second.pNormTex.lock()->Use_Texture(szTextureName.c_str());
					}

					if ("ORM" == szitemkey)
					{
						string szTextureName = iter_item.value();
						iter_find->second.pORMTex.lock()->Use_Texture(szTextureName.c_str());
					}

					if ("Density" == szitemkey)
					{
						iter_find->second.fDensity = iter_item.value();
					}
				}
			}

			if (Desc.pDiffTex.lock() && Desc.pNormTex.lock())
			{
				m_pTextureCom[szkey] = Desc;
			}
		}
	}

	if (In_Json.find("VIBufferCom") != In_Json.end())
	{
		m_szMeshName = In_Json["VIBufferCom"];

		shared_ptr<MODEL_DATA> pModelData = GAMEINSTANCE->Get_ModelFromKey(m_szMeshName.c_str());

		if (!pModelData.get())
		{
			MSG_BOX("Err : CGround::Load_FromJson(...) <This Obj Make By [An]>");
			return;
		}

		/* --- Buffer Data --- */

		string szBinFilePath = "../Bin/GroundInfo/Mesh_SubInfo/" + m_szMeshName + ".bin";

		ifstream is(szBinFilePath, ios::binary);

		if (!is.is_open())
			return;

		_int	iNumVertexX, iNumVertexZ;
		_float	fInterval;

		read_typed_data(is, iNumVertexX);
		read_typed_data(is, iNumVertexZ);
		read_typed_data(is, fInterval);

		/* --- Create Buffer --- */

		Remove_Components<CVIBuffer_Ground>();
		m_pVIBufferCom = Add_Component<CVIBuffer_Ground>();
		m_pVIBufferCom.lock()->Init_Mesh(pModelData.get()->Mesh_Datas[0], iNumVertexX, iNumVertexZ, fInterval);

		m_vBufferInfo.x = _float(iNumVertexX);
		m_vBufferInfo.y = _float(iNumVertexZ);
		m_vBufferInfo.z = fInterval;

		m_bCreate = true;
	}

	if (In_Json.find("g_FilterTexture") != In_Json.end())
	{
		m_szSaveTextureTag = In_Json["g_FilterTexture"];

		Load_FilterTexture_FromJson(m_szSaveTextureTag);
	}

	if (In_Json.find("ShaderPass") != In_Json.end())
		m_iShaderPass = In_Json["ShaderPass"];
}

void CEditGround::Load_AllMeshInfo()
{
	fs::directory_iterator itr("../Bin/GroundInfo/Mesh/");

	string szFileName;

	while (itr != fs::end(itr))
	{
		const fs::directory_entry& entry = *itr;

		szFileName = entry.path().filename().string().c_str();
		szFileName = szFileName.substr(0, szFileName.size() - 4);

		if (FAILED(GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(), MODEL_TYPE::GROUND, XMMatrixIdentity())))
			continue;

		itr++;
	}
}

_bool CEditGround::Load_AddMeshInfo(string _szFileName)
{
	return !FAILED(GAMEINSTANCE->Load_Model(_szFileName.c_str(), "../bin/GroundInfo/Mesh/", MODEL_TYPE::GROUND, XMMatrixIdentity()));
}

_bool CEditGround::Load_AddTextureInfo(string _szFileName)
{
	wstring szFilePath = TEXT("../bin/GroundInfo/Mesh/");

	_tchar szFullTag[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, _szFileName.c_str(), (_int)_szFileName.length(), szFullTag, MAX_PATH);

	szFilePath += wstring(szFullTag) + TEXT(".dds");

	return !FAILED(GAMEINSTANCE->Load_Textures(_szFileName.c_str(), szFilePath.c_str(), MEMORY_TYPE::MEMORY_STATIC));
}

void CEditGround::Load_FilterTexture_FromJson(string _szFileName)
{
	if (!m_pTexture2D.Get())
		CreateFilterTexture();

	_tchar szFullTag[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, _szFileName.c_str(), (_int)_szFileName.length(), szFullTag, MAX_PATH);

	wstring szFilePath = TEXT("../Bin/GroundInfo/Filter_SubInfo/") + wstring(szFullTag) + TEXT(".bin");

	ifstream is(szFilePath, ios::binary);

	if (!is.is_open())
		return;

	_uint iSize, iData;
	read_typed_data(is, iSize);

	m_vColors.clear();

	for (_uint i = 0; i < iSize; ++i)
	{
		read_typed_data(is, iData);
		m_vColors.push_back(iData);
	}

	D3D11_MAPPED_SUBRESOURCE		SubResource;
	DEVICECONTEXT->Map(m_pTexture2D.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	for (_uint iZ = 0; iZ < FILTER_TEXTURE_SIZE; ++iZ)
	{
		for (_uint iX = 0; iX < FILTER_TEXTURE_SIZE; ++iX)
		{
			_ulong	iIndex = iZ * FILTER_TEXTURE_SIZE + iX;

			((_uint*)SubResource.pData)[iIndex] = m_vColors[iIndex];
		}
	}

	DEVICECONTEXT->Unmap(m_pTexture2D.Get(), 0);
}

void CEditGround::OnEventMessage(_uint iArg)
{
	if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
	{
		switch (m_eEditMode)
		{
			case EDIT_MODE::HEIGHT_FLAT:
			case EDIT_MODE::HEIGHT_LERP:
			case EDIT_MODE::HEIGHT_SET:
			{
				PickingGround();
			}
			break;

			case EDIT_MODE::FILLTER:
			{
				PickingFillterTextureDraw();
			}
			break;
		}


		if (ImGui::BeginTabBar("Ground"))
		{
			if (ImGui::BeginTabItem("Create VIBuffer"))
			{
				m_eEditMode = EDIT_MODE::NON;

				SetUp_Info();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Edit"))
			{
				SetUp_EditMode();
				SetUp_PinckingInfo();
				SetUp_ShaderComponent();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Texture"))
			{
				m_eEditMode = EDIT_MODE::NON;

				SetUp_Textures();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Load & Save"))
			{
				m_eEditMode = EDIT_MODE::NON;

				SetUp_File();

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");

	ImGui::Separator();
	ImGui::Separator();
}


void CEditGround::Free()
{
}