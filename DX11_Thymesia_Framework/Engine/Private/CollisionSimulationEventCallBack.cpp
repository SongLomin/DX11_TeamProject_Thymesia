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

void CollisionSimulationEventCallBack::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	int i = 0;
}

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
