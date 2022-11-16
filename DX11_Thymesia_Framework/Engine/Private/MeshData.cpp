#include "MeshData.h"

HRESULT MESH_DATA::Make_MeshData(const MODEL_TYPE& In_eModelType, aiMesh* In_pAiMesh, const _fmatrix& In_TransformMatrix)
{
    szName = In_pAiMesh->mName.C_Str();
    eModelType = In_eModelType;
    iNumVertices = In_pAiMesh->mNumVertices;
    iNumFaces = In_pAiMesh->mNumFaces;
    iNumBones = In_pAiMesh->mNumBones;
    iMaterialIndex = In_pAiMesh->mMaterialIndex;

    // �ִϸ��̼�
    if (MODEL_TYPE::ANIM == In_eModelType)
    {
        for (_uint i = 0; i < iNumBones; i++)
        {
            shared_ptr<BONE_DATA> pBoneData = make_shared<BONE_DATA>();
            pBoneData->Make_BoneData(In_pAiMesh->mBones[i]);
            Bone_Datas.push_back(pBoneData);
        }      

        pAnimVertices = shared_ptr<VTXANIM[]>(new VTXANIM[iNumVertices]);

        _float3 vMaxPos, vMinPos, vCenterPos;

        for (_uint i = 0; i < iNumVertices; ++i)
        {
            memcpy(&pAnimVertices[i].vPosition, &In_pAiMesh->mVertices[i], sizeof(_float3));
            //XMStoreFloat3(&pAnimVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pAnimVertices[i].vPosition), In_TransformMatrix));

            memcpy(&pAnimVertices[i].vNormal, &In_pAiMesh->mNormals[i], sizeof(_float3));
            XMStoreFloat3(&pAnimVertices[i].vNormal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pAnimVertices[i].vNormal), In_TransformMatrix)));

            memcpy(&pAnimVertices[i].vTexUV, &In_pAiMesh->mTextureCoords[0][i], sizeof(_float2));
            memcpy(&pAnimVertices[i].vTangent, &In_pAiMesh->mTangents[i], sizeof(_float3));


            // TODO : Map Tool Info, For Test
            // Check_Position(vMaxPos, pVertices[i].vPosition, [](_float _fLeft, _float _fRight)->_bool { return _fLeft < _fRight; });
            // Check_Position(vMinPos, pVertices[i].vPosition, [](_float _fLeft, _float _fRight)->_bool { return _fLeft > _fRight; });               
        }

        // TODO : Map Tool Info, For Test
        // Compute_Center(vMaxPos, vMinPos, vCenterPos);
        // Bake_MeshInfo(vMaxPos, vMinPos, vCenterPos);

        for (_uint i = 0; i < iNumBones; ++i)
        {
            aiBone* pAIBone = In_pAiMesh->mBones[i];

            /* pAIBone->mNumWeights : �� ���� ��� ������ ���⸪ �ִ��� */
            for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
            {
                /* pAIBone->mWeights[j].mVertexId : �� �߿� j��° ������ �ε����� ��������?  */
                /* pAIBone->mWeights[j].mWeight : j��° �������� �����ؾ��� ����ġ. */

                if (0 == pAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x)
                {
                    pAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.x = i;
                    pAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
                }

                else if (0 == pAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y)
                {
                    pAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.y = i;
                    pAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
                }

                else if (0 == pAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z)
                {
                    pAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.z = i;
                    pAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
                }

                else
                {
                    pAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.w = i;
                    pAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
                }
            }
        }

    }

    // �ִϸ��̼��� �ƴ�
    else if (MODEL_TYPE::NONANIM == In_eModelType)
    {
        pVertices = shared_ptr<VTXMODEL[]>(new VTXMODEL[iNumVertices]);
        pPosVertices = shared_ptr<VTXPOS[]>(new VTXPOS[iNumVertices]);

        _float3 vMaxPos, vMinPos, vCenterPos;

        for (_uint i = 0; i < iNumVertices; ++i)
        {
            memcpy(&pVertices[i].vPosition, &In_pAiMesh->mVertices[i], sizeof(_float3));
            XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), In_TransformMatrix));
            memcpy(&pPosVertices[i].vPosition, &pVertices[i].vPosition, sizeof(_float3));

            memcpy(&pVertices[i].vNormal, &In_pAiMesh->mNormals[i], sizeof(_float3));
            XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), In_TransformMatrix)));
            
            if (In_pAiMesh->mTextureCoords[0])
            {
                memcpy(&pVertices[i].vTexUV, &In_pAiMesh->mTextureCoords[0][i], sizeof(_float2));
            }
            else
            {
                pVertices[i].vTexUV = { 0.f, 0.f };
            }

            if (In_pAiMesh->mTangents)
            {
                memcpy(&pVertices[i].vTangent, &In_pAiMesh->mTangents[i], sizeof(_float3));
            }
            else
            {
                pVertices[i].vTangent = { 0.f, 0.f, 0.f };
            }

            // TODO : Map Tool Info, For Test
            Check_Position(vMaxPos, pVertices[i].vPosition, [](_float _fLeft, _float _fRight)->_bool { return _fLeft < _fRight; });
            Check_Position(vMinPos, pVertices[i].vPosition, [](_float _fLeft, _float _fRight)->_bool { return _fLeft > _fRight; });
        }

        // TODO : Map Tool Info, For Test
        Compute_Center(vMaxPos, vMinPos, vCenterPos);
        Bake_MeshInfo(vMaxPos, vMinPos, vCenterPos);
    }

    else if (MODEL_TYPE::NAVI == In_eModelType)
    {
        pPosVertices = shared_ptr<VTXPOS[]>(new VTXPOS[iNumVertices]);

        for (_uint i = 0; i < iNumVertices; ++i)
        {
            memcpy(&pVertices[i].vPosition, &In_pAiMesh->mVertices[i], sizeof(_float3));
            XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), In_TransformMatrix));
        }
    }

    else
    {
        //�� Ÿ���� �������� ����.
        DEBUG_ASSERT;
    }

    pIndices = shared_ptr<FACEINDICES32[]>(new FACEINDICES32[iNumFaces]);

    for (_uint i = 0; i < iNumFaces; ++i)
    {
        pIndices[i]._1 = In_pAiMesh->mFaces[i].mIndices[0];
        pIndices[i]._2 = In_pAiMesh->mFaces[i].mIndices[1];
        pIndices[i]._3 = In_pAiMesh->mFaces[i].mIndices[2];
    }

    return S_OK;
}

void MESH_DATA::Bake_Binary(ofstream& os)
{
    write_typed_data(os, szName.size());
    os.write(&szName[0], szName.size());

    write_typed_data(os, eModelType);
    write_typed_data(os, iNumVertices);
    write_typed_data(os, iNumFaces);
    write_typed_data(os, iNumBones);
    write_typed_data(os, iMaterialIndex);
    
    if (MODEL_TYPE::ANIM == eModelType)
    {
        for (_uint i = 0; i < iNumBones; i++)
        {
            Bone_Datas[i]->Bake_Binary(os);
        }

        for (_uint i = 0; i < iNumVertices; ++i)
        {
            write_typed_data(os, pAnimVertices[i]);
        }
    }

    else if(MODEL_TYPE::NONANIM == eModelType)
    {
        for (_uint i = 0; i < iNumVertices; ++i)
        {
            write_typed_data(os, pVertices[i]);
        }
    }

    else if (MODEL_TYPE::NAVI == eModelType)
    {
        for (_uint i = 0; i < iNumVertices; ++i)
        {
            write_typed_data(os, pPosVertices[i]);
        }
    }

    else if (MODEL_TYPE::GROUND == eModelType)
    {
        for (_uint i = 0; i < iNumVertices; ++i)
        {
            write_typed_data(os, pGroundVertices[i]);
        }
    }

    for (_uint i = 0; i < iNumFaces; ++i)
    {
        write_typed_data(os, pIndices[i]);
    }

}

void MESH_DATA::Load_FromBinary(ifstream& is)
{
    size_t istringSize;
    read_typed_data(is, istringSize);
    szName.resize(istringSize);
    is.read(&szName[0], istringSize);

    read_typed_data(is, eModelType);
    read_typed_data(is, iNumVertices);
    read_typed_data(is, iNumFaces);
    read_typed_data(is, iNumBones);
    read_typed_data(is, iMaterialIndex);

    if (MODEL_TYPE::ANIM == eModelType)
    {
        for (_uint i = 0; i < iNumBones; i++)
        {
            shared_ptr<BONE_DATA> pBoneData = make_shared<BONE_DATA>();
            pBoneData->Load_FromBinary(is);
            Bone_Datas.push_back(pBoneData);
        }

        pAnimVertices = shared_ptr<VTXANIM[]>(new VTXANIM[iNumVertices]);

        for (_uint i = 0; i < iNumVertices; ++i)
        {
            read_typed_data(is, pAnimVertices[i]);
        }

    }

    else if (MODEL_TYPE::NONANIM == eModelType)
    {
        pVertices = shared_ptr<VTXMODEL[]>(new VTXMODEL[iNumVertices]);
        pPosVertices = shared_ptr<VTXPOS[]>(new VTXPOS[iNumVertices]);

        for (_uint i = 0; i < iNumVertices; ++i)
        {
            read_typed_data(is, pVertices[i]);
            memcpy(&pPosVertices[i].vPosition, &pVertices[i].vPosition, sizeof(_float3));
        }
    }

    else if (MODEL_TYPE::NAVI == eModelType)
    {
        pPosVertices = shared_ptr<VTXPOS[]>(new VTXPOS[iNumVertices]);

        for (_uint i = 0; i < iNumVertices; ++i)
        {
            read_typed_data(is, pPosVertices[i]);
        }
    }

    else if (MODEL_TYPE::GROUND == eModelType)
    {
        pGroundVertices = shared_ptr<VTXGROUND[]>(new VTXGROUND[iNumVertices]);

        for (_uint i = 0; i < iNumVertices; ++i)
        {
            read_typed_data(is, pGroundVertices[i]);
        }
    }

    pIndices = shared_ptr<FACEINDICES32[]>(new FACEINDICES32[iNumFaces]);

    for (_uint i = 0; i < iNumFaces; ++i)
    {
        read_typed_data(is, pIndices[i]);
    }

}

void MESH_DATA::Bake_MeshInfo(_float3 _vMaxPos, _float3 _vMinPos, _float3 _vCenterPos)
{
    string szBinFilePath = "../Bin/MapTool_MeshInfo/";

    switch (eModelType)
    {
        case MODEL_TYPE::NONANIM : szBinFilePath += "NONANIM/"; break;
        case MODEL_TYPE::ANIM    : szBinFilePath += "ANIM/";    break;
        case MODEL_TYPE::GROUND  : szBinFilePath += "GROUND/";  break;

        default:
            return;
    }

    szBinFilePath += szName + ".bin";

    ofstream os(szBinFilePath, ios::binary);

    if (!os.is_open())
        return;

    write_typed_data(os, _vMaxPos);
    write_typed_data(os, _vMinPos);
    write_typed_data(os, _vCenterPos);

    os.close();
}

void MESH_DATA::Check_Position(_float3& _vRecodePosion, _float3 _vValue, _bool(*_pFncPtr)(_float, _float))
{
    if (_pFncPtr(_vRecodePosion.x, _vValue.x))
        _vRecodePosion.x = _vValue.x;

    if (_pFncPtr(_vRecodePosion.y, _vValue.y))
        _vRecodePosion.y = _vValue.y;

    if (_pFncPtr(_vRecodePosion.z, _vValue.z))
        _vRecodePosion.z = _vValue.z;
}

void MESH_DATA::Compute_Center(_float3 _vMaxPos, _float3 _vMinPos, _float3& _vCenterOut)
{
    _vector vDist      = (XMLoadFloat3(&_vMaxPos) + XMLoadFloat3(&_vMinPos)) * 0.5f;
    _vector vCentorPos = XMLoadFloat3(&_vMinPos) + vDist;

    XMStoreFloat3(&_vCenterOut, vCentorPos);
}
