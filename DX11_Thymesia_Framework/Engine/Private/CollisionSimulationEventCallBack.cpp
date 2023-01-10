#include "CollisionSimulationEventCallBack.h"
#include "PhysXCollider.h"
#include "PhysX_Manager.h"

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

// 둘다 트리거를 쓰지 않는 쉐이프면 onContact
void CollisionSimulationEventCallBack::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{

	while (nbPairs--)
	{
		const PxContactPair& current = *pairs++;

		// The reported pairs can be trigger pairs or not. We only enabled contact reports for
		// trigger pairs in the filter shader, so we don't need to do further checks here. In a
		// real-world scenario you would probably need a way to tell whether one of the shapes
		// is a trigger or not. You could e.g. reuse the PxFilterData like we did in the filter
		// shader, or maybe use the shape's userData to identify triggers, or maybe put triggers
		// in a hash-set and test the reported shape pointers against it. Many options here.

		//if (!(pairHeader.flags & PxContactPairHeaderFlag::eREMOVED_ACTOR_0) || !(pairHeader.flags & PxContactPairHeaderFlag::eREMOVED_ACTOR_0))
		//{
		//	PxFilterData shape0_FilterData = current.shapes[0]->getSimulationFilterData();
		//	PxFilterData shape1_FilterData = current.shapes[1]->getSimulationFilterData();

		//	weak_ptr<CPhysXCollider> pColliderCom = GET_SINGLE(CPhysX_Manager)->Find_PhysXCollider(shape0_FilterData.word3);
		//	weak_ptr<CPhysXCollider> pOtherColliderCom = GET_SINGLE(CPhysX_Manager)->Find_PhysXCollider(shape1_FilterData.word3);

		//	pColliderCom.lock()->OnCollision(pOtherColliderCom);
		//	pOtherColliderCom.lock()->OnCollision(pColliderCom);
		//	// collision Exit event
		//}


		/*if (current.events & (PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_CCD))
		{
			PxFilterData shape0_FilterData = current.shapes[0]->getSimulationFilterData();
			PxFilterData shape1_FilterData = current.shapes[1]->getSimulationFilterData();

			printf("Shape is entering Contact volume!!!!\n");
			weak_ptr<CPhysXCollider> pColliderCom = GET_SINGLE(CPhysX_Manager)->Find_PhysXCollider(shape0_FilterData.word3);
			weak_ptr<CPhysXCollider> pOtherColliderCom = GET_SINGLE(CPhysX_Manager)->Find_PhysXCollider(shape1_FilterData.word3);

			pColliderCom.lock()->PhysXCollisionEnter(pOtherColliderCom);
			pOtherColliderCom.lock()->PhysXCollisionEnter(pColliderCom);
		}
		if (current.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			PxFilterData shape0_FilterData = current.shapes[0]->getSimulationFilterData();
			PxFilterData shape1_FilterData = current.shapes[1]->getSimulationFilterData();

			printf("Shape is leaving Contact volume!!!!\n");

			weak_ptr<CPhysXCollider> pColliderCom = GET_SINGLE(CPhysX_Manager)->Find_PhysXCollider(shape0_FilterData.word3);
			weak_ptr<CPhysXCollider> pOtherColliderCom = GET_SINGLE(CPhysX_Manager)->Find_PhysXCollider(shape1_FilterData.word3);

			pColliderCom.lock()->PhysXCollisionExit(pOtherColliderCom);
			pOtherColliderCom.lock()->PhysXCollisionExit(pColliderCom);

		}*/

	}
}


// 트리거를 쓰는 얘들은 OnTrigger
void CollisionSimulationEventCallBack::onTrigger(PxTriggerPair* pairs, PxU32 count)
{

	while (count--)
	{
		const PxTriggerPair& current = *pairs++;
		if (current.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			weak_ptr<CPhysXCollider> pColliderCom = GET_SINGLE(CPhysX_Manager)->Find_PhysXCollider(*(_uint*)current.triggerActor->userData);
			weak_ptr<CPhysXCollider> pOtherColliderCom = GET_SINGLE(CPhysX_Manager)->Find_PhysXCollider(*(_uint*)current.otherActor->userData);

			if (pColliderCom.lock()->Get_ColliderDesc().bTrigger &&
				pOtherColliderCom.lock()->Get_ColliderDesc().bTrigger)
			{
				pColliderCom.lock()->PhysXCollisionEnter(pOtherColliderCom);
				pOtherColliderCom.lock()->PhysXCollisionEnter(pColliderCom);
			}

			//CPhysXCollider* pColliderCom = (CPhysXCollider*)current.triggerActor->userData;
			//CPhysXCollider* pOtherColliderCom = (CPhysXCollider*)current.otherActor->userData;

			//weak_ptr<CPhysXCollider> pColliderCom = *reinterpret_cast<std::weak_ptr<CPhysXCollider>*>(&current.triggerActor->userData);
			//weak_ptr<CPhysXCollider> pOtherColliderCom = *reinterpret_cast<std::weak_ptr<CPhysXCollider>*>(&current.otherActor->userData);

			
		}
		if (current.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			weak_ptr<CPhysXCollider> pColliderCom = GET_SINGLE(CPhysX_Manager)->Find_PhysXCollider(*(_uint*)current.triggerActor->userData);
			weak_ptr<CPhysXCollider> pOtherColliderCom = GET_SINGLE(CPhysX_Manager)->Find_PhysXCollider(*(_uint*)current.otherActor->userData);

			if (pColliderCom.lock()->Get_ColliderDesc().bTrigger &&
				pOtherColliderCom.lock()->Get_ColliderDesc().bTrigger)
			{
				pColliderCom.lock()->PhysXCollisionExit(pOtherColliderCom);
				pOtherColliderCom.lock()->PhysXCollisionExit(pColliderCom);
			}
		}
			cout << "Shape is leaving trigger volume\n";
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
