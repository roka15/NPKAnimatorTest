#include "SoundManager.h"
#include "Application.h"
extern yeram_client::Application application;

namespace yeram_client
{
	LPDIRECTSOUND8 SoundManager::mSoundDevice;
	bool SoundManager::Initialize()
	{
		if (FAILED(DirectSoundCreate8(NULL, &mSoundDevice, NULL)))
		{
			MessageBox(NULL, L"�������̽���������", L"SYSTEM ERROR", MB_OK);
			return false;
		}

		// ���� ����̽� �������� ����.
		HWND hWnd = application.GetHandle();
		if (FAILED(mSoundDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE))) // Flag�� ����
		{
			MessageBox(NULL, L"�������̽� �������� ����", L"SYSTEM ERROR", MB_OK);
			return false;
		}

		return true;
	}
	void SoundManager::Release()
	{
		mSoundDevice->Release();
		mSoundDevice = NULL;
	}
}

