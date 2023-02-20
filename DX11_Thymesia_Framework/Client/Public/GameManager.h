#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "Status_Player.h"

BEGIN(Engine)
class CGameObject;
class CCamera;
class CModel;
class CTransform;
class CSubThread_Pool;
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
class CInteraction_DeadSpot;
class CSection_Eventer;
class CFog;
class CInteriorProp;
class CWater;

class CGameManager :
    public CBase
{
    DECLARE_SINGLETON(CGameManager)

public:
    void Initialize(_uint iNumThread);

public:
    void LateTick(_float fTimeDelta);

public:
    shared_ptr<CSubThread_Pool> Get_ClientThread();

public:
    void Set_GameState(const GAME_STATE& In_eState);


public:
    void OnEventMessageForLayer(const OBJECT_LAYER& In_Layer, EVENT_TYPE iArg);

    void Register_Layer(const OBJECT_LAYER& In_Layer, weak_ptr<CGameObject> In_GameObject);
    void Remove_Layer(const OBJECT_LAYER& In_Layer, weak_ptr<CGameObject> In_GameObject);

    void Enable_Layer(const OBJECT_LAYER& In_Layer);
    void Disable_Layer(const OBJECT_LAYER& In_Layer);

    list<weak_ptr<CGameObject>> Get_Layer(const OBJECT_LAYER& In_Layer);
    list<weak_ptr<CGameObject>> Get_LayerSelectEnable(const OBJECT_LAYER& In_Layer);
    void Set_CurrentPlayer(weak_ptr<CPlayer> In_pPlayer);

    weak_ptr<CStatus_Player>    Get_CurrentPlayer_Status();
    weak_ptr<CPlayer>           Get_CurrentPlayer();

#pragma region Created By. 조성우
public: // For. ON / OFF Eye Effect
    void Store_EffectIndex(const char* szEffectName, const _uint iEffectIndex)
    {
        m_StoredEffects.emplace(szEffectName, iEffectIndex);
    }
    const _uint Get_StoredEffectIndex(const char* szEffectName)
    {
        _uint iIndex = m_StoredEffects[szEffectName];
        m_StoredEffects.erase(szEffectName);
        return iIndex;
    }

private:
	std::unordered_map<const char*, _uint> m_StoredEffects;
#ifdef _EFFECT_TOOL_
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

#pragma endregion // Created By. 조성우
#endif // _EFFECT_TOOL_

public:
    void Set_TargetCamera(weak_ptr<CCamera_Target> In_TargetCamera);
    weak_ptr<CCamera_Target> Get_TargetCamera();
    void Add_Shaking(_vector vShakingDir, _float fRatio, _float fShakingTime, _float fFrequency = 2.f, _float fDecreaseRatio = 0.5f);
    //void Set_TargetForTargetCamera(weak_ptr<CGameObject> In_TargetGameObject);
    void Focus_Monster();
    void Release_Focus();
    weak_ptr<CMonster> Get_TargetMonster();
    weak_ptr<CCamera_Target> Get_CameraTarget() { return m_pTargetCamera; }

private:
    weak_ptr<CMonster> Forced_SearchNearTargetMonster();

public:
    void Register_EffectGroup(const string& In_szEffectGroupName, weak_ptr<CEffectGroup> In_pEffectGroup);
    // 해당 이펙트 그룹의 인덱스를 반환합니다. (메모리 풀)
    // 이펙트를 UnUse할때 인덱스를 필요로 합니다.

    _uint Use_EffectGroupFromHash(const _hashcode& In_EffectGroupNameFromHash, weak_ptr<Engine::CTransform> pParentTransformCom, const _uint& In_iTimeScaleLayer = -1);
    _uint Use_EffectGroup(const string& In_szEffectGroupName, weak_ptr<Engine::CTransform> pParentTransformCom, const _uint& In_iTimeScaleLayer = -1);
    void UnUse_EffectGroup(const string& In_szEffectGroupName, const _uint& In_EffectGroupIndex);

private:
    void Enable_WeaponFromEvent(weak_ptr<Engine::CTransform> pParentTransformCom, const _bool In_bEnable);

public:
    void Load_AllKeyEventFromJson();
    // 키 이벤트가 있는 애니메이션에게만 함수를 바인드합니다.
    // 다음 애니메이션 키 이벤트가 들어오면 Bind_Function을 호출합니다.
    void Bind_KeyEvent(const string& In_szModelName, weak_ptr<CModel> ModelCom, function<void(const _uint&)> Bind_Function);
    void Unbind_KeyEvent(const string& In_szModelName, weak_ptr<CModel> ModelCom, function<void(const _uint&)> Bind_Function);

    // 해당 키에 이벤트가 있는지 확인 후 있다면 이벤트를 호출합니다.
    void Active_KeyEvent(const string& In_szKeyEventName, const weak_ptr<CModel> In_ModelCom, const weak_ptr<CTransform> In_TransformCom, const _uint& In_iKeyIndex, const _uint& In_iTimeScaleLayer = -1);
    void Start_Cinematic(weak_ptr<CModel> _pModel, const _char* pBoneName,_matrix& OffSetMatrix, CINEMATIC_TYPE iCinematicType);
    void End_Cinematic();

    void Activate_Zoom(_float fRatio, _float fZoomTime, EASING_TYPE eZoomLerpFunc, DIRECTION eMoveDir = DIRECTION::LOOK);
    void Deactivate_Zoom(_float fZoomTime, EASING_TYPE eZoomLerpFunc);
    _vector Get_PlayerPos();

public:
    void Registration_CheckPoint(weak_ptr<CInteraction_CheckPoint> In_CheckPoint);
    HRESULT Respawn_LastCheckPoint(_float4* Out_RespawnPos);
    void ResetWorld();
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

public:
    template <class T>
    weak_ptr<T>         GetGameObject_SafetyUseMemoryPool(_uint iLevelIndex)
    {
        weak_ptr<T> pTemplate = GAMEINSTANCE->Get_GameObject_UseMemoryPool<T>(iLevelIndex);

        if (!pTemplate.lock())
        {
            pTemplate = GAMEINSTANCE->Add_GameObject<T>(iLevelIndex);
        }
        return pTemplate;
    }


public:
    void  Registration_Section(_uint In_iSection, weak_ptr<CGameObject> In_pObj);
    void  Activate_Section(_uint In_iSection, EVENT_TYPE In_eEventType);
    void  Remove_Section(_uint In_iSection, weak_ptr<CGameObject> In_pObj);

    void  Registration_SectionLight(_uint In_iSection, weak_ptr<CLight_Prop> In_pObj);
    void  Activate_SectionLight(_uint In_iSection, EVENT_TYPE In_eEventType);
    void  Remove_SectionLight(_uint In_iSection, weak_ptr<CGameObject> In_pObj);

    void  Registration_OnlyResetObject(weak_ptr<CGameObject> In_pObj);

    void  Registration_Camera(string In_szTag, weak_ptr<CCamera> In_pCamera);
    void  Activate_Camera(string In_szTag);

    void  Registration_Fog(weak_ptr<CFog> In_pObj);
    void  Activate_Fog(_uint In_iFogIndex);

    void Set_PreLevel(LEVEL eLevel) { m_ePreLevel = eLevel; }
    LEVEL Get_PreLevel() { return m_ePreLevel; }

    void                    Set_MoveTargetCamera(_bool bTargetCamera);


public:
    void Register_Water(weak_ptr<CWater> pWater);
    void Add_WaterWave(_fvector In_vWorldPosition, const _float In_fVibrationScale, const _float In_fFreq, const _float In_fSpeed);

public: /* Change Game Options */
    void Change_Option(OPTION_TYPE eOption, _uint iAmount);


 public:
    FDelegate<>                 CallBack_ChangePlayer;
    FDelegate<>                 CallBack_FocusInMonster;
    FDelegate<>                 CallBack_FocusOutMonster;

private: /* For. SubThread Pool  */
    shared_ptr<CSubThread_Pool> m_pClientThread;

private:
    list<weak_ptr<CGameObject>> m_pLayers[(_uint)OBJECT_LAYER::LAYER_END];
    unordered_map<_hashcode, list<weak_ptr<CEffectGroup>>> m_pEffectGroups;
    weak_ptr<CCamera_Target>    m_pTargetCamera;
    weak_ptr<CCamera>           m_pCurrentCamera;
    weak_ptr<CPlayer>           m_pCurrentPlayer;
    weak_ptr<CMonster>          m_pTargetMonster;
    GAME_STATE                  m_eCurrentState = GAME_STATE::STATE_END;

private:
    typedef unordered_map<_int, KEYFRAME_EVENT>                             KEYEVENT;
    typedef unordered_map<_int, KEYEVENT>                                   ANIM_MAP;
    unordered_map<_hashcode, ANIM_MAP>                                      m_KeyEvents;

private:
    typedef map<_int, list<weak_ptr<CGameObject>>>         SECTION_OBJ;
    typedef map<_int, list<weak_ptr<CLight_Prop>>>         SECTION_LIGHT;
    typedef list<weak_ptr<CGameObject>>                    RESET_OBJ;
    typedef map<string, weak_ptr<CCamera>>                 SECTION_CAMERA;

    SECTION_OBJ                         m_SectionObejects;
    SECTION_LIGHT                       m_SectionLights;
    RESET_OBJ                           m_ResetObjects;
    SECTION_CAMERA                      m_SectionCamera;
    weak_ptr<CFog>                      m_FogObject;
    weak_ptr<CInteraction_CheckPoint>   m_pCurSavePoint;
    weak_ptr<CInteraction_DeadSpot>     m_pDeadSpot;
    weak_ptr<CWater>                    m_pWater;
    weak_ptr<CInteriorProp>             m_pInterior;
    _uint                               m_iPreEventSection;  
    LEVEL                               m_ePreLevel = LEVEL::LEVEL_END;

private:
    _int                                m_iMonsterCount   = 0;

private:

protected:
    void Free();
};

END

