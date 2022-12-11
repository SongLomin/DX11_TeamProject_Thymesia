#pragma once
#include "Component.h"

BEGIN(Engine)

class CModel;
class CBoneNode;
struct CHANNEL_DATA;

class ENGINE_DLL CChannel :
	public CComponent
{
	GAMECLASS_H(CChannel);
	CLONE_H(CChannel, CComponent);

protected:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;

public:
	_uint Get_NumKeyFrames();

public:
	void Init_Channel(shared_ptr<CHANNEL_DATA> pChannelData, weak_ptr<CModel> pModel);

	void Update_TransformationMatrices(_float fCurrentTime);
	void Blend_PreTransformationMatrices(_float fBlendTime, _float In_fRatio);

	void Reset_KeyFrame();
public:
	HRESULT SetUp_BoneNodePtr(weak_ptr<CModel> pModel);

private:
	string						m_szName;
	//_uint						m_iNumKeyframes = 0;
	//vector<KEYFRAME>			m_KeyFrames;
	weak_ptr<CBoneNode>			m_pBoneNode;
	_uint						m_iCurrentKeyFrame = 0;
	weak_ptr<CModel>			m_pModel;
	shared_ptr<CHANNEL_DATA>	m_pChannelData;

public:
	void Free();
};

END