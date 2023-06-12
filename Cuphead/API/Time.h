#pragma once
#include "CommonInclude.h"
namespace yeram_client
{
	class Time
	{
	public:
		static void Initailize();
		static void Update();
		static void Render(HDC hdc);
		static void Release();
		__forceinline static double DeltaTime() { return mDeltaTime; };
	private:
		static double mDeltaTime;
		static double mSecond;
		static LARGE_INTEGER mCpuFrequency;
		static LARGE_INTEGER mPrevFrequency;
		static LARGE_INTEGER mCurFrequency;
	};

}

