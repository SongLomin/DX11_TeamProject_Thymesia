#pragma once
#include "CustomThread.h"

BEGIN(Engine)
class CGameObject;

class CGameObject_Thread :
    public CCustomThread
{
public:
    void Add_ReserveThreadObject(weak_ptr<CGameObject> pGameObject);
    void Bind_Works();

    void Add_ThreadObject(weak_ptr<CGameObject> pGameObject);
    future<void> Async(const THREAD_TYPE In_eThread_Type);

public:
    void Loop(const THREAD_TYPE In_eThread_Type);
    void Do(weak_ptr<CGameObject> pGameObject);
    
private:
    list<weak_ptr<CGameObject>>	m_ThreadObjects;
    list<weak_ptr<CGameObject>>   m_ReservedThreadObjects;

    THREAD_TYPE     m_eThreadType;

public:
    static shared_ptr<CGameObject_Thread> Create(const THREAD_TYPE In_eThread_Type);

};

END