#pragma once
#include "CommonInclude.h"
namespace core
{
#define MEMORY_BYTE 32768 //32KB

	typedef unsigned char byte;
	class MemoryPool
	{
	public:
		enum class EMemErrType
		{
			WRONGSIZE = -1,//음수 입력
			POOLSIZEOVER = 0,// (32KB/2) 보다 큰 데이터 할당시.
		};
		struct memory_pool_info
		{
			memory_pool_info();
			memory_pool_info(std::size_t capacity, std::size_t _size);
			~memory_pool_info();
			bool AddMemoryPool(std::size_t _capacity, std::size_t _size);

			byte* current;
			std::vector<byte*> memorys;
		};


		static void Initialize();
		static void Release();

		static void* operator new(std::size_t _size);
		static void operator delete(void* _memory, std::size_t _size);
		static int AssignSize(std::size_t _size);

	private:
		static RBT<std::size_t, memory_pool_info*> m_memory_pools;
		static std::shared_ptr<RBT<std::size_t, memory_pool_info*>> sp_pools;
	};





}

