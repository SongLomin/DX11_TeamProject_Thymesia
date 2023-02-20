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

_uint CChannel::Get_NumKeyFrames()
{
	return m_pChannelData->iNumKeyframes;
}

void CChannel::Init_Channel(shared_ptr<CHANNEL_DATA> pChannelData, weak_ptr<CModel> pModel)
{
	m_pChannelData = pChannelData;
	m_szName = pChannelData->szNodeName;

	SetUp_BoneNodePtr(pModel);

	//m_iNumKeyframes = pChannelData->iNumKeyframes;
	//m_iNumKeyframes = max(pChannelData->iNumScalingKeys, pChannelData->iNumRotationKeys);
	//m_iNumKeyframes = max(m_iNumKeyframes, pChannelData->iNumPositionKeys);

	//_float3		vScale;
	//_float4		vRotation;
	//_float3		vPosition;

	//for (_uint i = 0; i < m_iNumKeyframes; ++i)
	//{
	//	KEYFRAME		KeyFrame;
	//	ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

	//	if (pChannelData->iNumScalingKeys > i)
	//	{
	//		memcpy(&vScale, &pChannelData->tScalingKeys[i].vValue, sizeof(_float3));
	//		KeyFrame.fTime = (_float)pChannelData->tScalingKeys[i].fTime;
	//	}

	//	if (pChannelData->iNumRotationKeys > i)
	//	{
	//		/*memcpy(&vRotation, &pAIChannel->mRotationKeys[i].mValue, sizeof(_float4));*/
	//		vRotation.x = pChannelData->tRotationKeys[i].vValue.x;
	//		vRotation.y = pChannelData->tRotationKeys[i].vValue.y;
	//		vRotation.z = pChannelData->tRotationKeys[i].vValue.z;
	//		vRotation.w = pChannelData->tRotationKeys[i].vValue.w;
	//		KeyFrame.fTime = (_float)pChannelData->tRotationKeys[i].fTime;
	//	}

	//	if (pChannelData->iNumPositionKeys > i)
	//	{
	//		memcpy(&vPosition, &pChannelData->tPositionKeys[i].vValue, sizeof(_float3));
	//		KeyFrame.fTime = (_float)pChannelData->tPositionKeys[i].fTime;
	//	}

	//	KeyFrame.vScale = vScale;
	//	KeyFrame.vRotation = vRotation;
	//	KeyFrame.vPosition = vPosition;

	//	m_KeyFrames.push_back(KeyFrame);
	//}



}

void CChannel::Update_TransformationMatrices(_float fCurrentTime)
{
	_vector			vScale, vRotation, vPosition;
	vector<KEYFRAME>& MyKeyFrames = m_pChannelData->tKeyFrames;

	if (fCurrentTime >= MyKeyFrames.back().fTime)
	{
		vScale = XMLoadFloat3(&MyKeyFrames.back().vScale);
		vRotation = XMLoadFloat4(&MyKeyFrames.back().vRotation);
		vPosition = XMLoadFloat3(&MyKeyFrames.back().vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	else
	{
		m_iCurrentKeyFrame = 0;

		while (fCurrentTime > MyKeyFrames[m_iCurrentKeyFrame + 1].fTime)
		{
			++m_iCurrentKeyFrame;
		}

		_float fRatio = (fCurrentTime - MyKeyFrames[m_iCurrentKeyFrame].fTime)
			/ (MyKeyFrames[m_iCurrentKeyFrame + 1].fTime - MyKeyFrames[m_iCurrentKeyFrame].fTime);

		_vector			vSourScale, vSourRotation, vSourPosition;
		_vector			vDestScale, vDestRotation, vDestPosition;

		vSourScale = XMLoadFloat3(&MyKeyFrames[m_iCurrentKeyFrame].vScale);
		vSourRotation = XMLoadFloat4(&MyKeyFrames[m_iCurrentKeyFrame].vRotation);
		vSourPosition = XMLoadFloat3(&MyKeyFrames[m_iCurrentKeyFrame].vPosition);

		vDestScale = XMLoadFloat3(&MyKeyFrames[m_iCurrentKeyFrame + 1].vScale);
		vDestRotation = XMLoadFloat4(&MyKeyFrames[m_iCurrentKeyFrame + 1].vRotation);
		vDestPosition = XMLoadFloat3(&MyKeyFrames[m_iCurrentKeyFrame + 1].vPosition);

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
	vector<KEYFRAME>& MyKeyFrames = m_pChannelData->tKeyFrames;
	_uint			iCurrentKeyFrame = 0;


	if (fBlendTime >= MyKeyFrames.back().fTime)
	{
		vScale = XMLoadFloat3(&MyKeyFrames.back().vScale);
		vRotation = XMLoadFloat4(&MyKeyFrames.back().vRotation);
		vPosition = XMLoadFloat3(&MyKeyFrames.back().vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	else
	{
		while (fBlendTime > MyKeyFrames[iCurrentKeyFrame + 1].fTime)
		{
			++iCurrentKeyFrame;
		}

		_float fRatio = (fBlendTime - MyKeyFrames[iCurrentKeyFrame].fTime)
			/ (MyKeyFrames[iCurrentKeyFrame + 1].fTime - MyKeyFrames[iCurrentKeyFrame].fTime);

		_vector			vSourScale, vSourRotation, vSourPosition;
		_vector			vDestScale, vDestRotation, vDestPosition;

		vSourScale = XMLoadFloat3(&MyKeyFrames[iCurrentKeyFrame].vScale);
		vSourRotation = XMLoadFloat4(&MyKeyFrames[iCurrentKeyFrame].vRotation);
		vSourPosition = XMLoadFloat3(&MyKeyFrames[iCurrentKeyFrame].vPosition);

		vDestScale = XMLoadFloat3(&MyKeyFrames[iCurrentKeyFrame + 1].vScale);
		vDestRotation = XMLoadFloat4(&MyKeyFrames[iCurrentKeyFrame + 1].vRotation);
		vDestPosition = XMLoadFloat3(&MyKeyFrames[iCurrentKeyFrame + 1].vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	
	XMStoreFloat3(&Key.vScale, vScale);
	XMStoreFloat4(&Key.vRotation, vRotation);
	XMStoreFloat3(&Key.vPosition, vPosition);

	if (m_pBoneNode.lock())
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
