#include "ChannelData.h"

HRESULT CHANNEL_DATA::Make_ChannelData(aiNodeAnim* In_pChannel)
{
	szNodeName = In_pChannel->mNodeName.C_Str();
	iNumScalingKeys = In_pChannel->mNumScalingKeys;
	iNumRotationKeys = In_pChannel->mNumRotationKeys;
	iNumPositionKeys = In_pChannel->mNumPositionKeys;

	iNumKeyframes = max(iNumScalingKeys, iNumRotationKeys);
	iNumKeyframes = max(iNumKeyframes, iNumPositionKeys);

	KEYFRAME			KeyFrame;
	//KEY_DATA			ScalingKey;
	//ROTATIONKEY_DATA	RotationKey;
	//KEY_DATA			PositionKey;

	tKeyFrames.reserve(iNumKeyframes);
	//tScalingKeys.reserve(iNumKeyframes);
	//tRotationKeys.reserve(iNumKeyframes);
	//tPositionKeys.reserve(iNumKeyframes);

	for (_uint i = 0; i < iNumKeyframes; i++)
	{
		if (iNumScalingKeys > i)
		{
			memcpy(&KeyFrame.vScale, &In_pChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = (_float)In_pChannel->mScalingKeys[i].mTime;
		}

		if (iNumRotationKeys > i)
		{
			KeyFrame.vRotation.x = In_pChannel->mRotationKeys[i].mValue.x;
			KeyFrame.vRotation.y = In_pChannel->mRotationKeys[i].mValue.y;
			KeyFrame.vRotation.z = In_pChannel->mRotationKeys[i].mValue.z;
			KeyFrame.vRotation.w = In_pChannel->mRotationKeys[i].mValue.w;
			KeyFrame.fTime = (_float)In_pChannel->mRotationKeys[i].mTime;
		}

		if (iNumPositionKeys > i)
		{
			memcpy(&KeyFrame.vPosition, &In_pChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = (_float)In_pChannel->mPositionKeys[i].mTime;
		}

		tKeyFrames.emplace_back(KeyFrame);
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
	write_typed_data(os, iNumKeyframes);

	for (_uint i = 0; i < iNumKeyframes; i++)
	{
		write_typed_data(os, tKeyFrames[i]);
	}


	/*for (_uint i = 0; i < iNumScalingKeys; i++)
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
	}*/


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
	read_typed_data(is, iNumKeyframes);

	tKeyFrames.reserve(iNumKeyframes);

	KEYFRAME KeyFrame;

	for (_uint i = 0; i < iNumKeyframes; i++)
	{
		read_typed_data(is, KeyFrame);
		tKeyFrames.emplace_back(KeyFrame);
	}

	int i = 0;

	/*tScalingKeys.reserve(iNumScalingKeys);
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
	}*/
}

void CHANNEL_DATA::Bake_ReverseChannel(shared_ptr<CHANNEL_DATA>& Out_ChannelData, const _float In_fDuration)
{
	Out_ChannelData->szNodeName = szNodeName;
	Out_ChannelData->iNumScalingKeys = iNumScalingKeys;
	Out_ChannelData->iNumRotationKeys = iNumRotationKeys;
	Out_ChannelData->iNumPositionKeys = iNumPositionKeys;
	Out_ChannelData->iNumKeyframes = iNumKeyframes;

	Out_ChannelData->tKeyFrames.reserve(Out_ChannelData->iNumKeyframes);

	KEYFRAME			KeyFrame;

	for (_int i = (_int)Out_ChannelData->iNumKeyframes - 1; i >= 0; --i)
	{
		memcpy(&KeyFrame, &tKeyFrames[i], sizeof(KEYFRAME));
		KeyFrame.fTime = In_fDuration - KeyFrame.fTime;

		Out_ChannelData->tKeyFrames.emplace_back(KeyFrame);
	}
}
