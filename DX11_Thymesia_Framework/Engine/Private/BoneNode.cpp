#include "BoneNode.h"
#include "ModelData.h"
#include "SMath.h"

GAMECLASS_C(CBoneNode)
CLONE_C(CBoneNode, CComponent)

_matrix CBoneNode::Get_CombinedMatrix() const
{
	_matrix TempCombinedTransformationMatrix = XMLoadFloat4x4(&m_CombinedTransformationMatrix);

	if (m_bRootNode)
	{
		// 이동 값 빼냈으니 0처리.
		TempCombinedTransformationMatrix.r[3].m128_f32[0] = 0.f;
		TempCombinedTransformationMatrix.r[3].m128_f32[2] = 0.f;

		// 그럼 회전은...?
		// 매트릭스가 X축, Y축, Z축 회전된 매트릭스라면
		// Y축 회전만 소거하고 싶다.
		// Transform에서 회전해줄거니까.

		// 원래는 애니메이션이 XYZ 회전. (이때 트랜스폼은 영향X 항상 회전 항등)
		// 현재는 애니메이션이 XZ  회전. (이때 트랜스폼은 Y의 회전을 옮겨 받음)

		// 매트릭스의 XYZ회전값이 나옴.

		_matrix PositionMatrix = SMath::Get_PositionMatrix(TempCombinedTransformationMatrix);
		_matrix RotationMatrix = SMath::Get_RotationMatrix(TempCombinedTransformationMatrix);
		_matrix ScaleMatrix = SMath::Get_ScaleMatrix(TempCombinedTransformationMatrix);

		_float3 vPitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(RotationMatrix);

		vPitchYawRoll.y = 0.f;

		TempCombinedTransformationMatrix = ScaleMatrix
			* XMMatrixRotationRollPitchYaw(vPitchYawRoll.x, vPitchYawRoll.y, vPitchYawRoll.z)
			* PositionMatrix;

		_matrix TransformationMatrix = XMLoadFloat4x4(&m_ModelTransformMatrix);
		_matrix JustPitchTransformationMatrix = XMMatrixRotationRollPitchYaw(0.f, SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(TransformationMatrix)).y, 0.f);

		TempCombinedTransformationMatrix *= JustPitchTransformationMatrix;
	}

	return TempCombinedTransformationMatrix;
}

HRESULT CBoneNode::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoneNode::Initialize(void* pArg)
{
	m_bRootNode = false;

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