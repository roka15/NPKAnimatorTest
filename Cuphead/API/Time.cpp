#include "Time.h"
#include "Application.h"
extern yeram_client::Application application;
namespace yeram_client
{
	double Time::mDeltaTime = 0.0l;
	double Time::mSecond = 0.0l;
	LARGE_INTEGER Time::mCpuFrequency = {};
	LARGE_INTEGER Time::mPrevFrequency = {};
	LARGE_INTEGER Time::mCurFrequency = {};


	void Time::Initailize()
	{
		//cpu 고유 진동수
		QueryPerformanceFrequency(&mCpuFrequency);
		//프로그램 시작시 진동수
		QueryPerformanceCounter(&mPrevFrequency);
		

	}

	void Time::Update()
	{
		//현재 진동 수
		QueryPerformanceCounter(&mCurFrequency);
		double diffFrequency = mCurFrequency.QuadPart - mPrevFrequency.QuadPart;
		mDeltaTime = diffFrequency / mCpuFrequency.QuadPart;
		mPrevFrequency.QuadPart = mCurFrequency.QuadPart;
	}

	void Time::Render(HDC hdc)
	{
		mSecond += mDeltaTime;
		if (mSecond > 1.0f)
		{
			HWND hWnd = application.GetHandle();
			wchar_t szFloat[50] = {};
			float FPS = 1.0f / (float)mDeltaTime;
			swprintf_s(szFloat, 50, L"FPS: %d", (UINT)FPS);
			int iLen = wcsnlen(szFloat, 50);
			SetWindowText(hWnd, szFloat);
			mSecond = 0.0f;
		}
		
	}

	void Time::Release()
	{
	}

}