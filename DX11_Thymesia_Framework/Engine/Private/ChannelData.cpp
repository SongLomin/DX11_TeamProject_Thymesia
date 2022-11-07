#include "ChannelData.h"

HRESULT CHANNEL_DATA::Make_ChannelData(aiNodeAnim* In_pChannel)
{
	szNodeName = In_pChannel->mNodeName.C_Str();
	iNumScalingKeys = In_pChannel->mNumScalingKeys;
	iNumRotationKeys = In_pChannel->mNumRotationKeys;
	iNumPositionKeys = In_pChannel->mNumPositionKeys;

	tScalingKeys.reserve(iNumScalingKeys);
	for (_uint i = 0; i < iNumScalingKeys; i++)
	{
		KEY_DATA Key;

		memcpy(&Key.vValue, &In_pChannel->mScalingKeys[i].mValue, sizeof(_float3));
		Key.fTime = (_float)In_pChannel->mScalingKeys[i].mTime;

		tScalingKeys.push_back(Key);
	}


	tRotationKeys.reserve(iNumRotationKeys);
	for (_uint i = 0; i < iNumRotationKeys; i++)
	{
		ROTATIONKEY_DATA Key;
		Key.vValue.x = In_pChannel->mRotationKeys[i].mValue.x;
		Key.vValue.y = In_pChannel->mRotationKeys[i].mValue.y;
		Key.vValue.z = In_pChannel->mRotationKeys[i].mValue.z;
		Key.vValue.w = In_pChannel->mRotationKeys[i].mValue.w;
		Key.fTime = (_float)In_pChannel->mRotationKeys[i].mTime;

		tRotationKeys.push_back(Key);
	}


	tPositionKeys.reserve(iNumPositionKeys);
	for (_uint i = 0; i < iNumPositionKeys; i++)
	{
		KEY_DATA Key;
		memcpy(&Key.vValue, &In_pChannel->mPositionKeys[i].mValue, sizeof(_float3));
		Key.fTime = (_float)In_pChannel->mPositionKeys[i].mTime;

		tPositionKeys.push_back(Key);
	}


	return S_OK;
}

void CHANNEL_DATA::Bake_Binary(ofstream& os)
{
	write_typed_data(os, szNodeName.size());
	os.write(&szNodeName[0], szNodeName.size());

	write_typed_data(os, iNumScalingKeys);
	write_typed_data(os, iNumRotationKeys);
	write_typed_data(os, iNumPositionKeys);

	for (_uint i = 0; i < iNumScalingKeys; i++)
	{
		write_typed_data(os, tScalingKeys[i]);
	}

	for (_uint i = 0; i < iNumRotationKeys; i++)
	{
		write_typed_data(os, tRotationKeys[i]);
	}

	for (_uint i = 0; i < iNumPositionKeys; i++)
	{
		write_typed_data(os, tPositionKeys[i]);
	}
}

void CHANNEL_DATA::Load_FromBinary(ifstream& is)
{
	size_t istringSize;
	read_typed_data(is, istringSize);
	szNodeName.resize(istringSize);
	is.read(&szNodeName[0], istringSize);

	read_typed_data(is, iNumScalingKeys);
	read_typed_data(is, iNumRotationKeys);
	read_typed_data(is, iNumPositionKeys);

	tScalingKeys.reserve(iNumScalingKeys);
	for (_uint i = 0; i < iNumScalingKeys; i++)
	{
		KEY_DATA Key;

		read_typed_data(is, Key);

		tScalingKeys.push_back(Key);
	}


	tRotationKeys.reserve(iNumRotationKeys);
	for (_uint i = 0; i < iNumRotationKeys; i++)
	{
		ROTATIONKEY_DATA Key;

		read_typed_data(is, Key);

		tRotationKeys.push_back(Key);
	}


	tPositionKeys.reserve(iNumPositionKeys);
	for (_uint i = 0; i < iNumPositionKeys; i++)
	{
		KEY_DATA Key;
		
		read_typed_data(is, Key);

		tPositionKeys.push_back(Key);
	}
}
