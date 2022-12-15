#pragma once
#include "Component.h"

BEGIN(Engine)

class CShader;
class CCell;
struct MODEL_DATA;

class ENGINE_DLL CNavigation final : public CComponent
{
	GAMECLASS_H(CNavigation);
	CLONE_H(CNavigation, CComponent);
	
public:
	CNavigation(const CNavigation& rhs);

public:
	typedef struct tagNavigationDesc
	{
		_int			m_iCurrentIndex = -1;
	}NAVIDESC;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	void Init_Navi(const string& szModelKey, const NAVIDESC& tNaviDesc);

public:
	_bool	IsMove(_fvector vPosition, _float3* Out_LineDir);
	void	Set_CurrentIndex(const _int& In_iIndex);
	_int	Get_CurrentIndex();

private:
	void Reset_VisitedTable();

#ifdef _DEBUG
public:
	HRESULT Render(ID3D11DeviceContext* pDeviceContext);
#endif // _DEBUG



private:
	shared_ptr<MODEL_DATA>				m_pModelData;
	vector<shared_ptr<CCell>>			m_pCells;

	NAVIDESC						m_NaviDesc;

	vector<_bool>					m_IsVisitedTable;

private:
	HRESULT Make_Neighbor();

#ifdef _DEBUG
private:
	weak_ptr<CShader> m_pShader;
#endif // _DEBUG

public:
	void Free();
};

END