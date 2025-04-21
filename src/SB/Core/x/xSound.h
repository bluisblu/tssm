#ifndef XSOUND_H
#define XSOUND_H

#include <types.h>

enum iSndHandle
{
};

enum iSndGroupHandle
{
};

struct zStreamedSound
{
    unsigned char* mAssetName;
    F32 mCycleTime;
    U32 mFlags;
    F32 mSoonestTimeToPlay;
    iSndGroupHandle mSoundGroupHdl;

    void StopLastPlayedSound();
};

struct zStreamedSoundList
{
    U32 mNumSounds;
    zStreamedSound* mSounds;
};

#endif
