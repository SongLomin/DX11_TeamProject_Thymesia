#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CBoneNode;
END

BEGIN(Client)

class CActorDecor :
    public CGameObject
{
    GAMECLASS_H(CActorDecor);
    CLONE_H(CActorDecor, CGameObject);

public:// CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    void Init_ActorDecor(weak_ptr<CModel> In_pModelCom, weak_ptr<CTransform> In_ParentTransformCom, const string& szTargetNode = "WeaponCase1");
    void Init_Model(const string& strWeaponName, TIMESCALE_LAYER eLayer);
    void Set_OffsetMatrix(_fmatrix In_OffsetMatrix) { XMStoreFloat4x4(&m_OffsetMatrix, In_OffsetMatrix); }
    void Set_Offset(_fvector In_OffsetVector) { XMStoreFloat3(&m_vOffset, In_OffsetVector); };

    weak_ptr<CGameObject> Get_ParentObject();
 



protected:
    void SetUp_ShaderResource();

private:
    weak_ptr<CModel> m_pModelCom;
    weak_ptr<CShader> m_pShaderCom;
    weak_ptr<CRenderer> m_pRendererCom;

    weak_ptr<CGameObject> m_pParent;
    weak_ptr<CBoneNode> m_pTargetBoneNode;
    weak_ptr<CTransform> m_pParentTransformCom;


    _float4x4				m_WorldMatrix;
    _float4x4               m_OffsetMatrix;
    _float4x4               m_TransformationMatrix;
    _float3                 m_vOffset;


public:
     void Free();



};

END