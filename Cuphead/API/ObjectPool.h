#pragma once
#include "Rectangle.h"

namespace core
{
	typedef yeram_client::Transform Transform;

	template <typename T>
	class ObjectPool
	{
	public:
		static bool ActiveObjectPool();
		static void Initialize(size_t _capacity = 100, size_t _max_capacity = 500);
		static void Initialize(yeram_client::GameObject* _origin, size_t _capacity = 100, size_t _max_capacity = 500);
		static void Release();
		static std::shared_ptr<yeram_client::GameObject> Spawn();
		static std::shared_ptr<yeram_client::GameObject> Spawn2();
		static void DeSpawn(yeram_client::GameObject* _obj);
	private:
		static bool UpgradePoolSize();
		ObjectPool() = delete;
		~ObjectPool() = delete;
	private:
		static bool mbInit;
		static size_t mcapacity;
		static size_t mlimit_capacity;
		static std::queue<yeram_client::GameObject*> mpools;
		static yeram_client::GameObject* mOrigin;
	};
	template<typename T>
	bool ObjectPool<T>::mbInit = false;
	template <typename T>
	size_t ObjectPool<T>::mcapacity = 0;
	template <typename T>
	std::queue<yeram_client::GameObject*> ObjectPool<T>::mpools;
	template <typename T>
	size_t ObjectPool<T>::mlimit_capacity = 0;
	template <typename T>
	yeram_client::GameObject* ObjectPool<T>::mOrigin = nullptr;
	

	template<typename T>
	inline bool ObjectPool<T>::ActiveObjectPool()
	{
		return mbInit == true;
	}

	template<typename T>
	inline void ObjectPool<T>::Initialize(size_t _capacity, size_t _max_capacity)
	{
		mbInit = true;
		mcapacity = _capacity;
		mlimit_capacity = _max_capacity;
		int temp_min = mcapacity < mlimit_capacity ? mcapacity : mlimit_capacity;
		for (int i = 0; i < temp_min; i++)
		{
			yeram_client::GameObject* obj = yeram_client::GameObject::Instantiate<yeram_client::GameObject>();
			obj->AddComponent<T>();
			obj->Initialize();
			mpools.push(obj);
		}
	}
	template<typename T>
	inline void ObjectPool<T>::Initialize(yeram_client::GameObject* _origin, size_t _capacity, size_t _max_capacity)
	{
		mbInit = true;
		mcapacity = _capacity;
		mlimit_capacity = _max_capacity;
		int temp_min = mcapacity < mlimit_capacity ? mcapacity : mlimit_capacity;
		for (int i = 0; i < temp_min; i++)
		{
			yeram_client::GameObject* obj = nullptr;
			if (dynamic_cast<yeram_client::Rectangle*>(_origin) != nullptr)
			{
				obj = yeram_client::GameObject::Instantiate<yeram_client::Rectangle>();
			}
			else
			{
				obj = yeram_client::GameObject::Instantiate<yeram_client::GameObject>();
			}
			mOrigin = _origin;
			obj->AddComponent<T>();
			obj->Initialize();
			mpools.push(obj);
		}
	}
	template<typename T>
	inline void ObjectPool<T>::Release()
	{
		while (mpools.empty() == false)
		{
			yeram_client::GameObject* obj = mpools.front();
			mpools.pop();
			obj->Release();
			delete obj;
		}
		mpools.~queue();
		if (mOrigin != nullptr)
		{
			mOrigin->Release();
			delete mOrigin;
			mOrigin = nullptr;
		}
		mbInit = false;
	}
	template<typename T>
	inline std::shared_ptr<yeram_client::GameObject> ObjectPool<T>::Spawn()
	{
		if (mpools.size() <= 0)
		{
			if (UpgradePoolSize() == false)
				return nullptr;
		}
		

		std::shared_ptr<yeram_client::GameObject> obj(mpools.front(), DeSpawn);
		mpools.pop();
		obj->SetActive(true);
		return obj;
	}
	template<typename T>
	inline std::shared_ptr<yeram_client::GameObject> ObjectPool<T>::Spawn2()
	{
		if (mpools.size() <= 0)
		{
			UpgradePoolSize();
		}

		std::shared_ptr<yeram_client::GameObject> obj(mpools.front(), DeSpawn);
		mpools.pop();
		obj->SetActive(true);
		return obj;
	}
	template<typename T>
	inline void ObjectPool<T>::DeSpawn(yeram_client::GameObject* _obj)
	{
		if (_obj == nullptr)
			return;

		/*T* obj = dynamic_cast<T*>(_obj);
		if (obj == nullptr)
			return;*/
		_obj->Release();
		_obj->Initialize();
		_obj->SetActive(false);
		_obj->RemoveChilds();
		mpools.push(_obj);
	}
	template<typename T>
	inline bool ObjectPool<T>::UpgradePoolSize()
	{
		if (mpools.size() == 0)
		{
			int add_cnt = mcapacity == 1 ? mcapacity + 1 : mcapacity + (mcapacity / 2);

			if (add_cnt > mlimit_capacity)
				return false;
			yeram_client::GameObject* obj = nullptr;
			for (int i = mcapacity; i < add_cnt; i++)
			{
				if (dynamic_cast<yeram_client::Rectangle*>(mOrigin) != nullptr)
				{
					obj = yeram_client::GameObject::Instantiate<yeram_client::Rectangle>();
				}
				else
				{
					obj = yeram_client::GameObject::Instantiate<yeram_client::GameObject>();
				}
				obj->AddComponent<T>();
				mpools.push(obj);
			}


			mcapacity = add_cnt;
			return true;
		}
	}
}


