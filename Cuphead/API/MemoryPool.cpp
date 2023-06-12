#include "MemoryPool.h"

namespace core
{

RBT<std::size_t, MemoryPool::memory_pool_info*> MemoryPool::m_memory_pools;
std::shared_ptr<RBT<std::size_t, MemoryPool::memory_pool_info*>> MemoryPool::sp_pools(&m_memory_pools
	, [](void*)->void {std::cout << "release" << std::endl; MemoryPool::Release(); });

MemoryPool::memory_pool_info::memory_pool_info() :current(nullptr)
{
}

MemoryPool::memory_pool_info::memory_pool_info(std::size_t _capacity, std::size_t _size) : current(nullptr)
{
	AddMemoryPool(_capacity, _size);
}
bool MemoryPool::memory_pool_info::AddMemoryPool(std::size_t _capacity, std::size_t _size)
{
	current = (byte*)malloc(_capacity);

	if (current == nullptr)
	{
		//err 발생 시키기.
		return false;
	}

	memset(current, 0, _capacity);
	int cnt = _capacity / _size;
	//32KB 블럭이 한개 이상일때 이전 블럭의 마지막 주소에 현재 새로 만든 블럭 주소를 연결.
	if (memorys.size() != 0)
	{
		byte* befor_mem = memorys[memorys.size() - 1];
		befor_mem += (_size * (cnt - 1));
		if (*befor_mem == '\0')
		{
			memcpy(befor_mem, &current, sizeof(byte*));
		}
	}

	byte* curptr = current;
	byte* nextptr = nullptr;
	for (int i = 0; i < cnt - 1; i++)
	{
		nextptr = curptr + _size;
		memcpy(curptr, &nextptr, sizeof(byte*));
		curptr = nextptr;
	}
	*curptr = '\0'; // 메모리 마지막 구분.

	memorys.push_back(current);
	return true;
}
MemoryPool::memory_pool_info::~memory_pool_info()
{
	for (int i=0;i<memorys.size();i++) 
	{
		delete memorys[i];
		memorys[i] = nullptr;
	}
	current = nullptr;
	memorys.clear();
	memorys.~vector();
}

void MemoryPool::Initialize()
{
}

void MemoryPool::Release()
{
	for (auto pool_info = m_memory_pools.begin();pool_info!=m_memory_pools.end();pool_info++)
	{
		delete (*pool_info);
		(*pool_info) = nullptr;
	}
	/* 하고 싶었던것 for(auto pool_info : m_memory_pools) */
	m_memory_pools.clear();
}

void* MemoryPool::operator new(std::size_t _size)
{
	int size = AssignSize(_size);
	if (size == (int)EMemErrType::POOLSIZEOVER)
	{
		return malloc(_size); // 단일 메모리 할당.
	}
	else if (size == (int)EMemErrType::WRONGSIZE)
	{
		return nullptr;
	}
	
	if (m_memory_pools.find(size) == m_memory_pools.end())
	{
		m_memory_pools.insert(std::make_pair(size, new memory_pool_info(MEMORY_BYTE, size)));
	}
	else if (m_memory_pools[size]->current == nullptr)
	{
		m_memory_pools[size]->AddMemoryPool(MEMORY_BYTE, size);
	}
	//success code
	byte* return_ptr = m_memory_pools[size]->current;
	byte* next_ptr = *reinterpret_cast<byte**>(return_ptr);
	//fail code 
	//자아성찰 링크 :https://ramingstudy.tistory.com/73
	/*
	byte* next_ptr = reinterpret_cast<byte*>(*(m_memory_pools[size].current));
	byte* return_ptr = m_memory_pools[size].current;
	*/
	m_memory_pools[size]->current = next_ptr;

	return return_ptr;
}

void MemoryPool::operator delete(void* _memory, std::size_t _size)
{
	int size = AssignSize(_size);
	if (size == (int)EMemErrType::POOLSIZEOVER)
	{
		free(_memory); // 단일 메모리 할당.
		return;
	}
	else if (size == (int)EMemErrType::WRONGSIZE)
	{
		return;
	}


	byte* next_ptr = m_memory_pools[size]->current;
	memcpy(_memory, &m_memory_pools[size]->current, sizeof(byte*));
	m_memory_pools[size]->current = reinterpret_cast<byte*>(_memory);
}

int MemoryPool::AssignSize(std::size_t _size)
{
	if (_size * 2 > MEMORY_BYTE)
	{
		return static_cast<int>(EMemErrType::POOLSIZEOVER);
	}
	int befor = 0;
	for (int i = 8; i <= MEMORY_BYTE / 2; i *= 2)
	{
		if (_size > befor && _size <= i)
		{
			return i;
		}
		befor = i;
	}

	return static_cast<int>(EMemErrType::WRONGSIZE);
}

}