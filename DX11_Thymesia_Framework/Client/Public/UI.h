#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CFader;
END

BEGIN(Client)

class CCharacter;
class CPlayer;
class CStatus_Player;

class CUI :
    public CGameObject
{
    GAMECLASS_H(CUI)
        CLONE_H(CUI, CGameObject)


public:
    //LeftTop : 너비, 높이 절반을 포지션에 더함.
    //Center : 더하기 X
    enum UI_ALIGN_TYPE
    {
        ALIGN_LEFTTOP,
        ALIGN_CENTER,
        ALIGN_END
    };

    enum class UI_POINT
    {
        LEFT,
        RIGHT,
        TOP,
        BOTTOM,
        LEFT_TOP,
        LEFT_BOTTOM,
        RIGHT_TOP,
        RIGHT_BOTTOM,
        UI_POINT_END
    };


public:
    struct UI_DESC
    {
        _float fX;
        _float fY;
        _float fSizeX;
        _float fSizeY;
        _float fDepth = 1.f;
    };

public:
    UI_DESC Get_UIDESC() const {
        return m_tUIDesc;
    }

public:
    // CGameObject을(를) 통해 상속됨
    virtual _float Get_CamDistance() override;
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

    weak_ptr<CUI>   Get_This();

public:
    void            Set_TextureIndex(const _uint& Index) { m_iTextureIndex = Index; }
    virtual void    Set_Texture(const _char* sKey);
    virtual void    Set_UIPosition(const _float fX, const _float fY, const _float fSizeX, const _float fSizeY, UI_ALIGN_TYPE eType = UI_ALIGN_TYPE::ALIGN_CENTER);
    virtual void    Set_UIPosition(const _float fX, const _float fY, UI_ALIGN_TYPE eType);//먼저 선언된 사이즈가 있어야함.
    virtual void    Set_UIPosition(const _float fX, const _float fY);
    void            Set_UIPositionAllChilds(const _float fX, const _float fY);



    virtual void    Set_UIDesc(UI_DESC _tUIDesc);
    virtual void    Set_Size(const _float& fSizeX, const _float& fSizeY);
    virtual void    Set_Y(_float fY);
    virtual void    Set_X(_float fX);

    virtual void    Set_AlphaColor(_float fAlphaColor) { m_fAlphaColor = fAlphaColor; }

    void            Set_AlphaColorAllChilds(_float fAlphaColor);

    virtual void    Set_Depth(_float _fDepth);
    virtual void    Set_SizeX(const _float In_fSize);

    virtual void    Add_Y(_float fAmountY) { m_tUIDesc.fY += fAmountY; }

    virtual void    Set_Render(_bool bRender) { m_bRender = bRender; }

    virtual void	Set_RenderGroup(RENDERGROUP eRenderGroup) { m_eRenderGroup = eRenderGroup; }
public: /* Shaking */
    virtual void    Add_Shaking(const _float& In_ShakeTime, const _float& _fShakePower = 5.f);

public:
    virtual void            OnEnable(void* _Arg = nullptr);
    //객체의 상태            가 비활성화 상태로 변경될 때, 호출되는 이벤트입니다.
    virtual void            OnDisable() override;

    //Moon Functions

public:
    virtual _bool            MousePtInUI();

protected:
    virtual void             Add_Child(weak_ptr<CUI> pChild);
    virtual void             Set_Target(weak_ptr<CBase> pTarget);
    virtual void             Set_Owner(weak_ptr<CUI> pOwner);
public:
    _float2                  Get_Point(UI_POINT eType);
    _float                   Get_Y();
    _float                   Get_SizeY() { return m_tUIDesc.fSizeY; }
    _float2                   Get_Size();

    _float2                  GetPos();


protected:
    virtual void             Enable_AllEventChild();
    virtual void             Disable_AllEventChild();
    
public:
    _float		            Get_AlphaColor() { return m_fAlphaColor; }

    void		            Set_OffsetPosition(_float2 fOffset) { m_fOffsetPosition = fOffset; }
    void                    Set_OffsetY(_float fOffsetY) { m_fOffsetPosition.y = fOffsetY; }
protected:
    virtual HRESULT         SetUp_ShaderResource();

    virtual void            Bind_Player();

private:
    virtual void Update_Shaking(_float fTimeDelta);

protected:
   virtual void                    SetUp_Component();
  
protected:
    weak_ptr<CShader> m_pShaderCom;
    weak_ptr<CRenderer> m_pRendererCom;
    weak_ptr<CTexture> m_pTextureCom;
    weak_ptr<CVIBuffer_Rect> m_pVIBufferCom;
    weak_ptr<CFader> m_pFaderCom;


protected:
    FDelegate<>     CallBack_ShakingStart;
    FDelegate<>     CallBack_ShakingEnd;
    
protected:
    UI_DESC         m_tUIDesc;
    _float4x4		m_ProjMatrix;
    RENDERGROUP     m_eRenderGroup = RENDERGROUP::RENDER_UI;
    _uint           m_iTextureIndex = 0;
    _uint           m_iPassIndex = 0;
    _float	        m_fAlphaColor = 1.f;
    _float          m_fOwnerAlphaColor = 1.f;
    _bool           m_bRender = true;

protected:
    vector<weak_ptr<CUI>> m_vecChildUI;
    vector<weak_ptr<CUI>> m_vecEventChildUI;
   
    weak_ptr<CUI>       m_pOwner;



    weak_ptr<CBase>       m_pTarget;

    weak_ptr<CPlayer> m_pPlayer;
    weak_ptr<CStatus_Player> m_pPlayerStatus;


protected: /* For. UI Shaking */
    _float          m_fCurrentShakeTime = 0.f;
    _float          m_fShakingTime;
    _float          m_fShakeFreq = 0.04f;
    _float          m_fCurrentFreq = 0.f;
    _float          m_fPower;
    _float2         m_fOffsetPosition{0.f, 0.f};
    _float2         m_fShakedPos = { 0.f,0.f };
    _bool           m_bShaking;

protected:
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

protected:
    virtual void OnEventMessage(_uint iArg) override;
    void Free();

};

END
