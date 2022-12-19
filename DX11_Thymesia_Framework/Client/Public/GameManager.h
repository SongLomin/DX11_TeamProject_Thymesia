#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "Status_Player.h"

BEGIN(Engine)
class CGameObject;
class CCamera;
class CModel;
class CTransform;
END

BEGIN(Client)

class CCamera_Target;
class CPlayer;
class CEffectGroup;
class CSwapPlayerUI;

//Player_UI
class CPlayer_MPBar;
class CPlayer_HPBar;
class CHUD_Player_Memory;
class CPlayer_PotionUI;
class CPlayer_FeatherUI;
class CStatus_Player;

class CMonster;
class CInteraction_CheckPoint;
class CUI_Cursor;
class CLight_Prop;
class CItemPopup_Queue;

class CGameManager :
    public CBase
{
    DECLARE_SINGLETON(CGameManager)

public:
    void LateTick(_float fTimeDelta);

public:
    void Set_GameState(const GAME_STATE& In_eState);


public:
    void Register_Layer(const OBJECT_LAYER& In_Layer, weak_ptr<CGameObject> In_GameObject);
    void Remove_Layer(const OBJECT_LAYER& In_Layer, weak_ptr<CGameObject> In_GameObject);

    void Enable_Layer(const OBJECT_LAYER& In_Layer);
    void Disable_Layer(const OBJECT_LAYER& In_Layer);

    list<weak_ptr<CGameObject>> Get_Layer(const OBJECT_LAYER& In_Layer);
    list<weak_ptr<CGameObject>> Get_LayerSelectEnable(const OBJECT_LAYER& In_Layer);
    void Set_CurrentPlayer(weak_ptr<CPlayer> In_pPlayer);

    weak_ptr<CStatus_Player>    Get_CurrentPlayer_Status();
    weak_ptr<CPlayer>           Get_CurrentPlayer();

#pragma region Created By. ������
public: // For. ON / OFF Eye Effect
    void Store_EffectIndex(const char* szEffectName, const _uint iEffectIndex)
    {
        m_StoredEffects.emplace(szEffectName, iEffectIndex);
    }
    const _uint Get_EffectIndex(const char* szEffectName)
    {
        return m_StoredEffects[szEffectName];
    }

private:
	std::unordered_map<const char*, _uint> m_StoredEffects;
#ifdef _DEBUG
public: // For. Cloning Particle
    void Store_ParticleInfo(EFFECTPARTICLE_DESC tParticleDesc, std::string strBoneName)
    {
        m_tParticleDesc = tParticleDesc;
        if (!strBoneName.empty())
            m_strBoneName = strBoneName;
    }

    const EFFECTPARTICLE_DESC Get_StoredParticleInfo()
    {
        return m_tParticleDesc;
    }

    const std::string Get_BoneName()
    {
        return m_strBoneName;
    }

private:
    EFFECTPARTICLE_DESC m_tParticleDesc;
    std::string m_strBoneName;

public: // For. Cloning Effect Mesh
    void Store_EffectMeshInfo(EFFECTMESH_DESC tEffectMeshDesc)
    {
        m_tEffectMeshDesc = tEffectMeshDesc;
    }
    const EFFECTMESH_DESC Get_StoredEffectMeshInfo()
    {
        return m_tEffectMeshDesc;
    }

private:
    EFFECTMESH_DESC m_tEffectMeshDesc;

#pragma endregion // Created By. ������
#endif // _DEBUG

public:
    void Set_TargetCamera(weak_ptr<CCamera_Target> In_TargetCamera);
    weak_ptr<CCamera_Target> Get_TargetCamera();
    void Add_Shaking(_vector vShakingDir, _float fRatio, _float fShakingTime, _float fFrequency = 2.f, _float fDecreaseRatio = 0.5f);
    //void Set_TargetForTargetCamera(weak_ptr<CGameObject> In_TargetGameObject);
    void Focus_Monster();
    void Release_Focus();
    weak_ptr<CMonster> Get_TargetMonster();
    weak_ptr<CCamera_Target> Get_Target() { return m_pTargetCamera; }

private:
    weak_ptr<CMonster> Forced_SearchNearTargetMonster();

public:
    void Register_EffectGroup(const string& In_szEffectGroupName, weak_ptr<CEffectGroup> In_pEffectGroup);
    // �ش� ����Ʈ �׷��� �ε����� ��ȯ�մϴ�. (�޸� Ǯ)
    // ����Ʈ�� UnUse�Ҷ� �ε����� �ʿ�� �մϴ�.

    _uint Use_EffectGroupFromHash(const _hashcode& In_EffectGroupNameFromHash, weak_ptr<CTransform> pParentTransformCom, const _uint& In_iTimeScaleLayer = -1);
    _uint Use_EffectGroup(const string& In_szEffectGroupName, weak_ptr<CTransform> pParentTransformCom, const _uint& In_iTimeScaleLayer = -1);
    void UnUse_EffectGroup(const string& In_szEffectGroupName, const _uint& In_EffectGroupIndex);

private:
    void Enable_WeaponFromEvent(weak_ptr<CTransform> pParentTransformCom, const _bool In_bEnable);

public:
    void Load_AllKeyEventFromJson();
    // Ű �̺�Ʈ�� �ִ� �ִϸ��̼ǿ��Ը� �Լ��� ���ε��մϴ�. 
    // ���� �ִϸ��̼� Ű �̺�Ʈ�� ������ Bind_Function�� ȣ���մϴ�. 
    void Bind_KeyEvent(const string& In_szModelName, weak_ptr<CModel> ModelCom, function<void(const _uint&)> Bind_Function);

    // �ش� Ű�� �̺�Ʈ�� �ִ��� Ȯ�� �� �ִٸ� �̺�Ʈ�� ȣ���մϴ�.
    void Active_KeyEvent(const weak_ptr<CModel> In_ModelCom, const weak_ptr<CTransform> In_TransformCom, const _uint& In_iKeyIndex, const _uint& In_iTimeScaleLayer = -1);
    void Start_Cinematic(weak_ptr<CModel> _pModel, const _char* pBoneName,_matrix& OffSetMatrix, CINEMATIC_TYPE iCinematicType);
    void End_Cinematic();

    void Activate_Zoom(_float fRatio, _float fZoomTime, EASING_TYPE eZoomLerpFunc);
    void Deactivate_Zoom(_float fZoomTime, EASING_TYPE eZoomLerpFunc);
    _vector Get_PlayerPos();

public:
    void Registration_CheckPoint(weak_ptr<CInteraction_CheckPoint> In_CheckPoint);
    HRESULT Respawn_LastCheckPoint(_float4* Out_RespawnPos);
    virtual void OnLevelExit() override;

private:
    void Start_Peace();
    void Start_Battle();
    void Start_BattleStop();
    void Start_BattleEnd();

    void Stop_Peace();
    void Stop_Battle();
    void Stop_BattleStop();
    void Stop_BattleEnd();

    _bool Check_StageEnd();

public:
    void Call_BattleEndFadeOut();
    void Call_BattleEndFadeIn();

public:
    void Change_NextLevel(void* pArg);

    /*
    ������ ����ٿ� �Լ�����.
    
    ������� �Լ���?
    ���ĵ� ��ǻ�� ȯ�� �� ������ ���������� �ϼ��ϱ� ����
    �η��� ������ ����� �Լ� �� �������� ��Ƴ��� ����.
    ���߿� ������ ����(���ش�)��Ű����.
    */
public:// For UI;
    POINT   Get_MousePoint();
    void    Set_PlayerStatusDesc(void* pArg);
    CStatus_Player::PLAYERDESC&   Get_PlayerStatusDesc()
    {
        return m_tPlayerDesc;
    }

public:
    _uint   m_iTestTalent = 2;
    CStatus_Player::PLAYERDESC m_tPlayerDesc;


public:
    void    SetCursor(weak_ptr<CUI_Cursor> pCursor) { m_pCursor = pCursor; }
    void    EnableCursor();
    void    DisableCursor();


public:
    void   CreatePopupQueue();
    void   Add_Popup(ITEM_NAME eItemName);
private:
    weak_ptr< CUI_Cursor> m_pCursor;

    shared_ptr<CItemPopup_Queue> m_pItemPopupQueue;


public:
    void  Registration_Section(_uint In_iSection, weak_ptr<CGameObject> In_pObj);
    void  Activate_Section(_uint In_iSection, _bool In_bState);

    void  Registration_SectionLight(_uint In_iSection, weak_ptr<CLight_Prop> In_pObj);
    void  Activate_SectionLight(_uint In_iSection, _bool In_bState);

public:
    FDelegate<>                 CallBack_ChangePlayer;
    FDelegate<>                 CallBack_FocusInMonster;
    FDelegate<>                 CallBack_FocusOutMonster;

private:
    list<weak_ptr<CGameObject>> m_pLayers[(_uint)OBJECT_LAYER::LAYER_END];
    map<_hashcode, list<weak_ptr<CEffectGroup>>> m_pEffectGroups;
    weak_ptr<CCamera_Target>    m_pTargetCamera;
    weak_ptr<CCamera>           m_pCurrentCamera;
    weak_ptr<CPlayer>           m_pCurrentPlayer;
    weak_ptr<CMonster>          m_pTargetMonster;
    GAME_STATE                  m_eCurrentState = GAME_STATE::STATE_END;

private:
    map<_hashcode, map<_int, map<_int, KEYFRAME_EVENT>>>         m_KeyEvents;
    typedef map<_int, KEYFRAME_EVENT>                            KEYEVENT;
    typedef map<_int, KEYEVENT>                                  ANIM_MAP;

private:
    typedef map<_int, list<weak_ptr<CGameObject>>> SECTION_OBJ;
    typedef map<_int, list<weak_ptr<CLight_Prop>>> SECTION_LIGHT;

    SECTION_OBJ                         m_SectionObejects;
    SECTION_LIGHT                       m_SectionLights;
    weak_ptr<CInteraction_CheckPoint>   m_pCurSavePoint;

private:
    _int                                                        m_iMonsterCount   = 0;

protected:
    void Free();
};

END