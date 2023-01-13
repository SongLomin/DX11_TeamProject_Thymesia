#include "Transform.h"
#include "Shader.h"
#include "GameObject.h"
#include "SMath.h"
#include "Navigation.h"

GAMECLASS_C(CTransform)
CLONE_C(CTransform, CComponent)

CTransform::CTransform(const CTransform& rhs)
{
    *this = rhs;
}

_matrix CTransform::Get_UnScaledWorldMatrix() const
{
    _matrix ResultMatrix = XMLoadFloat4x4(&m_WorldMatrix);

    return SMath::Get_MatrixNormalize(ResultMatrix);
}

void CTransform::Set_Scaled(_float3 vScale)
{
    _vector		vRight = XMVector3Normalize(Get_State(CTransform::STATE_RIGHT)) * vScale.x;
    _vector		vUp = XMVector3Normalize(Get_State(CTransform::STATE_UP)) * vScale.y;
    _vector		vLook = XMVector3Normalize(Get_State(CTransform::STATE_LOOK)) * vScale.z;

    Set_State(CTransform::STATE_RIGHT, vRight);
    Set_State(CTransform::STATE_UP, vUp);
    Set_State(CTransform::STATE_LOOK, vLook);
}

_float3 CTransform::Get_Scaled()
{
    return _float3(XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_RIGHT))),
        XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_UP))),
        XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_LOOK))));
}

HRESULT CTransform::Initialize_Prototype()
{
    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

    return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
    USE_START(CTransform);
    return S_OK;
}

void CTransform::Start()
{
    m_pOwner.lock()->CallBack_Bind_SRV += bind(&CTransform::Set_ShaderResource, this, placeholders::_1, "g_WorldMatrix");
}

HRESULT CTransform::Set_ShaderResource(weak_ptr<CShader> pShader, const char* pConstantName)
{
    _float4x4		WorldMatrixTP;
    XMStoreFloat4x4(&WorldMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));
    pShader.lock()->Set_RawValue(pConstantName, &WorldMatrixTP, sizeof(_float4x4));

    return S_OK;
}

HRESULT CTransform::Go_Direction(_fvector vDirection, _float fTimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	vPosition += vDirection /* 회전 속도: XMConvertToRadians(90.0f) */ * fTimeDelta;

	Set_State(CTransform::STATE_TRANSLATION, vPosition);
	return S_OK;
}

HRESULT CTransform::Go_Straight(_float fTimeDelta, weak_ptr<CNavigation> pNaviCom)
{
    _vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
    _vector		vLook = XMVector3Normalize(Get_State(CTransform::STATE_LOOK));
    _vector     vNewPosition = vPosition + XMVector3Normalize(vLook) * fTimeDelta;

    if (pNaviCom.lock().get())
    {
        _float3 vLineDir;

        if (!pNaviCom.lock()->IsMove(vNewPosition, &vLineDir))
        {
            _vector vLineDirFromVector = XMLoadFloat3(&vLineDir);
            _vector vRoatatedPosition;

            _float fDotLookToLine = XMVectorGetX(XMVector3Dot(XMVector3Normalize(vLook), vLineDirFromVector));

            fTimeDelta *= fabsf(fDotLookToLine);

            if (0.f < fDotLookToLine)
            {
                vRoatatedPosition = vPosition + XMVector3Normalize(vLineDirFromVector) * fTimeDelta;
            }
            else
            {
                vRoatatedPosition = vPosition + -XMVector3Normalize(vLineDirFromVector) * fTimeDelta;
            }

            if (!pNaviCom.lock()->IsMove(vRoatatedPosition, &vLineDir))
            {
                return S_OK;
            }

            Set_State(CTransform::STATE_TRANSLATION, vRoatatedPosition);
            return S_OK;
        }
    }

    Set_State(CTransform::STATE_TRANSLATION, vNewPosition);

    return S_OK;
}

HRESULT CTransform::Go_Backward(_float fTimeDelta)
{
    _vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
    _vector		vLook = Get_State(CTransform::STATE_LOOK);

    vPosition -= XMVector3Normalize(vLook) /* 회전 속도: XMConvertToRadians(90.0f) */ * fTimeDelta;

    Set_State(CTransform::STATE_TRANSLATION, vPosition);

    return S_OK;
}

HRESULT CTransform::Go_Left(_float fTimeDelta)
{
    _vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
    _vector		vRight = Get_State(CTransform::STATE_RIGHT);

    vPosition -= XMVector3Normalize(vRight) /* 속도:5.f */ * fTimeDelta;

    Set_State(CTransform::STATE_TRANSLATION, vPosition);

    return S_OK;
}

HRESULT CTransform::Go_Right(_float fTimeDelta)
{
    _vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
    _vector		vRight = Get_State(CTransform::STATE_RIGHT);

    vPosition += XMVector3Normalize(vRight) /* 속도:5.f */ * fTimeDelta;

    Set_State(CTransform::STATE_TRANSLATION, vPosition);

    return S_OK;
}

HRESULT CTransform::Go_Up(_float fTimeDelta)
{
    _vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
    _vector		vUp = Get_State(CTransform::STATE_UP);

    vPosition += XMVector3Normalize(vUp) /* 속도:5.f */ * fTimeDelta;

    Set_State(CTransform::STATE_TRANSLATION, vPosition);

    return S_OK;
}

HRESULT CTransform::Go_Down(_float fTimeDelta)
{
    _vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
    _vector		vUp = Get_State(CTransform::STATE_UP);

    vPosition -= XMVector3Normalize(vUp) /* 속도:5.f */ * fTimeDelta;

    Set_State(CTransform::STATE_TRANSLATION, vPosition);
    return S_OK;
}

HRESULT CTransform::Go_Straight_WorldAxis
(
    _float fTimeDelta
)
{
    _vector vPosition = this->Get_State(CTransform::STATE_TRANSLATION);
    _vector vWorldLook{ 0.f, 0.f, 1.f };
    vPosition += vWorldLook * fTimeDelta;
    this->Set_State(CTransform::STATE_TRANSLATION, vPosition);
    return S_OK;
}

HRESULT CTransform::Go_Backward_WorldAxis
(
    _float fTimeDelta
)
{
    _vector vPosition = this->Get_State(CTransform::STATE_TRANSLATION);
    _vector vWorldLook{ 0.f, 0.f, 1.f };
    vPosition -= vWorldLook * fTimeDelta;
    this->Set_State(CTransform::STATE_TRANSLATION, vPosition);
    return S_OK;
}

HRESULT CTransform::Go_Left_WorldAxis
(
    _float fTimeDelta
)
{
    _vector vPosition = this->Get_State(CTransform::STATE_TRANSLATION);
    _vector vWorldRight{ 1.f, 0.f, 0.f };
    vPosition -= vWorldRight * fTimeDelta;
    this->Set_State(CTransform::STATE_TRANSLATION, vPosition);
    return S_OK;
}

HRESULT CTransform::Go_Right_WorldAxis
(
    _float fTimeDelta
)
{
    _vector vPosition = this->Get_State(CTransform::STATE_TRANSLATION);
    _vector vWorldRight{ 1.f, 0.f, 0.f };
    vPosition += vWorldRight * fTimeDelta;
    this->Set_State(CTransform::STATE_TRANSLATION, vPosition);
    return S_OK;
}

HRESULT CTransform::Go_Up_WorldAxis
(
    _float fTimeDelta
)
{
    _vector vPosition = this->Get_State(CTransform::STATE_TRANSLATION);
    _vector vWorldUp{ 0.f, 1.f, 0.f };
    vPosition += vWorldUp * fTimeDelta;
    this->Set_State(CTransform::STATE_TRANSLATION, vPosition);
    return S_OK;
}

HRESULT CTransform::Go_Down_WorldAxis
(
    _float fTimeDelta
)
{
    _vector vPosition = this->Get_State(CTransform::STATE_TRANSLATION);
    _vector vWorldUp{ 0.f, 1.f, 0.f };
    vPosition -= vWorldUp * fTimeDelta;
    this->Set_State(CTransform::STATE_TRANSLATION, vPosition);
    return S_OK;
}

HRESULT CTransform::Set_Position(_fvector vPos, weak_ptr<CNavigation> pNaviCom)
{
    if (pNaviCom.lock())
    {
        _float3 vLineDir;

        if (!pNaviCom.lock()->IsMove(XMVectorSetW(vPos, 1.f), &vLineDir))
        {
            return S_OK;
        }
    }

    Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(vPos, 1.f));

    return S_OK;
}

HRESULT CTransform::Add_Position(_fvector vPos, weak_ptr<CNavigation> pNaviCom)
{
    _vector vPosition = Get_State(CTransform::STATE_TRANSLATION);

    vPosition = XMVectorAdd(vPosition, vPos);
    vPosition.m128_f32[3] = 1.f;

    if (pNaviCom.lock().get())
    {
        _float3 vLineDir;

        if (!pNaviCom.lock()->IsMove(vPosition, &vLineDir))
        {
            return S_OK;
        }
    }

    Set_State(CTransform::STATE_TRANSLATION, vPosition);

    return S_OK;
}

HRESULT CTransform::Add_PositionWithRotation(_fvector vPos, weak_ptr<CNavigation> pNaviCom, const _bool& In_bReverse)
{
    _vector vPosition = Get_State(CTransform::STATE_TRANSLATION);
    _vector vRotatedPosition = XMVector3TransformCoord(vPos, SMath::Get_RotationMatrix(Get_WorldMatrix()));

    _float3 vPitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(Get_WorldMatrix()));
    _vector vPitchYawRollFromVector = XMLoadFloat3(&vPitchYawRoll);

    vPosition = XMVectorAdd(vPosition, vRotatedPosition * (In_bReverse ? -1.f : 1.f));
    vPosition.m128_f32[3] = 1.f;

    if (pNaviCom.lock().get())
    {
        _float3 vLineDir;

        if (!pNaviCom.lock()->IsMove(vPosition, &vLineDir))
        {
            return S_OK;
        }
    }

    Set_State(CTransform::STATE_TRANSLATION, vPosition);

    return S_OK;
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
    _matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, /* 회전 속도: XMConvertToRadians(90.0f) */ fTimeDelta);

    _vector		vRight = Get_State(CTransform::STATE_RIGHT);
    _vector		vUp = Get_State(CTransform::STATE_UP);
    _vector		vLook = Get_State(CTransform::STATE_LOOK);

    vRight = XMVector3TransformNormal(vRight, RotationMatrix);
    vUp = XMVector3TransformNormal(vUp, RotationMatrix);
    vLook = XMVector3TransformNormal(vLook, RotationMatrix);

    Set_State(CTransform::STATE_RIGHT, vRight);
    Set_State(CTransform::STATE_UP, vUp);
    Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
    _matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

    _vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * Get_Scaled().x;
    _vector		vUp    = XMVectorSet(0.f, 1.f, 0.f, 0.f) * Get_Scaled().y;
    _vector		vLook  = XMVectorSet(0.f, 0.f, 1.f, 0.f) * Get_Scaled().z;

    vRight = XMVector3TransformNormal(vRight, RotationMatrix);
    vUp    = XMVector3TransformNormal(vUp, RotationMatrix);
    vLook  = XMVector3TransformNormal(vLook, RotationMatrix);

    Set_State(CTransform::STATE_RIGHT, vRight);
    Set_State(CTransform::STATE_UP, vUp);
    Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Rotation_Add(_fvector vAxis, _float fRadian)
{
    _matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

    _vector		vLook  = XMVector3Normalize(XMVector3TransformNormal(Get_State(CTransform::STATE_LOOK), RotationMatrix));
    _vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
    _vector		vUp    = XMVector3Normalize(XMVector3Cross(vLook, vRight));

    _float3		vScale = Get_Scaled();

    Set_State(CTransform::STATE_RIGHT, XMVectorSetW(XMVector3Normalize(vRight) * vScale.x, 0.f));
    Set_State(CTransform::STATE_UP   , XMVectorSetW(XMVector3Normalize(vUp)    * vScale.y, 0.f));
    Set_State(CTransform::STATE_LOOK , XMVectorSetW(XMVector3Normalize(vLook)  * vScale.z, 0.f));
}

void CTransform::Rotation_Quaternion(_fvector vQuaternion)
{
    _float3     vScale = Get_Scaled();

    _vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * Get_Scaled().x;
    _vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * Get_Scaled().y;
    _vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * Get_Scaled().z;

    _matrix     RotationMatrix = XMMatrixRotationQuaternion(vQuaternion);

    vRight = XMVector3TransformNormal(vRight, RotationMatrix);
    vUp = XMVector3TransformNormal(vUp, RotationMatrix);
    vLook = XMVector3TransformNormal(vLook, RotationMatrix);

    vRight = XMVector3Normalize(vRight);
    vUp = XMVector3Normalize(vUp);
    vLook = XMVector3Normalize(vLook);

    Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
    Set_State(CTransform::STATE_UP, vUp * vScale.y);
    Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
}

void CTransform::Rotation_PitchYawRoll(_float3 vRadian)
{
    _matrix matPitchYawRoll = XMMatrixRotationRollPitchYaw(vRadian.x, vRadian.y, vRadian.z);
    _vector vPosition = Get_Position();
    _matrix WorldMatrix = SMath::Get_ScaleMatrix(XMLoadFloat4x4(&m_WorldMatrix)) * matPitchYawRoll;

    WorldMatrix.r[3] = vPosition;
    XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

void CTransform::LookAt(_fvector vTargetPos)
{
    _vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

    _vector		vLook  = XMVector3Normalize(vTargetPos - vPosition);
    _vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
    _vector		vUp    = XMVector3Normalize(XMVector3Cross(vLook, vRight));

    _float3		vScale = Get_Scaled();

    Set_State(CTransform::STATE_RIGHT, XMVectorSetW(XMVector3Normalize(vRight) * vScale.x, 0.f));
    Set_State(CTransform::STATE_UP   , XMVectorSetW(XMVector3Normalize(vUp)    * vScale.y, 0.f));
    Set_State(CTransform::STATE_LOOK , XMVectorSetW(XMVector3Normalize(vLook)  * vScale.z, 0.f));
}

void CTransform::LookAt2D(_fvector vTargetPos)
{
    if (XMVectorGetX(XMVector3Length(vTargetPos)) <= DBL_EPSILON)
        DEBUG_ASSERT;
    _vector     vTargetPosFromVec4 = vTargetPos;
    vTargetPosFromVec4.m128_f32[3] = 1.f;
    _vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

    vPosition.m128_f32[1] = vTargetPosFromVec4.m128_f32[1];

    _vector		vLook = XMVector3Normalize(vTargetPosFromVec4 - vPosition);

    _vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));

    _vector		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

    _float3		vScale = Get_Scaled();

    Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
    Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
    Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);

}

void CTransform::LookAt_Right(_fvector vTargetRight)
{
    _vector vRight  = XMVector3Normalize(XMVectorSetW(vTargetRight, 0.f));
    _vector vLook   = XMVector3Normalize(XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f)));
    _vector vUp     = XMVector3Normalize(XMVector3Cross(vLook, vRight));
    _float3	vScale  = Get_Scaled();

    Set_State(CTransform::STATE_RIGHT, (vRight * vScale.x));
    Set_State(CTransform::STATE_UP   , (vUp    * vScale.y));
    Set_State(CTransform::STATE_LOOK , (vLook  * vScale.z));
}

void CTransform::Set_Look(_fvector vLook)
{

    _vector     vNormalLook = XMVector3Normalize(vLook);
    _vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vNormalLook);

    _vector		vUp = XMVector3Cross(vNormalLook, vRight);

    _float3		vScale = Get_Scaled();

    Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
    Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
    Set_State(CTransform::STATE_LOOK, vNormalLook * vScale.z);

}

void CTransform::Set_Look2D(_fvector vLook)
{
    _vector     vNormalLook = vLook;
    vNormalLook.m128_f32[1] = 0.f;

    vNormalLook = XMVector3Normalize(vNormalLook);

    _vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vNormalLook);

    _vector		vUp = XMVector3Cross(vNormalLook, vRight);

    _float3		vScale = Get_Scaled();

    Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
    Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
    Set_State(CTransform::STATE_LOOK, vNormalLook * vScale.z);

}

void CTransform::Chase(_fvector vTargetPos, _float fTimeDelta)
{
    _vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

    _vector		vMoveDir = vTargetPos - vPosition;

    vPosition += XMVector3Normalize(vMoveDir) /* 속도:5.f */ * fTimeDelta;

    Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

void CTransform::Write_Json(json& Out_Json)
{
    Out_Json.clear();

    Out_Json.emplace("Transform", m_WorldMatrix.m);
}

void CTransform::Load_FromJson(const json& In_Json)
{
    if (In_Json.find("Transform") == In_Json.end())
    {
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m_WorldMatrix.m[i][j] = In_Json["Transform"][i][j];
        }
    }

}

void CTransform::OnDestroy()
{
    UNUSE_START(CTransform);
    m_pOwner.lock()->CallBack_Bind_SRV -= bind(&CTransform::Set_ShaderResource, this, placeholders::_1, "g_WorldMatrix");
}

void CTransform::Free()
{
}
