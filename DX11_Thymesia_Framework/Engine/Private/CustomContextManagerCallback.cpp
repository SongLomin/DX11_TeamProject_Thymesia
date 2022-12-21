#include "CustomContextManagerCallback.h"
#include "GameInstance.h"

CCustomContextManagerCallback::CCustomContextManagerCallback(ID3D11Device* pDevice)
{
	mDevice = pDevice;
}

void CCustomContextManagerCallback::acquireContext()
{
	mMutex.lock();
}

void CCustomContextManagerCallback::releaseContext()
{
	mMutex.unlock();
}

ID3D11Device* CCustomContextManagerCallback::getDevice() const
{
	return mDevice;
}

ID3D11DeviceContext* CCustomContextManagerCallback::getContext() const
{
	return mContext;
}

bool CCustomContextManagerCallback::synchronizeResources() const
{
	return mSynchronizeResources;
}
