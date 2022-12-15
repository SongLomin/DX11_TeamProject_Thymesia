#include "CustomFont.h"
#include "GameInstance.h"

GAMECLASS_C(CCustomFont)

HRESULT CCustomFont::Initialize(const _tchar* pFontFilePath, ComPtr<ID3D11DeviceContext> pDeviceContext)
{
    m_pDeviceContext = pDeviceContext;
    m_pBatch = make_unique<SpriteBatch>(pDeviceContext.Get()); 
    m_pFont = make_unique<SpriteFont>(DEVICE, pFontFilePath); 

    return S_OK;
}

void CCustomFont::Add_Text(const tstring& pString, const _float2& vPosition, _fvector vColor, _bool bAlways, _bool bCenterAlign)
{
    TEXTINFO tTextInfo;
    m_TextInfos.push_back(tTextInfo);
    
    TEXTINFO& pCurrentTextInfo = m_TextInfos.back();

    pCurrentTextInfo.szText = pString;
    pCurrentTextInfo.vPosition = vPosition;
    XMStoreFloat4(&pCurrentTextInfo.vColor, vColor);
    pCurrentTextInfo.bAlways = bAlways;
    pCurrentTextInfo.fRotation = 0.f;
    pCurrentTextInfo.vScale = _float2(1.f, 1.f);

    if (bCenterAlign)
    {
        _float fTextSize = m_pFont->MeasureString(pCurrentTextInfo.szText.c_str(), false).m128_f32[0];
        pCurrentTextInfo.vPosition.x -= fTextSize * 0.5f;
    }

}

void CCustomFont::Add_Text(const TEXTINFO& In_tTextInfo)
{
    TEXTINFO tTextInfo;
    m_TextInfos.push_back(tTextInfo);

    TEXTINFO& pCurrentTextInfo = m_TextInfos.back();

    pCurrentTextInfo = In_tTextInfo;

    if (pCurrentTextInfo.bCenterAlign)
    {
        _float fTextSize = m_pFont->MeasureString(pCurrentTextInfo.szText.c_str(), false).m128_f32[0];
        fTextSize *= pCurrentTextInfo.vScale.x;
        pCurrentTextInfo.vPosition.x -= fTextSize * 0.5f;
    }
}

HRESULT CCustomFont::Render(const _tchar* pString, const _float2& vPosition, _fvector vColor)
{/*
    m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);*/

    m_pBatch->Begin();

    m_pFont->DrawString(m_pBatch.get(), pString, vPosition, vColor);
    
    

    m_pBatch->End();

    return S_OK;
}

void CCustomFont::Render(ID3D11DeviceContext* pDeviceContext)
{/*
	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);*/

    m_pBatch->Begin();

    _vector vColor;

    for (auto iter = m_TextInfos.begin(); iter != m_TextInfos.end();)
    {
        vColor = XMLoadFloat4(&(*iter).vColor);

        if (!(*iter).bAlways)
        {
            m_pFont->DrawString(m_pBatch.get(), (*iter).szText.c_str(), (*iter).vPosition, vColor, (*iter).fRotation, { 0.f, 0.f }, (*iter).vScale);

           iter = m_TextInfos.erase(iter);
        }

        else
        {
            m_pFont->DrawString(m_pBatch.get(), (*iter).szText.c_str(), (*iter).vPosition, vColor, (*iter).fRotation, { 0.f, 0.f }, (*iter).vScale);

            iter++;
        }
    }

    m_pBatch->End();

}

void CCustomFont::RenderWithRenderGroup(const RENDERGROUP In_eRenderGroup)
{
    // m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);

    m_pBatch->Begin();

    _vector vColor;

    for (auto iter = m_TextInfos.begin(); iter != m_TextInfos.end();)
    {
        // Assert: The TextInfo belongs to an invalid RenderGroup.
        if (!(RENDERGROUP::RENDER_BEFOREUI <= (*iter).eRenderGroup &&
            (*iter).eRenderGroup <= RENDERGROUP::RENDER_AFTER_UI))
            DEBUG_ASSERT;

        if (In_eRenderGroup != (*iter).eRenderGroup)
        {
            iter++;
            continue;
        }

        vColor = XMLoadFloat4(&(*iter).vColor);

        if (!(*iter).bAlways)
        {
            m_pFont->DrawString(m_pBatch.get(), (*iter).szText.c_str(), (*iter).vPosition, vColor, (*iter).fRotation, { 0.f, 0.f }, (*iter).vScale);

            iter = m_TextInfos.erase(iter);
        }

        else
        {
            m_pFont->DrawString(m_pBatch.get(), (*iter).szText.c_str(), (*iter).vPosition, vColor, (*iter).fRotation, { 0.f, 0.f }, (*iter).vScale);

            iter++;
        }
    }

    m_pBatch->End();
}

shared_ptr<CCustomFont> CCustomFont::Create(const _tchar* pFontFilePath, ComPtr<ID3D11DeviceContext> pDeviceContext)
{
    shared_ptr<CCustomFont>		pInstance = make_shared<CCustomFont>();
    pInstance->Initialize(pFontFilePath, pDeviceContext.Get());
    pInstance->m_this = pInstance;
    return pInstance;
}

void CCustomFont::Free()
{
}
