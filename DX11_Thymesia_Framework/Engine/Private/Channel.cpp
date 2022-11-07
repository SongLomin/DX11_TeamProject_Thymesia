#include "Channel.h"
#include "BoneNode.h"
#include "Animation.h"
#include "Model.h"
#include "ChannelData.h"
#include "SMath.h"

GAMECLASS_C(CChannel);
CLONE_C(CChannel, CComponent);

HRESULT CChannel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChannel::Initialize(void* pArg)
{
	return S_OK;
}

void CChannel::Start()
{
}

void CChannel::Init_Channel(shared_ptr<CHANNEL_DATA> pChannelData, weak_ptr<CModel> pModel)
{
	m_szName = pChannelData->szNodeName;

	SetUp_BoneNodePtr(pModel);


	m_iNumKeyframes = max(pChannelData->iNumScalingKeys, pChannelData->iNumRotationKeys);
	m_iNumKeyframes = max(m_iNumKeyframes, pChannelData->iNumPositionKeys);

	_float3		vScale;
	_float4		vRotation;
	_float3		vPosition;

	for (_uint i = 0; i < m_iNumKeyframes; ++i)
	{
		KEYFRAME		KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

		if (pChannelData->iNumScalingKeys > i)
		{
			memcpy(&vScale, &pChannelData->tScalingKeys[i].vValue, sizeof(_float3));
			KeyFrame.fTime = (_float)pChannelData->tScalingKeys[i].fTime;
		}

		if (pChannelData->iNumRotationKeys > i)
		{
			/*memcpy(&vRotation, &pAIChannel->mRotationKeys[i].mValue, sizeof(_float4));*/
			vRotation.x = pChannelData->tRotationKeys[i].vValue.x;
			vRotation.y = pChannelData->tRotationKeys[i].vValue.y;
			vRotation.z = pChannelData->tRotationKeys[i].vValue.z;
			vRotation.w = pChannelData->tRotationKeys[i].vValue.w;
			KeyFrame.fTime = (_float)pChannelData->tRotationKeys[i].fTime;
		}

		if (pChannelData->iNumPositionKeys > i)
		{
			memcpy(&vPosition, &pChannelData->tPositionKeys[i].vValue, sizeof(_float3));
			KeyFrame.fTime = (_float)pChannelData->tPositionKeys[i].fTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_KeyFrames.push_back(KeyFrame);
	}



}

void CChannel::Update_TransformationMatrices(_float fCurrentTime)
{
	_vector			vScale, vRotation, vPosition;

	if (fCurrentTime >= m_KeyFrames.back().fTime)
	{
		vScale = XMLoadFloat3(&m_KeyFrames.back().vScale);
		vRotation = XMLoadFloat4(&m_KeyFrames.back().vRotation);
		vPosition = XMLoadFloat3(&m_KeyFrames.back().vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	else
	{
		m_iCurrentKeyFrame = 0;

		while (fCurrentTime > m_KeyFrames[m_iCurrentKeyFrame + 1].fTime)
		{
			++m_iCurrentKeyFrame;
		}

		_float fRatio = (fCurrentTime - m_KeyFrames[m_iCurrentKeyFrame].fTime)
			/ (m_KeyFrames[m_iCurrentKeyFrame + 1].fTime - m_KeyFrames[m_iCurrentKeyFrame].fTime);

		_vector			vSourScale, vSourRotation, vSourPosition;
		_vector			vDestScale, vDestRotation, vDestPosition;

		vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame].vScale);
		vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame].vRotation);
		vSourPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame].vPosition);

		vDestScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1].vScale);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame + 1].vRotation);
		vDestPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1].vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}


	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	if (nullptr != m_pBoneNode.lock())
		m_pBoneNode.lock()->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::Blend_PreTransformationMatrices(_float fBlendTime, _float In_fRatio)
{
	if (!m_pBoneNode.lock().get())
		return;

	KEYFRAME Key;
	ZeroMemory(&Key, sizeof(KEYFRAME));

	_vector			vScale, vRotation, vPosition;
	_uint			iCurrentKeyFrame = 0;


	if (fBlendTime >= m_KeyFrames.back().fTime)
	{
		vScale = XMLoadFloat3(&m_KeyFrames.back().vScale);
		vRotation = XMLoadFloat4(&m_KeyFrames.back().vRotation);
		vPosition = XMLoadFloat3(&m_KeyFrames.back().vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	else
	{
		while (fBlendTime > m_KeyFrames[iCurrentKeyFrame + 1].fTime)
		{
			++iCurrentKeyFrame;
		}

		_float fRatio = (fBlendTime - m_KeyFrames[iCurrentKeyFrame].fTime)
			/ (m_KeyFrames[iCurrentKeyFrame + 1].fTime - m_KeyFrames[iCurrentKeyFrame].fTime);

		_vector			vSourScale, vSourRotation, vSourPosition;
		_vector			vDestScale, vDestRotation, vDestPosition;

		vSourScale = XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrame].vScale);
		vSourRotation = XMLoadFloat4(&m_KeyFrames[iCurrentKeyFrame].vRotation);
		vSourPosition = XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrame].vPosition);

		vDestScale = XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrame + 1].vScale);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[iCurrentKeyFrame + 1].vRotation);
		vDestPosition = XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrame + 1].vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	
	XMStoreFloat3(&Key.vScale, vScale);
	XMStoreFloat4(&Key.vRotation, vRotation);
	XMStoreFloat3(&Key.vPosition, vPosition);

	if (m_pBoneNode.lock().get())
		m_pBoneNode.lock()->Blend_Animation(Key, In_fRatio);
}

void CChannel::Reset_KeyFrame()
{
	m_iCurrentKeyFrame = 0;
}

HRESULT CChannel::SetUp_BoneNodePtr(weak_ptr<CModel> pModel)
{
	m_pBoneNode = pModel.lock()->Find_BoneNode(m_szName);
	if (nullptr == m_pBoneNode.lock().get())
		assert(false);

	return S_OK;
}

void CChannel::Free()
{
}
