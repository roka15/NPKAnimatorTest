#pragma once
#include "CommonInclude.h"

namespace yeram_client
{
	class SoundManager
	{
	public:
		static bool Initialize();
		static LPDIRECTSOUND8 GetDevice() { return mSoundDevice; }
		static void Release();
	private:
		static LPDIRECTSOUND8 mSoundDevice;
	};
}


