#include "CollisionSimulationEventCallBack.h"

void CollisionSimulationEventCallBack::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
	int i = 0;
}

void CollisionSimulationEventCallBack::onWake(PxActor** actors, PxU32 count)
{
	int i = 0;
}

void CollisionSimulationEventCallBack::onSleep(PxActor** actors, PxU32 count)
{
	int i = 0;
}

// 둘다 트리거를 쓰지 않는 쉐이프면 onContact
void CollisionSimulationEventCallBack::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	int i = 0;
}


// 트리거를 쓰는 얘들은 OnTrigger
void CollisionSimulationEventCallBack::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	int i = 0;
}

void CollisionSimulationEventCallBack::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	int i = 0;
}

void CollisionSimulationEventCallBack::Release()
{
	delete this;
}
