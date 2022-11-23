#pragma once
#include "Base.h"
#include "Client_Defines.h"

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
class CPlayer_Memory;
class CPlayer_PotionUI;
class CPlayer_FeatherUI;


class CMonster;

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

    list<weak_ptr<CGameObject>> Get_Layer(const OBJECT_LAYER& In_Layer);
    list<weak_ptr<CGameObject>> Get_LayerSelectEnable(const OBJECT_LAYER& In_Layer);
    void Set_CurrentPlayer(weak_ptr<CPlayer> In_pPlayer);
    weak_ptr<CPlayer> Get_CurrentPlayer();

public:
   
public:
    void Set_TargetCamera(weak_ptr<CCamera_Target> In_TargetCamera);
    weak_ptr<CCamera_Target> Get_TargetCamera();
    void Add_Shaking(const SHAKE_DIRECTION& In_eState , const _float& In_fPower, const _float& In_fTime);
    //void Set_TargetForTargetCamera(weak_ptr<CGameObject> In_TargetGameObject);
    void Focus_Monster();
    void Release_Focus();
    weak_ptr<CMonster> Get_TargetMonster();

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
    void Start_Cinematic(weak_ptr<CModel> _pModel, const _char* pBoneName);
    void End_Cinematic();
    
public:
    _bool Is_OpenHyperSpace();

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
    FDelegate<>                 CallBack_ChangePlayer;

private:
    list<weak_ptr<CGameObject>> m_pLayers[(_uint)OBJECT_LAYER::LAYER_END];
    map<_hashcode, list<weak_ptr<CEffectGroup>>> m_pEffectGroups;
    weak_ptr<CCamera_Target>    m_pTargetCamera;
    weak_ptr<CCamera>           m_pCurrentCamera;
    weak_ptr<CPlayer>           m_pCurrentPlayer;
    weak_ptr<CMonster>          m_pTargetMonster;
    GAME_STATE                  m_eCurrentState = GAME_STATE::STATE_END;


    //For. PlayerUI
private:


private:
    map<_hashcode, map<_int, map<_int, KEYFRAME_EVENT>>>         m_KeyEvents;
    typedef map<_int, KEYFRAME_EVENT>                            KEYEVENT;
    typedef map<_int, KEYEVENT>                                   ANIM_MAP;

private:
    _int                                                        m_iMonsterCount = 0;

protected:
    void Free();
};

END