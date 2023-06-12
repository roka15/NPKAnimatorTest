#pragma once
#include "CommonInclude.h"
#include "LRU_Queue.h"
#include "My_Resource.h"
#define MAX_RESOURCE 2400
namespace yeram_client
{
	class Resources;
}
namespace core
{
	class ResourceDeleterThread
	{
	protected:
		typedef yeram_client::My_Resource myResource;
		typedef yeram_client::Resource_Greater<std::shared_ptr<myResource>> resource_greater;
		//typedef yeram_client::Resource_Less<std::shared_ptr<myResource>> resource_less;
		typedef yeram_client::Resources Resources;
	public:
		static void Initialize();
		static void RegisterResourceInfo(myResource* _resource);
		static DWORD __stdcall DeleterThread(PVOID _param);
		static void SetResourceQueueCapacity(size_t _capacity);
		static void WriteLogConsole(myResource* _resource);
		static void Release();
		static void DeleterQueueSort();
		static void DeleteConsole();
	private:
		static LRU_Queue<std::shared_ptr<myResource>, resource_greater> mLRUQueue;
		static std::queue<std::wstring> mDeleteMessage;
		static HANDLE mDeleteThread;
		static BOOL mEndFlag;
		static CRITICAL_SECTION mCs;
	};
}


