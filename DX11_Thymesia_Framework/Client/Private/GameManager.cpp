#include "stdafx.h"
#include "GameManager.h"
#include "Client_GameObjects.h"
#include "Client_Components.h"
#include "Player.h"
#include "ClientLevel.h"
#include "Status_Player.h"


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

void CGameManager::Enable_Layer(const OBJECT_LAYER& In_Layer)
{
	list<weak_ptr<CGameObject>> pObjectLayer = m_pLayers[(_uint)In_Layer];

	for (auto& elem : pObjectLayer)
	{
		elem.lock()->Set_Enable(true);

	}

}

void CGameManager::Disable_Layer(const OBJECT_LAYER& In_Layer)
{
	list<weak_ptr<CGameObject>> pObjectLayer = m_pLayers[(_uint)In_Layer];

	for (auto& elem : pObjectLayer)
	{
		elem.lock()->Set_Enable(false);

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

weak_ptr<CStatus_Player> CGameManager::Get_CurrentPlayer_Status()
{
	weak_ptr<CPlayer> pPlayer;

	pPlayer = Get_CurrentPlayer();	

	return	 pPlayer.lock()->Get_Component<CStatus_Player>();
}

weak_ptr<CPlayer> CGameManager::Get_CurrentPlayer()
{
	return m_pCurrentPlayer;
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

void CGameManager::Add_Shaking(_vector vShakingDir, _float fRatio, _float fShakingTime,_float fFrequency, _float fDecreaseRatio)
{
	if (m_pTargetCamera.lock().get() != m_pCurrentCamera.lock().get())
	{
		return;
	}

	if (!m_pTargetCamera.lock())
		DEBUG_ASSERT;

	m_pTargetCamera.lock()->Add_Shaking(vShakingDir, fRatio, fShakingTime, fFrequency, fDecreaseRatio);
	
}

void CGameManager::Focus_Monster()
{
//타겟 찾고
//
	m_pTargetMonster = Forced_SearchNearTargetMonster();
	if (!m_pTargetMonster.lock())
		return;

	m_pTargetCamera.lock()->Focus_Monster(m_pTargetMonster);
	m_pCurrentPlayer.lock()->Focus_Monster(m_pTargetMonster);
	
	CallBack_FocusInMonster();
}

void CGameManager::Release_Focus()
{
	m_pTargetMonster = weak_ptr<CMonster>();
	m_pTargetCamera.lock()->Release_Focus();
	m_pCurrentPlayer.lock()->Release_Focus();

	CallBack_FocusOutMonster();
}

weak_ptr<CMonster> CGameManager::Get_TargetMonster()
{
	return m_pTargetMonster;
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
	#ifdef _DEBUG_COUT_
		cout << "CloneEffect" << endl;
#endif

	return iIndex;
}

_uint CGameManager::Use_EffectGroup(const string& In_szEffectGroupName, weak_ptr<CTransform> pParentTransformCom, const _uint& In_iTimeScaleLayer)
{
	_hashcode hashcode = hash<string>()(In_szEffectGroupName);
	
	#ifdef _DEBUG_COUT_
		cout << "Use EffectGroup: " << In_szEffectGroupName << endl;
#endif


	return Use_EffectGroupFromHash(hashcode, pParentTransformCom, In_iTimeScaleLayer);
}

void CGameManager::UnUse_EffectGroup(const string& In_szEffectGroupName, const _uint& In_EffectGroupIndex)
{
	_hashcode hashcode = hash<string>()(In_szEffectGroupName);
	_uint iIndex = 0;

#ifdef _DEBUG_COUT_
		cout << "Init UnUse EffectGroup: " << In_szEffectGroupName << endl;
#endif

	for (auto& elem : m_pEffectGroups[hashcode])
	{
		if (In_EffectGroupIndex == iIndex)
		{
#ifdef _DEBUG_COUT_
		cout << "UnUse EffectGroup: " << In_szEffectGroupName << endl;
#endif
			if (elem.lock())
				elem.lock()->UnUse_EffectGroup();
		}

		++iIndex;
	}
}

void CGameManager::Enable_WeaponFromEvent(weak_ptr<CTransform> pParentTransformCom, const _bool In_bEnable)
{
	weak_ptr<CCharacter> pCharacter = Weak_Cast<CCharacter>(pParentTransformCom.lock()->Get_Owner());
	pCharacter.lock()->Enable_Weapons(In_bEnable);
}


weak_ptr<CMonster> CGameManager::Forced_SearchNearTargetMonster()
{
	list<weak_ptr<CGameObject>> pMonsters = Get_LayerSelectEnable(OBJECT_LAYER::MONSTER);

	if (pMonsters.empty())
	{
		return weak_ptr<CMonster>();
	}


	for (auto iter = pMonsters.begin(); iter != pMonsters.end();)
	{
		_vector vMonsterPosition = iter->lock()->Get_Transform()->Get_Position();

		if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(vMonsterPosition, 5.f))
		{
			iter = pMonsters.erase(iter);
		}
		else
			++iter;
	}

	if (pMonsters.empty())
	{
		return weak_ptr<CMonster>();
	}

	_vector vMyPosition = m_pCurrentPlayer.lock()->Get_Transform()->Get_Position();

	_vector vMonsterPosition;
	weak_ptr<CGameObject> pTargetMonster = pMonsters.front();
	_float fMinDistance = 99999.f;
	_float fDistance = 0.f;



	for (auto& elem : pMonsters)
	{
		if (!elem.lock())
		{
			continue;
		}

		vMonsterPosition = elem.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
		fDistance = XMVector3Length(vMonsterPosition - vMyPosition).m128_f32[0];

		if (fDistance < fMinDistance)
		{
			pTargetMonster = elem;
			fMinDistance = fDistance;
		}
	}

	return Weak_Cast<CMonster>(pTargetMonster);
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
		for (_size_t i(0); i < KeyEventJson["AnimationIndex"].size(); ++i)
		{
			if (KeyEventJson["AnimationIndex"][i].empty())
			{
				continue;
			}

			
			m_KeyEvents[szFileNameToHash].emplace(i, KEYEVENT());

			// 애니메이션 키 인덱스 리스트
			for (_size_t j(0); j < KeyEventJson["AnimationIndex"][i].size(); ++j)
			{
				if (KeyEventJson["AnimationIndex"][i][j].empty())
				{
					continue;
				}
				m_KeyEvents[szFileNameToHash][(_int)i].emplace(j, KEYFRAME_EVENT());

				// 이펙트그룹 순회

				for (auto& EffectGroupName : KeyEventJson["AnimationIndex"][i][j]["EffectName"])
				{
					if (EffectGroupName.empty())
					{
						// 이펙트 그룹 이름 리스트의 중간이 비어있으면 안된다.
						DEBUG_ASSERT;
					}

					string szEffectGroupName = EffectGroupName;

					m_KeyEvents[szFileNameToHash][(_int)i][(_int)j].EffectGroups.push_back(hash<string>()(szEffectGroupName));
				}

				if (KeyEventJson["AnimationIndex"][i][j]["Enable_Weapon"].empty())
				{
					continue;
				}

				m_KeyEvents[szFileNameToHash][(_int)i][(_int)j].Enable_Weapon.push_back(KeyEventJson["AnimationIndex"][i][j]["Enable_Weapon"]);
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

	// 무기 이벤트 발동
	for (auto& elem : Key_iter->second.Enable_Weapon)
	{
		Enable_WeaponFromEvent(In_TransformCom, elem);
	}
}

void CGameManager::Start_Cinematic(weak_ptr<CModel> _pModel, const _char* pBoneName, _matrix& OffSetMatrix, CINEMATIC_TYPE iCinematicType)
{
	m_pTargetCamera.lock()->Start_Cinematic(_pModel, pBoneName, OffSetMatrix, iCinematicType);
}

void CGameManager::End_Cinematic()
{
	m_pTargetCamera.lock()->End_Cinematic();
}

void CGameManager::Activate_Zoom(_float fRatio, _float fZoomTime, EASING_TYPE eZoomLerpFunc)
{
	m_pTargetCamera.lock()->Activate_Zoom(fRatio, fZoomTime, eZoomLerpFunc);
}

void CGameManager::Deactivate_Zoom(_float fZoomTime, EASING_TYPE eZoomLerpFunc)
{
	m_pTargetCamera.lock()->Deactivate_Zoom(fZoomTime, eZoomLerpFunc);
}

_vector CGameManager::Get_PlayerPos()
{
	if (!m_pCurrentPlayer.lock())
	{
		return XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	_matrix PlayerMat = m_pCurrentPlayer.lock()->Get_Component<CTransform>().lock()->Get_WorldMatrix();

	return PlayerMat.r[3];
}

void CGameManager::Registration_CheckPoint(weak_ptr<CInteraction_CheckPoint> In_CheckPoint)
{
	m_pCurSavePoint = In_CheckPoint;
}

HRESULT CGameManager::Respawn_LastCheckPoint(_float4* Out_RespawnPos)
{
	if (!m_pCurrentPlayer.lock() || !m_pCurSavePoint.lock())
		return E_FAIL;

	XMStoreFloat4(Out_RespawnPos, m_pCurSavePoint.lock()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	return S_OK;
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
	// TODO : wtf
	//if (KEY_INPUT(KEY::DELETEKEY, KEY_STATE::TAP))
	//{
	//	return true;
	//}

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
	m_SectionObejects.clear();

}

POINT CGameManager::Get_MousePoint()
{
	POINT	tMousePt;

	GetCursorPos(&tMousePt);
	ClientToScreen(g_hWnd, &tMousePt);

	return tMousePt;
}

void CGameManager::Registration_Section(_uint In_iSection, weak_ptr<CGameObject> In_pObj)
{
	auto iter_find = m_SectionObejects.find(In_iSection);

	if (iter_find == m_SectionObejects.end())
	{
		list<weak_ptr<CGameObject>> ObjList;
		ObjList.push_back(In_pObj);

		m_SectionObejects[In_iSection] = ObjList;
	}
	else
	{
		iter_find->second.push_back(In_pObj);
	}
}


void CGameManager::Activate_Section(_uint In_iSection, _bool In_bState)
{
	auto iter_find = m_SectionObejects.find(In_iSection);

	if (iter_find == m_SectionObejects.end())
		return;

	for (auto& elem : iter_find->second)
	{
		elem.lock()->OnEventMessage((In_bState) ? ((_uint)EVENT_TYPE::ON_ENTER_SECTION) : ((_uint)EVENT_TYPE::ON_EXIT_SECTION));
	}
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
