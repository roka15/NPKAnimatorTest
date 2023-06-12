#include "ResourceDeleterThread.h"
#include "Resources.h"
#include "Image.h"
#include "Audio.h"
namespace core
{
	typedef yeram_client::Image Image;
	typedef yeram_client::Audio Audio;
	LRU_Queue<std::shared_ptr<ResourceDeleterThread::myResource>, ResourceDeleterThread::resource_greater> ResourceDeleterThread::mLRUQueue(MAX_RESOURCE);
	std::queue<std::wstring> ResourceDeleterThread::mDeleteMessage;
	HANDLE ResourceDeleterThread::mDeleteThread;
	BOOL ResourceDeleterThread::mEndFlag=false;
	CRITICAL_SECTION ResourceDeleterThread::mCs;
	void ResourceDeleterThread::Initialize()
	{
		BOOL suc = AllocConsole();

		HWND consoleHandle = GetConsoleWindow();

		// �ܼ� â ��ġ�� ũ�� ����
		RECT consoleRect;
		GetWindowRect(consoleHandle, &consoleRect);
		MoveWindow(consoleHandle, 1500, 100, 1100, 800, TRUE);

		if (suc == false)
		{

		}
		LPVOID lpParam = nullptr;
		InitializeCriticalSection(&mCs);
		mDeleteThread = CreateThread(NULL, 0, DeleterThread,lpParam, 0, NULL);
		if (mDeleteThread == NULL)
		{

		}
	}

	void ResourceDeleterThread::RegisterResourceInfo(myResource* _resource)
	{
		// �� �Լ��� main thread���� �����ϴϱ� ����ȭ �����൵ �ɵ�?
		std::shared_ptr<myResource> inData = std::shared_ptr<myResource>(_resource, std::bind([_resource]()->
			void
			{
				Resources::Remove(_resource->GetKey());
				WriteLogConsole(_resource);
				delete _resource;
			}
		));
		std::shared_ptr<myResource> outData = nullptr;
		bool is_delete = mLRUQueue.Push(inData, outData);
		if (is_delete == true)
		{
			////������
			double time = outData->GetTime();
			////����볡
			std::wstring key = outData->GetKey();
			outData.reset();
			outData = nullptr;
		}
	}

	DWORD __stdcall ResourceDeleterThread::DeleterThread(PVOID _param)
	{
		while (1)
		{
			DWORD waitResult = WaitForSingleObject(mDeleteThread, 0);
			if (waitResult == WAIT_TIMEOUT)
			{
				EnterCriticalSection(&mCs);
				if (mDeleteMessage.empty() == false)
				{
					std::wstring deleteMsg = mDeleteMessage.front();
					mDeleteMessage.pop();

					DWORD dwByte(0);
					HANDLE subConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
					size_t bufferSize = (deleteMsg.length() + 1) * sizeof(wchar_t);
					WriteFile(subConsoleHandle, deleteMsg.c_str(), static_cast<DWORD>(bufferSize), &dwByte, NULL);
				}
				else if(mEndFlag==true)
				{
				
					if (mDeleteThread != NULL)
						CloseHandle(mDeleteThread);

					DeleteCriticalSection(&mCs);	
					FreeConsole();
					return 0;
				}
				LeaveCriticalSection(&mCs);
			}
			else 
			{
				if (waitResult != WAIT_OBJECT_0)//error
				{
					
				}
				if (mDeleteThread != NULL)
					CloseHandle(mDeleteThread);
				DeleteCriticalSection(&mCs);
				FreeConsole();
				return 0;
			}
		}
		return 0;
	}

	void ResourceDeleterThread::SetResourceQueueCapacity(size_t _capacity)
	{
		mLRUQueue.Capacity(_capacity);
	}
	void ResourceDeleterThread::WriteLogConsole(myResource* _resource)
	{
		HWND consoleWindow = GetConsoleWindow();

		if (consoleWindow == NULL)
		{
			SendMessage(consoleWindow, WM_CLOSE, 0, 0);
			return;
		}


		std::wstring strMsg;
		Image* image = dynamic_cast<Image*>(_resource);
		Audio* audio = dynamic_cast<Audio*>(_resource);
		/*| Key: % s, Path : % s, UseTime : % lf \n",
			_resource->GetKey().c_str(), _resource->GetPath().c_str(), _resource->GetTime()*/
		if (image != nullptr)
			strMsg = L"*Image Resource Delete* \n";
		else if (audio != nullptr)
			strMsg = L"*Audio Resource Delete* \n";
		strMsg += format_string(L"|Key: %ls | UseTime: %lf|\n", _resource->GetKey().c_str(), _resource->GetTime());

		mDeleteMessage.push(strMsg);
	}
	void ResourceDeleterThread::Release()
	{
		SetResourceQueueCapacity(0);
		mEndFlag = true;
	}
	void ResourceDeleterThread::DeleterQueueSort()
	{
		mLRUQueue.Sort();
	}

}
