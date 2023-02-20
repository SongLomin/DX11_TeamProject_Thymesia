#include "Animation.h"
#include "Model.h"
#include "ModelData.h"
#include "Channel.h"
#include "GameObject.h"

GAMECLASS_C(CAnimation);
CLONE_C(CAnimation, CComponent);

HRESULT CAnimation::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CAnimation::Initialize(void* pArg)
{
	m_fTimeAcc = 0.f;
	return S_OK;
}

void CAnimation::Start()
{
}

void CAnimation::Init_Animation(weak_ptr<ANIMATION_DATA> pAnimData, weak_ptr<CModel> pModel, _uint iTimeScaleLayer)
{
	if (pModel.lock().get() == nullptr)
		assert(false);

	m_szName = pAnimData.lock()->szName;
	m_pModel = pModel;
	m_pAnimData = pAnimData;
	m_iTimeScaleLayer = iTimeScaleLayer;

	m_fDuration = (_float)m_pAnimData.lock()->fDuration;
	m_fTickPerSecond = (_float)m_pAnimData.lock()->fTickPerSecond;

	m_fAnimRatio = 0.f;

	m_Channels.reserve(m_pAnimData.lock()->iNumChannels);

	for (_uint i(0); i < m_pAnimData.lock()->iNumChannels; ++i)
	{
		shared_ptr<CHANNEL_DATA> pChannelData = m_pAnimData.lock()->Channel_Datas[i];

		weak_ptr<CChannel> pChannel = m_pOwner.lock()->Add_Component<CChannel>();
		pChannel.lock()->Init_Channel(pChannelData, pModel);
		m_Channels.emplace_back(pChannel);
	}

	for (_uint i(0); i < m_pAnimData.lock()->iNumChannels; ++i)
	{
		if (m_iMaxkeyFrame < m_Channels[i].lock()->Get_NumKeyFrames())
			m_iMaxkeyFrame = m_Channels[i].lock()->Get_NumKeyFrames();
	}
}

void CAnimation::Update_TransformationMatrices(_float fTimeDelta)
{
	m_fTimeAcc += m_fTickPerSecond * fTimeDelta * m_fAnimationSpeed * GAMEINSTANCE->Get_TimeScale(m_iTimeScaleLayer);
	m_fAnimRatio = m_fTimeAcc / m_fDuration;

	if (m_fTimeAcc >= m_fDuration)
	{

		for (_uint i(0); i < m_pAnimData.lock()->iNumChannels; ++i)
		{
			m_Channels[i].lock()->Reset_KeyFrame();
		}

		m_pModel.lock()->CallBack_AnimationEnd(m_pModel.lock()->Get_CurrentAnimationIndex());
		m_pModel.lock()->Reset_DeltaBonePositions();

		m_fTimeAcc = 0.f;
		return;
	}

	for (_uint i(0); i < m_pAnimData.lock()->iNumChannels; ++i)
		m_Channels[i].lock()->Update_TransformationMatrices(m_fTimeAcc);

	_int iCurrentKey = (_int)Get_CurrentChannelKeyIndex();
	if (iCurrentKey != m_iPreChannelKey)
	{
		for (_uint i(m_iPreChannelKey + 1); i <= (_uint)iCurrentKey; ++i)
			CallBack_NextChannelKey(i);
		
		m_iPreChannelKey = iCurrentKey;
	}
}

void CAnimation::Blend_Animation(_float fMaxBlendTime, _float fRatio)
{
	m_fAnimRatio = 0.f;

	for (_uint i(0); i < m_pAnimData.lock()->iNumChannels; ++i)
		m_Channels[i].lock()->Blend_PreTransformationMatrices(fMaxBlendTime, fRatio);

}

void CAnimation::Reset_Animaition()
{
	m_fTimeAcc       = 0.f;
	m_fAnimRatio     = 0.f;
	m_iPreChannelKey = -1;
	for (_uint i(0); i < m_pAnimData.lock()->iNumChannels; ++i)
		m_Channels[i].lock()->Reset_KeyFrame();
}

void CAnimation::Set_StartAnimationKey(_uint iKeyIndex)
{
	m_fTimeAcc = (m_fDuration / m_iMaxkeyFrame) * iKeyIndex;
}

void CAnimation::Set_AnimationSpeed(_float fAnimationSpeed)
{
	m_fAnimationSpeed = fAnimationSpeed;
}

_uint CAnimation::Get_CurrentChannelKeyIndex() const
{
	return (_uint)(m_fTimeAcc / (m_fDuration / m_iMaxkeyFrame));
}

_uint CAnimation::Get_MaxChannelKeyIndex() const
{
	return m_iMaxkeyFrame;
}

void CAnimation::OnDestroy()
{
	for (auto& elem : m_Channels)
	{
		m_pOwner.lock()->Remove_Component(elem);
	}
}

void CAnimation::Free()
{
}
