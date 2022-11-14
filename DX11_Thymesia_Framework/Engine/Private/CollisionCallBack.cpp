#include "CollisionCallBack.h"

void CollisionCallBack::onContactModify(PxContactModifyPair* const pairs, PxU32 count)
{
	_int i = 0;

}

void CollisionCallBack::Release()
{
	delete this;
}
