#include "BoneNode.h"
#include "ModelData.h"
#include "SMath.h"

GAMECLASS_C(CBoneNode)
CLONE_C(CBoneNode, CComponent)

_matrix CBoneNode::Get_CombinedMatrix() const
{
	_matrix TempCombinedTransformationMatrix = XMLoadFloat4x4(&m_CombinedTransformationMatrix);

	if ((_byte)ROOTNODE_FLAG::X & m_RootNodeFlags)
	{
		// ÀÌµ¿ °ª »©³ÂÀ¸´Ï 0Ã³¸®.
		TempCombinedTransformationMatrix.r[3].m128_f32[0] = 0.f;	
	}

	if ((_byte)ROOTNODE_FLAG::Y & m_RootNodeFlags)
	{
		// ÀÌµ¿ °ª »©³ÂÀ¸´Ï 0Ã³¸®.
		TempCombinedTransformationMatrix.r[3].m128_f32[1] = 0.f;
	}

	if ((_byte)ROOTNODE_FLAG::Z & m_RootNodeFlags)
	{
		// ÀÌµ¿ °ª »©³ÂÀ¸´Ï 0Ã³¸®.
		TempCombinedTransformationMatrix.r[3].m128_f32[2] = 0.f;
	}

	return TempCombinedTransformationMatrix;
}

void CBoneNode::Set_RootNodeFlags(_byte RootNodeFlags)
{
	m_RootNodeFlags = RootNodeFlags;
}

HRESULT CBoneNode::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoneNode::Initialize(void* pArg)
{

	return S_OK;
}

void CBoneNode::Start()
{
}

void CBoneNode::Init_BoneNode(shared_ptr<NODE_DATA> pNodeData, weak_ptr<CBoneNode> pParent, _uint iDepth, _float4x4& In_ModelTransformMatrix)
{
	if (!pNodeData.get())
		DEBUG_ASSERT;

	if (!pParent.lock().get() && iDepth != 0)
		DEBUG_ASSERT;

	m_iDepth = iDepth;
	m_pParent = pParent;
	m_ModelTransformMatrix = In_ModelTransformMatrix;
	m_szName = pNodeData->szName;

	memcpy(&m_TransformationMatrix, &pNodeData->TransformationMatrix, sizeof(_float4x4));
	XMStoreFloat4x4(&m_TransformationMatrix,
		XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());


}

void CBoneNode::Update_CombinedTransformationMatrix()
{
	if (nullptr == m_pParent.lock().get())
	{
		m_CombinedTransformationMatrix = m_TransformationMatrix;
	}
	else
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * m_pParent.lock()->Get_CombinedMatrix());
	}
}

void CBoneNode::Bake_PreKeyFrame()
{
	XMStoreFloat3(&m_tPreKeyFrame.vPosition, SMath::Get_PositionMatrix(XMLoadFloat4x4(&m_TransformationMatrix)).r[3]);
	XMStoreFloat4(&m_tPreKeyFrame.vRotation, XMQuaternionRotationMatrix(SMath::Get_RotationMatrix(XMLoadFloat4x4(&m_TransformationMatrix))));
	XMStoreFloat3(&m_tPreKeyFrame.vScale, SMath::Get_Scale(XMLoadFloat4x4(&m_TransformationMatrix)));
}

void CBoneNode::Blend_Animation(const KEYFRAME& tCurKey, _float fRatio)
{

	_vector			vScale, vRotation, vPosition;
	
	vPosition = XMVectorLerp(XMLoadFloat3(&m_tPreKeyFrame.vPosition), XMLoadFloat3(&tCurKey.vPosition), fRatio);
	vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_tPreKeyFrame.vRotation), XMLoadFloat4(&tCurKey.vRotation), fRatio);
	vScale = XMVectorLerp(XMLoadFloat3(&m_tPreKeyFrame.vScale), XMLoadFloat3(&tCurKey.vScale), fRatio);

	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	Set_TransformationMatrix(TransformationMatrix);

}

void CBoneNode::Free()
{

}