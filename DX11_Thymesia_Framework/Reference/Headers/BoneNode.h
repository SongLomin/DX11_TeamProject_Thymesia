#pragma once
#include "Component.h"

BEGIN(Engine)

struct NODE_DATA;

class ENGINE_DLL CBoneNode :
    public CComponent
{
    GAMECLASS_H(CBoneNode);
    CLONE_H(CBoneNode, CComponent);

public:
	_uint Get_Depth() const {
		return m_iDepth;
	}

	const char* Get_Name() const {
		return m_szName.c_str();
	}

	_matrix Get_OffsetMatrix() const {
		return XMLoadFloat4x4(&m_OffsetMatrix);
	}

	_matrix Get_CombinedMatrix() const {

		_matrix TempCombinedTransformationMatrix = XMLoadFloat4x4(&m_CombinedTransformationMatrix);

		if (m_bRootNode)
		{
			TempCombinedTransformationMatrix.r[3].m128_f32[0] = 0.f;
			TempCombinedTransformationMatrix.r[3].m128_f32[2] = 0.f;
		}

		return TempCombinedTransformationMatrix;
	}

	_matrix Get_TransformationMatrix() const {
		return XMLoadFloat4x4(&m_TransformationMatrix);
	}

	void Set_OffsetMatrix(_fmatrix OffsetMatrix) {
		XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

protected:
    // CComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;

public:
    void Init_BoneNode(shared_ptr<NODE_DATA> pNodeData, weak_ptr<CBoneNode> pParent, _uint iDepth);
	void Update_CombinedTransformationMatrix();

	void Bake_PreKeyFrame();
	void Blend_Animation(const KEYFRAME& tCurKey, _float fRatio);

private:
	GETSET(_bool, bRootNode);

    _uint			m_iDepth = 0;
    string			m_szName;

    _float4x4		m_OffsetMatrix;
    _float4x4		m_TransformationMatrix;
    _float4x4		m_CombinedTransformationMatrix;

	weak_ptr<CBoneNode> m_pParent;

	KEYFRAME		m_tPreKeyFrame;


public:
    virtual void Free() override;


    

};

END