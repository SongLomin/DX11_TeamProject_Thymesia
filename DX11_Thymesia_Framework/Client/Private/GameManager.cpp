#include "stdafx.h"
#include "GameManager.h"
#include "Client_GameObjects.h"
#include "Client_Components.h"
#include "Player.h"
#include "ClientLevel.h"

IMPLEMENT_SINGLETON(CGameManager)

void CGameManager::LateTick(_float fTimeDelta)
{
	//지워진 객체 정리
	for (_uint i = 0; i < (_uint)OBJECT_LAYER::LAYER_END; i++)
	{
		for (auto iter = m_pLayers[i].begin(); iter != m_pLayers[i].end();)
		{
			if (!(*iter).lock().get())
			{
				iter = m_pLayers[i].erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	for (auto& elem_List : m_pEffectGroups)
	{
		for (auto iter_EffectGroup = elem_List.second.begin(); iter_EffectGroup != elem_List.second.end();)
		{
			if (!(*iter_EffectGroup).lock().get())
			{
				iter_EffectGroup = elem_List.second.erase(iter_EffectGroup);
			}
			else
			{
				++iter_EffectGroup;
			}
		}
	}

	if (Check_StageEnd())
	{
		Set_GameState(GAME_STATE::BATTLE_END);
	}
}

void CGameManager::Set_GameState(const GAME_STATE& In_eState)
{
	switch (m_eCurrentState)
	{
	case Client::GAME_STATE::PEACE:
		Stop_Peace();
		break;
	case Client::GAME_STATE::BATTLE:
		Stop_Battle();
		break;
	case Client::GAME_STATE::BATTLE_STOP:
		Stop_BattleStop();
		break;
	case Client::GAME_STATE::BATTLE_END:
		Stop_BattleEnd();
		break;
	case Client::GAME_STATE::STATE_END:

		break;
	default:

		break;
	}

	m_eCurrentState = In_eState;

	switch (m_eCurrentState)
	{
	case Client::GAME_STATE::PEACE:
		Start_Peace();
		break;
	case Client::GAME_STATE::BATTLE:
		Start_Battle();
		break;
	case Client::GAME_STATE::BATTLE_STOP:
		Start_BattleStop();
		break;
	case Client::GAME_STATE::BATTLE_END:
		Start_BattleEnd();
		break;
	case Client::GAME_STATE::STATE_END:

		break;
	default:

		break;
	}

}

void CGameManager::Register_Layer(const OBJECT_LAYER& In_Layer, weak_ptr<CGameObject> In_GameObject)
{
	if (OBJECT_LAYER::MONSTER == In_Layer)
	{
		++m_iMonsterCount;
		GAMEINSTANCE->Get_CurrentLevel().lock()->OnEventMessage((_uint)EVENT_TYPE::ON_MONSTERBEGIN);
	}

	m_pLayers[(_uint)In_Layer].push_back(In_GameObject);
}

void CGameManager::Remove_Layer(const OBJECT_LAYER& In_Layer, weak_ptr<CGameObject> In_GameObject)
{

	for (auto iter = m_pLayers[(_uint)In_Layer].begin(); iter != m_pLayers[(_uint)In_Layer].end();)
	{
		if ((*iter).lock()->Get_GameObjectIndex() == In_GameObject.lock()->Get_GameObjectIndex())
		{
			if (OBJECT_LAYER::MONSTER == In_Layer)
			{
				--m_iMonsterCount;
				GAMEINSTANCE->Get_CurrentLevel().lock()->OnEventMessage((_uint)EVENT_TYPE::ON_MONSTERDEAD);
			}

			m_pLayers[(_uint)In_Layer].erase(iter);
			break;
		}

		iter++;
	}
}

list<weak_ptr<CGameObject>> CGameManager::Get_Layer(const OBJECT_LAYER& In_Layer)
{
	return m_pLayers[(_uint)In_Layer];
}

list<weak_ptr<CGameObject>> CGameManager::Get_LayerSelectEnable(const OBJECT_LAYER& In_Layer)
{
	list<weak_ptr<CGameObject>> SelectedLayer;
	
	for (auto& elem_GameObject : m_pLayers[(_uint)In_Layer])
	{
		if (elem_GameObject.lock()->Get_Enable())
		{
			SelectedLayer.push_back(elem_GameObject);
		}
	}

	return SelectedLayer;
}

void CGameManager::Set_CurrentPlayer(weak_ptr<CPlayer> In_pPlayer)
{
	m_pCurrentPlayer = In_pPlayer;
	
	if (m_pTargetCamera.lock().get())
	{
		m_pTargetCamera.lock()->Change_Target();
	}

	CallBack_ChangePlayer();
}

weak_ptr<CPlayer> CGameManager::Get_CurrentPlayer()
{
	return m_pCurrentPlayer;
}

void CGameManager::Register_Player_HPBar(weak_ptr<CPlayer_HPBar> pHPBar)
{
	m_pPlayer_HPBar = pHPBar;
}

void CGameManager::Register_Player_MPBar(weak_ptr<CPlayer_MPBar> pMPBar)
{
	m_pPlayer_MPBar = pMPBar;
}

void CGameManager::Register_Player_Memory(weak_ptr<CPlayer_Memory> pMemory)
{
	m_pPlayer_Memory = pMemory;
}

void CGameManager::Register_Player_HUD_Potion(weak_ptr<CPlayer_PotionUI> pHUD_Potion)
{
}

void CGameManager::Register_Player_HUD_Feather(weak_ptr<CPlayer_FeatherUI> pHUD_Feather)
{
}

void CGameManager::Set_TargetCamera(weak_ptr<CCamera_Target> In_TargetCamera)
{
	m_pTargetCamera = In_TargetCamera;
	m_pCurrentCamera = In_TargetCamera;
}

weak_ptr<CCamera_Target> CGameManager::Get_TargetCamera()
{
	return m_pTargetCamera;
}

void CGameManager::Add_Shaking(const SHAKE_DIRECTION& In_eState, const _float& In_fPower, const _float& In_fTime)
{
	if (m_pTargetCamera.lock().get() != m_pCurrentCamera.lock().get())
	{
		return;
	}

	if (!m_pTargetCamera.lock())
		DEBUG_ASSERT;

	//m_pTargetCamera.lock()->Add_Shaking(In_eState, In_fPower, In_fTime);
	

}

void CGameManager::Focus_Monster()
{
//타겟 찾고
//
	weak_ptr<CGameObject> pTargetMonster = Get_Layer(OBJECT_LAYER::MONSTER).front();

	m_pTargetCamera.lock()->Focus_Monster(pTargetMonster);
}

void CGameManager::Release_Focus()
{
	m_pTargetCamera.lock()->Release_Focus();
}

void CGameManager::Find_Target()
{
}

void CGameManager::Register_EffectGroup(const string& In_szEffectGroupName, weak_ptr<CEffectGroup> In_pEffectGroup)
{
	_size_t hashcode = hash<string>()(In_szEffectGroupName);

	auto iter = m_pEffectGroups.find(hashcode);

	if (m_pEffectGroups.end() == iter)
	{
		m_pEffectGroups.emplace(hashcode, list<weak_ptr<CEffectGroup>>());
	}

	m_pEffectGroups[hashcode].push_back(In_pEffectGroup);

}

_uint CGameManager::Use_EffectGroupFromHash(const _hashcode& In_EffectGroupNameFromHash, weak_ptr<CTransform> pParentTransformCom, const _uint& In_iTimeScaleLayer)
{
	_uint iIndex = 0;

	// 만약 이펙트가 나오지 않는다면 이 예외처리를 지우고 터지는지 확인해볼 것.
	if (m_pEffectGroups.end() == m_pEffectGroups.find(In_EffectGroupNameFromHash))
	{
		return -1;
	}


	for (auto& elem : m_pEffectGroups[In_EffectGroupNameFromHash])
	{
		if (elem.lock()->Use_EffectGroup(pParentTransformCom, In_iTimeScaleLayer))
		{
			return iIndex;
		}

		++iIndex;
	}

	if (0 == iIndex)
	{
		return -1;
	}

	// 여기서 터진다면 이펙트그룹을 추가하지 않은 것인지 확인.
	// 등록된 모든 이펙트 그룹이 사용 중이라면
	m_pEffectGroups[In_EffectGroupNameFromHash].front().lock()->Clone_EffectGroup().lock()->Use_EffectGroup(pParentTransformCom, In_iTimeScaleLayer);
	cout << "CloneEffect" << endl;

	return iIndex;
}

_uint CGameManager::Use_EffectGroup(const string& In_szEffectGroupName, weak_ptr<CTransform> pParentTransformCom, const _uint& In_iTimeScaleLayer)
{
	_hashcode hashcode = hash<string>()(In_szEffectGroupName);
	
	cout << "Use EffectGroup: " << In_szEffectGroupName << endl;


	return Use_EffectGroupFromHash(hashcode, pParentTransformCom, In_iTimeScaleLayer);
}

void CGameManager::UnUse_EffectGroup(const string& In_szEffectGroupName, const _uint& In_EffectGroupIndex)
{
	_hashcode hashcode = hash<string>()(In_szEffectGroupName);
	_uint iIndex = 0;

	cout << "Init UnUse EffectGroup: " << In_szEffectGroupName << endl;

	for (auto& elem : m_pEffectGroups[hashcode])
	{
		if (In_EffectGroupIndex == iIndex)
		{
			cout << "UnUse EffectGroup: " << In_szEffectGroupName << endl;
			if (elem.lock())
				elem.lock()->UnUse_EffectGroup();
		}

		++iIndex;
	}
}

void CGameManager::Load_AllKeyEventFromJson()
{
	fs::directory_iterator itr("..\\Bin\\KeyEventData");

	json KeyEventJson;

	while (itr != fs::end(itr)) {
		const fs::directory_entry& entry = *itr;

		if (FAILED(CJson_Utility::Load_Json(entry.path().string().c_str(), KeyEventJson)))
		{
			itr++;
			continue;
		}

		string szFileName = entry.path().filename().string();
		szFileName = szFileName.substr(0, szFileName.size() - 5);

		_hashcode szFileNameToHash = hash<string>()(szFileName);

		m_KeyEvents.emplace(szFileNameToHash, ANIM_MAP());

		// 애니메이션 인덱스 리스트
		for (_size_t i = 0; i < KeyEventJson["AnimationIndex"].size(); ++i)
		{
			if (KeyEventJson["AnimationIndex"][i].empty())
			{
				continue;
			}

			
			m_KeyEvents[szFileNameToHash].emplace(i, KEYEVENT());

			// 애니메이션 키 인덱스 리스트
			for (_size_t j = 0; j < KeyEventJson["AnimationIndex"][i].size(); ++j)
			{
				if (KeyEventJson["AnimationIndex"][i][j].empty())
				{
					continue;
				}
				m_KeyEvents[szFileNameToHash][i].emplace(j, KEYFRAME_EVENT());

				// 이펙트그룹 순회

				for (auto& EffectGroupName : KeyEventJson["AnimationIndex"][i][j]["EffectName"])
				{
					if (EffectGroupName.empty())
					{
						// 이펙트 그룹 이름 리스트의 중간이 비어있으면 안된다.
						DEBUG_ASSERT;
					}

					string szEffectGroupName = EffectGroupName;

					m_KeyEvents[szFileNameToHash][i][j].EffectGroups.push_back(hash<string>()(szEffectGroupName));
				}

			}
		}

		itr++;
	}

}

void CGameManager::Bind_KeyEvent(const string& In_szModelName, weak_ptr<CModel> ModelCom, function<void(const _uint&)> Bind_Function)
{
	_hashcode ModelNameToHash = hash<string>()(In_szModelName);

	auto Model_iter = m_KeyEvents.find(ModelNameToHash);

	if (m_KeyEvents.end() == Model_iter)
	{
		return;
	}

	for (auto& KeyAnimation : Model_iter->second)
	{
		ModelCom.lock()->Get_AnimationFromIndex(KeyAnimation.first).lock()->CallBack_NextChannelKey += Bind_Function;
	}
}

void CGameManager::Active_KeyEvent(const weak_ptr<CModel> In_ModelCom, const weak_ptr<CTransform> In_TransformCom, 
	const _uint& In_iKeyIndex, const _uint& In_iTimeScaleLayer)
{
	string szModelName = In_ModelCom.lock()->Get_ModelKey();

	_hashcode szModelNameToHash = hash<string>()(szModelName);

	auto Model_iter = m_KeyEvents.find(szModelNameToHash);

	if (m_KeyEvents.end() == Model_iter)
	{
		return;
	}

	auto Anim_iter = Model_iter->second.find(In_ModelCom.lock()->Get_CurrentAnimationIndex());

	if (Model_iter->second.end() == Anim_iter)
	{
		// 등록된 애니메이션 컴포넌트에서 호출하기 때문에 해당 번호 json이 비어있으면 문제가 있는 것.
		DEBUG_ASSERT;
	}

	auto Key_iter = Anim_iter->second.find(In_iKeyIndex);

	if (Anim_iter->second.end() == Key_iter)
	{
		return;
	}

	// 이펙트 발동
	for (auto& elem : Key_iter->second.EffectGroups)
	{
		Use_EffectGroupFromHash(elem, In_TransformCom, In_iTimeScaleLayer);
	}

}

void CGameManager::Start_Cinematic(weak_ptr<CModel> _pModel, const _char* pBoneName)
{
	m_pTargetCamera.lock()->Start_Cinematic(_pModel, pBoneName);
}

void CGameManager::End_Cinematic()
{
	m_pTargetCamera.lock()->End_Cinematic();
}



_bool CGameManager::Is_OpenHyperSpace()
{
	return Get_Layer(OBJECT_LAYER::HYPERSPACE).front().lock()->Get_Enable();
}

void CGameManager::Start_Peace()
{
	for (auto& elem : m_pLayers[(_uint)OBJECT_LAYER::BATTLEUI])
	{
		elem.lock()->Set_Enable(false);
	}

}

void CGameManager::Start_Battle()
{
	for (auto& elem : m_pLayers[(_uint)OBJECT_LAYER::BATTLEUI])
	{
		elem.lock()->Set_Enable(true);
	}
}

void CGameManager::Start_BattleStop()
{

}

void CGameManager::Start_BattleEnd()
{
	

	weak_ptr<CFadeMask> pFadeMask = Weak_Cast<CFadeMask>(Get_Layer(OBJECT_LAYER::FADEMASK).front());

	if (!pFadeMask.lock())
	{
		DEBUG_ASSERT;
	}

	FaderDesc tFaderDesc;
	tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
	tFaderDesc.eLinearType = LINEAR_TYPE::POW;
	tFaderDesc.fFadeMaxTime = 2.f;
	tFaderDesc.fDelayTime = 0.5f;
	tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

	pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);

	pFadeMask.lock()->CallBack_FadeEnd += bind(&CGameManager::Call_BattleEndFadeOut, this);

}

void CGameManager::Stop_Peace()
{

}

void CGameManager::Stop_Battle()
{
	for (auto& elem : m_pLayers[(_uint)OBJECT_LAYER::BATTLEUI])
	{
		elem.lock()->Set_Enable(false);
	}
}

void CGameManager::Stop_BattleStop()
{

}

void CGameManager::Stop_BattleEnd()
{
}

_bool CGameManager::Check_StageEnd()
{
	if (KEY_INPUT(KEY::DELETEKEY, KEY_STATE::TAP))
	{
		return true;
	}

	return false;
}

void CGameManager::Call_BattleEndFadeOut()
{
	weak_ptr<CFadeMask> pFadeMask = Weak_Cast<CFadeMask>(Get_Layer(OBJECT_LAYER::FADEMASK).front());

	if (!pFadeMask.lock())
	{
		DEBUG_ASSERT;
	}

	FaderDesc tFaderDesc;
	tFaderDesc.eFaderType = FADER_TYPE::FADER_IN;
	tFaderDesc.eLinearType = LINEAR_TYPE::SQRT;
	tFaderDesc.fFadeMaxTime = 2.f;
	tFaderDesc.fDelayTime = 0.5f;
	tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

	pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);

	pFadeMask.lock()->CallBack_FadeEnd += bind(&CGameManager::Call_BattleEndFadeIn, this);

}

void CGameManager::Call_BattleEndFadeIn()
{
	if(m_pCurrentPlayer.lock())
		m_pCurrentPlayer.lock()->OnBattleEnd();
}

void CGameManager::Change_NextLevel(void* pArg)
{
	weak_ptr<CClientLevel> pCurrentLevel = Weak_Cast<CClientLevel>(GAMEINSTANCE->Get_CurrentLevel().lock());

	if (!pCurrentLevel.lock())
	{
		DEBUG_ASSERT;
	}

	pCurrentLevel.lock()->Change_NextLevel(nullptr);
}

//void CGameManager::Set_TargetForTargetCamera(weak_ptr<CGameObject> In_TargetGameObject)
//{
//	//타겟 카메라 객체가 없는데 타겟을 넘겨주려고 함.
//	if (!m_pTargetCamera.lock().get())
//		DEBUG_ASSERT;
//
//	m_pTargetCamera.lock()->Set_Target(In_TargetGameObject);
//}


void CGameManager::Free()
{
}
