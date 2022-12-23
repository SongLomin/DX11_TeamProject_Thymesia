#include "MeshData.h"

HRESULT MESH_DATA::Make_MeshData(const MODEL_TYPE& In_eModelType, aiMesh* In_pAiMesh, const _fmatrix& In_TransformMatrix, MESH_VTX_INFO* In_pVertexInfo)
{
    szName = In_pAiMesh->mName.C_Str();
    eModelType = In_eModelType;
    iNumVertices = In_pAiMesh->mNumVertices;
    iNumFaces = In_pAiMesh->mNumFaces;
    iNumBones = In_pAiMesh->mNumBones;
    iMaterialIndex = In_pAiMesh->mMaterialIndex;


    shared_ptr<VTXANIM[]> pFBXAnimVertices;
    shared_ptr<VTXMODEL[]> pFBXNonAnimVertices;

    // 애니메이션
    if (MODEL_TYPE::ANIM == In_eModelType)
    {
        for (_uint i(0); i < iNumBones; i++)
        {
            shared_ptr<BONE_DATA> pBoneData = make_shared<BONE_DATA>();
            pBoneData->Make_BoneData(In_pAiMesh->mBones[i]);
            Bone_Datas.push_back(pBoneData);
        }      

        // FBX에서 가져온 중복된 버텍스 리스트.
        pFBXAnimVertices = shared_ptr<VTXANIM[]>(DBG_NEW VTXANIM[iNumVertices]);

        for (_uint i(0); i < iNumVertices; ++i)
        {
            memcpy(&pFBXAnimVertices[i].vPosition, &In_pAiMesh->mVertices[i], sizeof(_float3));
            //XMStoreFloat3(&pAnimVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pAnimVertices[i].vPosition), In_TransformMatrix));

            memcpy(&pFBXAnimVertices[i].vNormal, &In_pAiMesh->mNormals[i], sizeof(_float3));
            XMStoreFloat3(&pFBXAnimVertices[i].vNormal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pFBXAnimVertices[i].vNormal), In_TransformMatrix)));

            memcpy(&pFBXAnimVertices[i].vTexUV, &In_pAiMesh->mTextureCoords[0][i], sizeof(_float2));
            memcpy(&pFBXAnimVertices[i].vTangent, &In_pAiMesh->mTangents[i], sizeof(_float3));

            Check_Position(In_pVertexInfo->vMax, pFBXAnimVertices[i].vPosition, [](_float _fLeft, _float _fRight)->_bool { return _fLeft < _fRight; });
            Check_Position(In_pVertexInfo->vMin, pFBXAnimVertices[i].vPosition, [](_float _fLeft, _float _fRight)->_bool { return _fLeft > _fRight; });
        }

        for (_uint i(0); i < iNumBones; ++i)
        {
            aiBone* pAIBone = In_pAiMesh->mBones[i];

            /* pAIBone->mNumWeights : 이 뼈는 몇개의 정점에 영향릉 주는지 */
            for (_uint j(0); j < pAIBone->mNumWeights; ++j)
            {
                /* pAIBone->mWeights[j].mVertexId : 그 중에 j번째 정점의 인덱스는 뭐였는지?  */
                /* pAIBone->mWeights[j].mWeight : j번째 정점에게 적용해야할 가중치. */

                if (0 == pFBXAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x)
                {
                    pFBXAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.x = i;
                    pFBXAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
                }

                else if (0 == pFBXAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y)
                {
                    pFBXAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.y = i;
                    pFBXAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
                }

                else if (0 == pFBXAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z)
                {
                    pFBXAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.z = i;
                    pFBXAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
                }

                else
                {
                    pFBXAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.w = i;
                    pFBXAnimVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
                }
            }
        }

        
        vector<VTXANIM> pCompressedVTXs;
        
        // 중복된 정점을 제거해서 압축한다.
        for (_int i = 1; i < iNumVertices; ++i)
        {
            _vector vVertice = XMLoadFloat3(&pFBXAnimVertices[i].vPosition);
            _bool bFlag = false;
            for (_int j = 0; j < pCompressedVTXs.size(); ++j)
            {
                _vector vTemp = XMLoadFloat3(&pCompressedVTXs[j].vPosition);

                _float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
                if (fLength <= DBL_EPSILON)
                {
                    bFlag = true;
                    break;
                }
            }

            if (!bFlag)
            {
                pCompressedVTXs.push_back(pFBXAnimVertices[i]);
            }
        }

        iNumVertices = pCompressedVTXs.size();
        pAnimVertices = shared_ptr<VTXANIM[]>(DBG_NEW VTXANIM[pCompressedVTXs.size()]);
        memcpy(&pAnimVertices[0], &pCompressedVTXs[0], sizeof(VTXANIM) * pCompressedVTXs.size());
    }

    // 애니메이션이 아님
    else if (MODEL_TYPE::NONANIM == In_eModelType)
    {
        // FBX에서 가져온 중복된 버텍스 리스트.
        pFBXNonAnimVertices = shared_ptr<VTXMODEL[]>(DBG_NEW VTXMODEL[iNumVertices]);

        //pVertices = shared_ptr<VTXMODEL[]>(DBG_NEW VTXMODEL[iNumVertices]);
        //pPosVertices = shared_ptr<VTXPOS[]>(DBG_NEW VTXPOS[iNumVertices]);

        for (_uint i(0); i < iNumVertices; ++i)
        {
            memcpy(&pFBXNonAnimVertices[i].vPosition, &In_pAiMesh->mVertices[i], sizeof(_float3));
            XMStoreFloat3(&pFBXNonAnimVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pFBXNonAnimVertices[i].vPosition), In_TransformMatrix));
            //memcpy(&pPosVertices[i].vPosition, &pVertices[i].vPosition, sizeof(_float3));

            memcpy(&pFBXNonAnimVertices[i].vNormal, &In_pAiMesh->mNormals[i], sizeof(_float3));
            XMStoreFloat3(&pFBXNonAnimVertices[i].vNormal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pFBXNonAnimVertices[i].vNormal), In_TransformMatrix)));
            
            if (In_pAiMesh->mTextureCoords[0])
                memcpy(&pFBXNonAnimVertices[i].vTexUV, &In_pAiMesh->mTextureCoords[0][i], sizeof(_float2));
            else
                pFBXNonAnimVertices[i].vTexUV = { 0.f, 0.f };

            if (In_pAiMesh->mTangents)
                memcpy(&pFBXNonAnimVertices[i].vTangent, &In_pAiMesh->mTangents[i], sizeof(_float3));
            else
                pFBXNonAnimVertices[i].vTangent = { 0.f, 0.f, 0.f };

            Check_Position(In_pVertexInfo->vMax, pFBXNonAnimVertices[i].vPosition, [](_float _fLeft, _float _fRight)->_bool { return _fLeft < _fRight; });
            Check_Position(In_pVertexInfo->vMin, pFBXNonAnimVertices[i].vPosition, [](_float _fLeft, _float _fRight)->_bool { return _fLeft > _fRight; });
        }

        vector<VTXMODEL> pCompressedVTXs;

        // 중복된 정점을 제거해서 압축한다.
        for (_int i = 1; i < iNumVertices; ++i)
        {
            _vector vVertice = XMLoadFloat3(&pFBXNonAnimVertices[i].vPosition);
            _bool bFlag = false;
            for (_int j = 0; j < pCompressedVTXs.size(); ++j)
            {
                _vector vTemp = XMLoadFloat3(&pCompressedVTXs[j].vPosition);

                _float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
                if (fLength <= DBL_EPSILON)
                {
                    bFlag = true;
                    break;
                }
            }

            if (!bFlag)
            {
                pCompressedVTXs.push_back(pFBXNonAnimVertices[i]);
            }
        }

        iNumVertices = pCompressedVTXs.size();
        pVertices = shared_ptr<VTXMODEL[]>(DBG_NEW VTXMODEL[pCompressedVTXs.size()]);
        memcpy(&pVertices[0], &pCompressedVTXs[0], sizeof(VTXMODEL) * pCompressedVTXs.size());
    }

    else if (MODEL_TYPE::NAVI == In_eModelType)
    {
        pPosVertices = shared_ptr<VTXPOS[]>(DBG_NEW VTXPOS[iNumVertices]);

        for (_uint i(0); i < iNumVertices; ++i)
        {
            memcpy(&pVertices[i].vPosition, &In_pAiMesh->mVertices[i], sizeof(_float3));
            XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), In_TransformMatrix));
        }
    }

    else
    {
        //모델 타입이 지정되지 않음.
        DEBUG_ASSERT;
    }

    //shared_ptr<FACEINDICES32[]> pFBXIndices = shared_ptr<FACEINDICES32[]>(DBG_NEW FACEINDICES32[iNumFaces]);
    pIndices = shared_ptr<FACEINDICES32[]>(DBG_NEW FACEINDICES32[iNumFaces]);

    for (_uint i(0); i < iNumFaces; ++i)
    {
        pIndices[i]._1 = In_pAiMesh->mFaces[i].mIndices[0];
        pIndices[i]._2 = In_pAiMesh->mFaces[i].mIndices[1];
        pIndices[i]._3 = In_pAiMesh->mFaces[i].mIndices[2];
    }

    if (MODEL_TYPE::ANIM == In_eModelType)
    {
        for (_int i = 0; i < iNumFaces; ++i)
        {
            _vector vVertice = XMLoadFloat3(&pFBXAnimVertices[pIndices[i]._1].vPosition);

            for (_int j = 0; j < iNumVertices; ++j)
            {
                _vector vTemp = XMLoadFloat3(&pAnimVertices[j].vPosition);

                _float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
                if (DBL_EPSILON > fLength)
                {

                    // Do. Somthing

                    pIndices[i]._1 = j;
                    break;
                }
            }

            vVertice = XMLoadFloat3(&pFBXAnimVertices[pIndices[i]._2].vPosition);

            for (_int j = 0; j < iNumVertices; ++j)
            {
                _vector vTemp = XMLoadFloat3(&pAnimVertices[j].vPosition);

                _float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
                if (DBL_EPSILON > fLength)
                {

                    // Do. Somthing

                    pIndices[i]._2 = j;
                    break;
                }
            }

            vVertice = XMLoadFloat3(&pFBXAnimVertices[pIndices[i]._3].vPosition);

            for (_int j = 0; j < iNumVertices; ++j)
            {
                _vector vTemp = XMLoadFloat3(&pAnimVertices[j].vPosition);

                _float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
                if (DBL_EPSILON > fLength)
                {

                    // Do. Somthing

                    pIndices[i]._3 = j;
                    break;
                }
            }

        }
    }
    else if (MODEL_TYPE::NONANIM == In_eModelType)
    {
        for (_int i = 0; i < iNumFaces; ++i)
        {
            _vector vVertice = XMLoadFloat3(&pFBXNonAnimVertices[pIndices[i]._1].vPosition);

            for (_int j = 0; j < iNumVertices; ++j)
            {
                _vector vTemp = XMLoadFloat3(&pVertices[j].vPosition);

                _float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
                if (DBL_EPSILON > fLength)
                {

                    // Do. Somthing

                    pIndices[i]._1 = j;
                    break;
                }
            }

            vVertice = XMLoadFloat3(&pFBXNonAnimVertices[pIndices[i]._2].vPosition);

            for (_int j = 0; j < iNumVertices; ++j)
            {
                _vector vTemp = XMLoadFloat3(&pVertices[j].vPosition);

                _float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
                if (DBL_EPSILON > fLength)
                {

                    // Do. Somthing

                    pIndices[i]._2 = j;
                    break;
                }
            }

            vVertice = XMLoadFloat3(&pFBXNonAnimVertices[pIndices[i]._3].vPosition);

            for (_int j = 0; j < iNumVertices; ++j)
            {
                _vector vTemp = XMLoadFloat3(&pVertices[j].vPosition);

                _float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
                if (DBL_EPSILON > fLength)
                {

                    // Do. Somthing

                    pIndices[i]._3 = j;
                    break;
                }
            }

        }
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
        for (_uint i(0); i < iNumBones; i++)
            Bone_Datas[i]->Bake_Binary(os);

        for (_uint i(0); i < iNumVertices; ++i)
            write_typed_data(os, pAnimVertices[i]);
    }

    else if(MODEL_TYPE::NONANIM == eModelType)
    {
        for (_uint i(0); i < iNumVertices; ++i)
            write_typed_data(os, pVertices[i]);
    }

    else if (MODEL_TYPE::NAVI == eModelType)
    {
        for (_uint i(0); i < iNumVertices; ++i)
            write_typed_data(os, pPosVertices[i]);
    }

    else if (MODEL_TYPE::GROUND == eModelType)
    {
        for (_uint i(0); i < iNumVertices; ++i)
            write_typed_data(os, pGroundVertices[i]);
    }

    for (_uint i(0); i < iNumFaces; ++i)
        write_typed_data(os, pIndices[i]);

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
        for (_uint i(0); i < iNumBones; i++)
        {
            shared_ptr<BONE_DATA> pBoneData = make_shared<BONE_DATA>();
            pBoneData->Load_FromBinary(is);
            Bone_Datas.push_back(pBoneData);
        }

        pAnimVertices = shared_ptr<VTXANIM[]>(DBG_NEW VTXANIM[iNumVertices]);

        for (_uint i(0); i < iNumVertices; ++i)
        {
            read_typed_data(is, pAnimVertices[i]);
        }

    }

    else if (MODEL_TYPE::NONANIM == eModelType)
    {
        pVertices = shared_ptr<VTXMODEL[]>(DBG_NEW VTXMODEL[iNumVertices]);
        pPosVertices = shared_ptr<VTXPOS[]>(DBG_NEW VTXPOS[iNumVertices]);

        for (_uint i(0); i < iNumVertices; ++i)
        {
            read_typed_data(is, pVertices[i]);
            memcpy(&pPosVertices[i].vPosition, &pVertices[i].vPosition, sizeof(_float3));
        }
    }

    else if (MODEL_TYPE::NAVI == eModelType)
    {
        pPosVertices = shared_ptr<VTXPOS[]>(DBG_NEW VTXPOS[iNumVertices]);

        for (_uint i(0); i < iNumVertices; ++i)
        {
            read_typed_data(is, pPosVertices[i]);
        }
    }

    else if (MODEL_TYPE::GROUND == eModelType)
    {
        pGroundVertices = shared_ptr<VTXGROUND[]>(DBG_NEW VTXGROUND[iNumVertices]);

        for (_uint i(0); i < iNumVertices; ++i)
        {
            read_typed_data(is, pGroundVertices[i]);
        }
    }

    pIndices = shared_ptr<FACEINDICES32[]>(DBG_NEW FACEINDICES32[iNumFaces]);

    for (_uint i(0); i < iNumFaces; ++i)
    {
        read_typed_data(is, pIndices[i]);
    }

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