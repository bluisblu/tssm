#ifndef ZSTREAMEDSOUND_H
#define ZSTREAMEDSOUND_H

#include "iSnd.h"

struct zStreamedSoundList
{
    U32 mNumSounds;
    zStreamedSound* mSounds;

    void Reset();
    void InitWithSounds(zStreamedSound* soundList);
};

struct zStreamedSound
{
    char* mAssetName;
    F32 mCycleTime;
    U32 mFlags;
    F32 mSoonestTimeToPlay;
    iSndGroupHandle mSoundGroupHdl;
};

#endif
