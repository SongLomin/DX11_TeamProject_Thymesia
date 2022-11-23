#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
struct MESH_DATA;
struct MODEL_DATA;
class CMeshContainer;
class CShader;


class ENGINE_DLL CVIBuffer_Model_Instance :
    public CVIBuffer
{
    GAMECLASS_H(CVIBuffer_Model_Instance)
    SHALLOW_COPY(CVIBuffer_Model_Instance)
    CLONE_H(CVIBuffer_Model_Instance, CComponent)

public:
    typedef struct tagModelMaterial
    {
        weak_ptr<CTexture> pTextures[AI_TEXTURE_TYPE_MAX];
    }MODEL_MATERIAL;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void	Start() override;
    virtual HRESULT Render() override;
    HRESULT         Render_Mesh(_uint iMeshContainerIndex);

public:
    _uint Get_InstanceCount() const { return m_iNumInstance; }
    _uint Get_MaterialIndex() const { return m_iMaterialIndex; }
    _uint Get_NumMeshContainers() const { return m_iNumMeshContainers; }
    string Get_ModelKey() const { return m_szModelKey; }
    weak_ptr<MODEL_DATA> Get_ModelData() const {return m_pModelData; }

    void Culling_Instance(vector<INSTANCE_MESH_DESC>& In_ParticleDescs, const _float In_fRange);

public:
    void Init_Model(const char* In_szModelName);
    void Init_Instance(_uint In_iNumInstance);

    HRESULT Bind_SRV(weak_ptr<CShader> pShader, const char* pConstantName, _uint iMeshContainerIndex, aiTextureType eActorType);

    void Update(const vector<INSTANCE_MESH_DESC>& In_ParticleDescs);

private:
    void Create_Materials(const char* pModelFilePath);
    void Create_MeshContainers();
    void Reset_Model();

private:
    ComPtr<ID3D11Buffer>        m_pVBInstance;

    _uint						m_iMaterialIndex    = 0;
    _uint						m_iInstanceStride   = 0;
    _uint						m_iNumInstance      = 0;
    _uint                       m_iVisibleCount    = 0;

    string									m_szModelKey;

    shared_ptr<MODEL_DATA>					m_pModelData;
    vector<weak_ptr<CMeshContainer>>		m_MeshContainers;
    vector<MODEL_MATERIAL>					m_Materials;
    _uint									m_iNumMeshContainers    = 0;
    _uint									m_iNumMaterials         = 0;

private:
    virtual void OnDestroy() override;
    void Free();

};

END