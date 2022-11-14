#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CollisionCallBack :
    public PxContactModifyCallback
{
public:
    // PxContactModifyCallback을(를) 통해 상속됨
    virtual void onContactModify(PxContactModifyPair* const pairs, PxU32 count) override;

    void Release();
};

//physx::PxFilterFlags CollisionFilterShader(
//	physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
//	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
//	physx::PxPairFlags& retPairFlags, const void* /*constantBlock*/, physx::PxU32 /*constantBlockSize*/)
//{
//	const physx::PxU32 needsReport = physx::PxU32(GROUND_FLAG | PROP_FLAG);
//	physx::PxU32 flags = (filterData0.word3 | filterData1.word3);
//
//	//retPairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
//	//retPairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
//	//retPairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;
//
//	retPairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
//	retPairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT | physx::PxPairFlag::eNOTIFY_TOUCH_CCD;
//
//	//if ((flags & needsReport) == needsReport)
//	//{
//	//	retPairFlags |= (physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_CONTACT_POINTS);
//	//	//retPairFlags |= physx::PxPairFlag::eMODIFY_CONTACTS;
//	//}
//
//	//return physx::PxFilterFlag::eDEFAULT;
//	return physx::PxFilterFlag::eCALLBACK;
//}
//
//class CCollisionSolver : public physx::PxSimulationFilterCallback
//{
//	virtual	physx::PxFilterFlags pairFound(physx::PxU32 /*pairID*/,
//		physx::PxFilterObjectAttributes /*attributes0*/, physx::PxFilterData /*filterData0*/, const physx::PxActor* /*a0*/, const physx::PxShape* s0,
//		physx::PxFilterObjectAttributes /*attributes1*/, physx::PxFilterData /*filterData1*/, const physx::PxActor* /*a1*/, const physx::PxShape* s1,
//		physx::PxPairFlags& pairFlags)
//	{
//		if (s0->userData || s1->userData)
//		{
//			auto pObject0 = ((CPhysCollideUserData*)s0->userData)->physObject;
//			auto pObject1 = ((CPhysCollideUserData*)s1->userData)->physObject;
//
//			if (m_pSolver && !m_pSolver->ShouldCollide(pObject0, pObject1, pObject0->GetGameData(), pObject1->GetGameData()))
//			{
//				pairFlags = physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
//				return physx::PxFilterFlag::eKILL;
//			}
//		}
//
//		pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
//		pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT | physx::PxPairFlag::eNOTIFY_TOUCH_CCD;
//
//		return physx::PxFilterFlags();
//	}
//};

END