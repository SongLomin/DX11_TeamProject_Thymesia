#include "CollisionSimulationEventCallBack.h"

void CollisionSimulationEventCallBack::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
	cout << "onConstraintBreak" << endl;
}

void CollisionSimulationEventCallBack::onWake(PxActor** actors, PxU32 count)
{
	cout << "onWake" << endl;
}

void CollisionSimulationEventCallBack::onSleep(PxActor** actors, PxU32 count)
{
	cout << "onSleep" << endl;
}

// �Ѵ� Ʈ���Ÿ� ���� �ʴ� �������� onContact
void CollisionSimulationEventCallBack::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	cout << "OnContact" << endl;
}


// Ʈ���Ÿ� ���� ����� OnTrigger
void CollisionSimulationEventCallBack::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	while (count--)
	{
		const PxTriggerPair& current = *pairs++;
		if (current.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			cout << "Shape is entering trigger volume\n";
		if (current.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
			cout << "Shape is leaving trigger volume\n";
		if (current.status & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			cout << "�Ľý�Ʈ ���\n";
	}
}

void CollisionSimulationEventCallBack::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	cout << "onAdvance" << endl;
}

void CollisionSimulationEventCallBack::Release()
{
	delete this;
}
