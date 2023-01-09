#pragma once

// maps unsigned 8 bits/channel to D3DCOLOR
#define D3DCOLOR_ARGB(a,r,g,b) \
    ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

#define DEGREE(value)	XMConvertToDegrees(value)
#define RADIANS(value)	XMConvertToRadians(value) 

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define MSG_BOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Error"), MB_OK)

#define BEGIN(NAMESPACE) namespace NAMESPACE { 
#define END }

#define CHECK_CUDA \
	(*GET_SINGLE(CCuda_Device)->Get_CudaContext())


#define BEGIN_PERFROMANCE_CHECK(TAG)\
	GAMEINSTANCE->Begin_PerformanceTime(TAG);
#define END_PERFROMANCE_CHECK(TAG)\
	GAMEINSTANCE->End_PerformanceTime(TAG);


#define NO_COPY(ClassType)								\
ClassType(const ClassType&) = delete;					\
ClassType& operator=(const ClassType&) = delete;

//#define DECLARE_SINGLETON(ClassType)					\
//		NO_COPY(ClassType)								\
//public :												\
//	static ClassType* Get_Instance();					\
//	static void Destroy_Instance();			\
//private:												\
//	static ClassType* m_pInstance;
//
//#define IMPLEMENT_SINGLETON(ClassType)					\
//ClassType* ClassType::m_pInstance = nullptr;			\
//ClassType* ClassType::Get_Instance()					\
//{														\
//	if (nullptr == m_pInstance)							\
//		m_pInstance = new ClassType;					\
//	return m_pInstance;									\
//}														\
//void ClassType::Destroy_Instance()						\
//{														\
//	return Safe_Release(m_pInstance);					\
//}

//간략 버전
#define DECLARE_SINGLETON(ClassType)									\
private:																\
	static shared_ptr<ClassType> m_pInstance;							\
public:																	\
	ClassType() {}														\
	virtual ~ClassType() { Free(); }									\
	static shared_ptr<ClassType> Create_Instance();						\
	static shared_ptr<ClassType> Get_Instance();						\
	static void Destroy_Instance();										\

#define IMPLEMENT_SINGLETON(ClassType)									\
shared_ptr<ClassType> ClassType::m_pInstance = shared_ptr<ClassType>();	\
shared_ptr<ClassType> ClassType::Get_Instance()							\
{																		\
	if (nullptr == m_pInstance.get())									\
		assert(false);													\
																		\
	return m_pInstance;													\
}																		\
shared_ptr<ClassType> ClassType::Create_Instance()						\
{																		\
	if (nullptr != m_pInstance.get())									\
		assert(false);													\
	m_pInstance = make_shared<ClassType>();								\
																		\
	return m_pInstance;													\
}																		\
void ClassType::Destroy_Instance()										\
{																		\
	if(nullptr == m_pInstance.get())									\
		return;															\
	m_pInstance->OnDestroy();											\
	m_pInstance.reset();												\
}																		

#define DECLARE_CLONABLE(ClassType, BaseType)								\
public:																		\
	static shared_ptr<ClassType> Create();									\
	virtual shared_ptr<BaseType> Clone(unsigned int iLevelIndex, void* pArg = nullptr) override;		\



#define CLONE_H(ClassType, BaseType) DECLARE_CLONABLE(ClassType, BaseType)

#define IMPLEMENT_CLONABLE(ClassType, BaseType)								\
shared_ptr<ClassType> ClassType::Create()									\
{																			\
	shared_ptr<ClassType>		pInstance = make_shared<ClassType>();		\
	pInstance->m_this = pInstance;											\
	pInstance->Initialize_Prototype();										\
	return pInstance;														\
}																			\
shared_ptr<BaseType> ClassType::Clone(unsigned int iLevelIndex, void* pArg)							\
{																			\
	shared_ptr<ClassType>		pInstance = make_shared<ClassType>(*this);	\
	pInstance->m_this = pInstance;											\
	pInstance->m_CreatedLevel = iLevelIndex;								\
	pInstance->Initialize(pArg);											\
	return pInstance;														\
}																			\

#define CLONE_C(ClassType, BaseType) IMPLEMENT_CLONABLE(ClassType, BaseType)

#define DECLARE_GAMECLASS(ClassType)										\
public:																		\
ClassType();																\
virtual ~ClassType();														\

#define GAMECLASS_H(ClassType) DECLARE_GAMECLASS(ClassType)

#define IMPLEMENT_GAMECLASS(ClassType)										\
ClassType::ClassType()														\
{																			\
}																			\
																			\
ClassType::~ClassType()														\
{																			\
	Free();																	\
}																			

#define GETSET(VarType, VarName)											\
public:																		\
	const VarType& Get_##VarName() const									\
	{																		\
		return m_##VarName;													\
	}																		\
	void Set_##VarName(const VarType& In_Arg)								\
	{																		\
		m_##VarName = In_Arg;												\
	}																		\
private:																	\
	VarType m_##VarName;													\

#define SHALLOW_COPY(ClassType)												\
public:																		\
ClassType(const ClassType& rhs)												\
{																			\
	*this = rhs;															\
}																			\



#define GAMECLASS_C(ClassType) IMPLEMENT_GAMECLASS(ClassType)

#define USE_START(ClassType) GAMEINSTANCE->Get_CallbackStart() += bind(&ClassType::Start, this); //GET_SINGLE(CObject_Manager)->CallBack_Start += bind(&ClassType::Start, this);
#define USE_TICK(ClassType) m_pOwner.lock()->CallBack_Tick += bind(&ClassType::Tick, this, placeholders::_1);
#define USE_LATETICK(ClassType) m_pOwner.lock()->CallBack_LateTick += bind(&ClassType::LateTick, this, placeholders::_1);
#define USE_RENDER(ClassType) m_pOwner.lock()->CallBack_Render += bind(&ClassType::Render, this);


#define UNUSE_START(ClassType) GAMEINSTANCE->Get_CallbackStart() -= bind(&ClassType::Start, this);
#define UNUSE_TICK(ClassType) m_pOwner.lock()->CallBack_Tick -= bind(&ClassType::Tick, this, placeholders::_1);
#define UNUSE_LATETICK(ClassType) m_pOwner.lock()->CallBack_LateTick -= bind(&ClassType::LateTick, this, placeholders::_1);
#define UNUSE_RENDER(ClassType) m_pOwner.lock()->CallBack_Render -= bind(&ClassType::Render, this);


#define GET_SINGLE(type)	(type::Get_Instance())

#define GAMEINSTANCE GET_SINGLE(CGameInstance)
//#define LPDEVICE (GAMEINSTANCE->Get_Device())
#define DEVICE (GAMEINSTANCE->Get_Device())
//#define LPDEVICECONTEXT (GAMEINSTANCE->Get_DeviceContext())
#define DEVICECONTEXT (GAMEINSTANCE->Get_DeviceContext())
#define DELTATIME GAMEINSTANCE->Get_DeltaTime()

#define KEY_INPUT GAMEINSTANCE->Is_KeyState
