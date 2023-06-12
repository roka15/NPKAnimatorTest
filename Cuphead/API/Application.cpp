#include "Application.h"
#include "SceneManager.h"
#include "Time.h"
#include "Input.h"
#include "Resources.h"
#include "ResourceDeleterThread.h"
#include "Camera.h"
#include "ColliderManager.h"

#include "SoundManager.h"
//#include "WindowManager.h"
namespace yeram_client
{
	Application::Application() :mHwnd(NULL), mhdc(NULL)
	{
	}
	Application::~Application()
	{
	}
	void Application::Initialize(HWND _hWnd)
	{
		mHwnd = _hWnd;
		mhdc = GetDC(mHwnd);
		mWidth = 1600;
		mHeight = 900;
		//비트맵 해상도 설정을 위한 실제 윈도우 크기 계산.
		RECT rect = { 0,0,mWidth,mHeight };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
		//윈도우 크기 변경 및 출력 설정
		SetWindowPos(mHwnd, nullptr,100, 50, 
			rect.right-rect.left,
			rect.bottom-rect.top, false);
		ShowWindow(mHwnd, true);


		//백버퍼
		mBackBuffer=CreateCompatibleBitmap(mhdc,mWidth,mHeight);
		mBackHdc = CreateCompatibleDC(mhdc);
		HBITMAP default_Bitmap=(HBITMAP)SelectObject(mBackHdc, mBackBuffer);
		DeleteObject(default_Bitmap);

		core::ResourceDeleterThread::Initialize();
		core::Input::Initialize();
		//WindowManager::Initialize();
		SoundManager::Initialize();
		SceneManager::Initalize();
		Time::Initailize();
		Camera::Initialize();
	}
	void Application::Run()
	{
		Update();
		Render();
	}
	void Application::Update()
	{
		core::Input::Update();
		Resources::Update();
		SceneManager::Update();
		Camera::Update();
		Time::Update();
		ColliderManager::Update();
		//MoveObjectManager::Update();
	}
	void Application::Render()
	{
		ScreenClear();
		core::Input::Render(mBackHdc);
		SceneManager::Render(mBackHdc);
		Time::Render(mBackHdc);
		Camera::Render(mBackHdc);
		//백버퍼를 원본 dc에 넘겨주기. SRCCOPY : 원본을 그대로 복사.
		BitBlt(mhdc, 0, 0, mWidth, mHeight, mBackHdc, 0, 0, SRCCOPY);
	}
	void Application::Release()
	{
		
		core::Input::Release();
		SceneManager::Release();
		Time::Release();
		Camera::Release();
		core::ResourceDeleterThread::Release();
		Resources::Release();
		//WindowManager::Release();
	}
	HWND Application::GetHandle()
	{
		return mHwnd;
	}
	HDC Application::GetHDC()
	{
		return mhdc;
	}
	Vector2 Application::GetWindowSize()
	{
		return Vector2((long)mWidth, (long)mHeight);
	}
	void Application::ScreenClear()
	{
		HBRUSH grayBrush = CreateSolidBrush(RGB(121, 121, 121));
		HBRUSH OldBrush = (HBRUSH)SelectObject(mBackHdc, grayBrush);
		::Rectangle(mBackHdc, -1, -1, 1602, 902);
		SelectObject(mBackHdc, OldBrush);
		DeleteObject(grayBrush);
	}
}