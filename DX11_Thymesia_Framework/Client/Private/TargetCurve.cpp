#include "stdafx.h"
#include "TargetCurve.h"
#include "VIBuffer_Curve.h"
#include "Renderer.h"
#include "Shader.h"
#include "GameManager.h"
#include "Monster.h"
#include "Player.h"
#include "Model.h"
#include "BoneNode.h"

GAMECLASS_C(CTargetCurve)
CLONE_C(CTargetCurve, CGameObject)

HRESULT CTargetCurve::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTargetCurve::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	_uint iNumDensity = 50;
	m_pVIBufferCom = Add_Component<CVIBuffer_Curve>(&iNumDensity);
	m_pRenderderCom = Add_Component<CRenderer>();
	m_pShaderCom = Add_Component<CShader>();

	m_pShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_VtxCurve"),
		VTXTEXCURVE_INSTANCE_DECLARATION::Element,
		VTXTEXCURVE_INSTANCE_DECLARATION::iNumElements);

	GET_SINGLE(CGameManager)->CallBack_FocusInMonster += bind(&CTargetCurve::Call_UpdateTarget, this);
	GET_SINGLE(CGameManager)->CallBack_FocusOutMonster += bind(&CTargetCurve::Call_ReleaseTarget, this);

	return S_OK;
}

HRESULT CTargetCurve::Start()
{
	__super::Start();

	m_pParentTransformCom = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Transform();

	return S_OK;
}

void CTargetCurve::Tick(_float fTimeDelta)
{
	if (!m_pParentTransformCom.lock() || !m_pTargetTransformCom.lock())
	{
		return;
	}

	__super::Tick(fTimeDelta);


	_matrix  matTargetMonsterWorld = XMLoadFloat4x4(&m_pTargetModelCom.lock()->Get_TransformationMatrix());
	_matrix  matTargetCombined = m_pTargetBoneNodeCom.lock()->Get_CombinedMatrix();


	_matrix BoneMatrix = matTargetCombined * matTargetMonsterWorld;

	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	_matrix MonsterWorldMatrix = BoneMatrix * m_pTargetTransformCom.lock()->Get_UnScaledWorldMatrix();

	_matrix CurvePoints;
	
	CurvePoints.r[1] = m_pParentTransformCom.lock()->Get_Position();
	CurvePoints.r[1].m128_f32[1] += 2.2f;
	CurvePoints.r[2] = MonsterWorldMatrix.r[3];

	_vector vLook = CurvePoints.r[2] - CurvePoints.r[1];
	vLook = XMVectorSetW(XMVector3Normalize(vLook), 1.f);
	_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));

	CurvePoints.r[0] = XMVector3TransformNormal(vLook, XMMatrixRotationAxis(vRight, XMConvertToRadians(-75.0f)));
	CurvePoints.r[0] = CurvePoints.r[1] - CurvePoints.r[0];
	CurvePoints.r[3] = XMVector3TransformNormal(vLook, XMMatrixRotationAxis(vRight, XMConvertToRadians(75.0f)));
	CurvePoints.r[3] = CurvePoints.r[2] + CurvePoints.r[3];

	XMStoreFloat4x4(&m_CurvePoints, CurvePoints);

}

void CTargetCurve::LateTick(_float fTimeDelta)
{
	if (!m_pParentTransformCom.lock() || !m_pTargetTransformCom.lock())
	{
		return;
	}

	__super::LateTick(fTimeDelta);

	m_pRenderderCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONLIGHT, m_thisToGameObject);
}

void CTargetCurve::Before_Render(_float fTimeDelta)
{
	__super::Before_Render(fTimeDelta);
}

HRESULT CTargetCurve::Render()
{
	SetUp_ShaderResource();

	__super::Render();

	m_pShaderCom.lock()->Begin(0);

	m_pVIBufferCom.lock()->Render();

	return S_OK;
}

void CTargetCurve::SetUp_ShaderResource()
{
	_matrix IdentityMatrix = XMMatrixIdentity();
	IdentityMatrix = XMMatrixTranspose(IdentityMatrix);

	_matrix CurvePoints = XMMatrixTranspose(XMLoadFloat4x4(&m_CurvePoints));
	_matrix RotationMatrix = XMMatrixTranspose(XMMatrixRotationZ(XMConvertToRadians(-45.0f)));

	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", (void*)(&IdentityMatrix), sizeof(_float4x4))))
		DEBUG_ASSERT;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
		DEBUG_ASSERT;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
		DEBUG_ASSERT;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_Points", (void*)(&CurvePoints), sizeof(_float4x4))))
		DEBUG_ASSERT;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_RotationMatrix", (void*)(&RotationMatrix), sizeof(_float4x4))))
		DEBUG_ASSERT;

}

void CTargetCurve::Init_ParentCurve(weak_ptr<CTransform> pParentTransform)
{
	m_pParentTransformCom = pParentTransform;
}

void CTargetCurve::Set_Target(weak_ptr<CTransform> pTargetTransform)
{
	m_pTargetTransformCom = pTargetTransform;
}

void CTargetCurve::Call_UpdateTarget()
{
	weak_ptr<CMonster> pMonster = GET_SINGLE(CGameManager)->Get_TargetMonster();

	m_pTargetTransformCom = pMonster.lock()->Get_Transform();
	m_pTargetModelCom = pMonster.lock()->Get_Model();

	m_pTargetBoneNodeCom = m_pTargetModelCom.lock()->Find_BoneNode("Spine01");
	if (!m_pTargetBoneNodeCom.lock())
	{
		m_pTargetBoneNodeCom = m_pTargetModelCom.lock()->Find_BoneNode("Bip001-Spine1");
	}
	if (!m_pTargetBoneNodeCom.lock())
	{
		m_pTargetBoneNodeCom = m_pTargetModelCom.lock()->Find_BoneNode("spine_01");
	}
}

void CTargetCurve::Call_ReleaseTarget()
{
	m_pTargetTransformCom = weak_ptr<CTransform>();
	m_pTargetModelCom = weak_ptr<CModel>();
	m_pTargetBoneNodeCom = weak_ptr<CBoneNode>();

}

void CTargetCurve::OnDestroy()
{
	GET_SINGLE(CGameManager)->CallBack_FocusInMonster -= bind(&CTargetCurve::Call_UpdateTarget, this);
	GET_SINGLE(CGameManager)->CallBack_FocusOutMonster -= bind(&CTargetCurve::Call_ReleaseTarget, this);
}

void CTargetCurve::Free()
{
}
