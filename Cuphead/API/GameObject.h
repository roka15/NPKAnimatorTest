#pragma once
#include "Component.h"
#include "Image.h"
#include "SceneManager.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Script.h"
#include "Audio.h"
#include "Animator.h"
#include "SpriteRenderer.h"

namespace yeram_client
{
	class GameObject :public Entity
	{
	public:
		GameObject();
		virtual ~GameObject();

		virtual void Initialize();
		virtual void Update();
		virtual void Render(HDC hdc);
		virtual void Release();


		template<typename T>
		static inline T* Instantiate(GameObject* _parent = nullptr)
		{
			T* obj = new T();
			if (_parent != nullptr)
			{
				obj->SetParent(_parent);
				_parent->AddChild(std::shared_ptr<T>(obj));
			}
			return obj;
		}
		template<typename T>
		static inline T* Instantiate(Vector2 _pos, GameObject* _parent = nullptr)
		{
			T* obj = new T();
			obj->SetParent(_parent);
			GameObject* obj_parent = obj->GetParent();
			if (obj_parent != nullptr)
			{
				obj_parent->AddChild(std::shared_ptr<T>(obj));
			}
			obj->GameObject::GetComponent<Transform>()->SetPos(_pos);
			return obj;
		}
		template<typename T>
		static inline T* Instantiate(const std::wstring _name, Vector2 _pos, GameObject* _parent = nullptr)
		{
			T* obj = new T();
			obj->SetName(_name);
			GameObject* obj_parent = obj->GetParent();
			if (obj_parent != nullptr)
			{
				obj_parent->AddChild(std::shared_ptr<T>(obj));
			}
			obj->GameObject::GetComponent<Transform>()->SetPos(_pos);
			return obj;
		}
		static inline std::shared_ptr<GameObject> Find(std::wstring _name)
		{
			Scene* cur = SceneManager::GetActiveScene();

			return cur->FindObject(_name);
		}
		static void Destroy(GameObject* _obj)
		{
			if (_obj == nullptr)
				return;
			Scene* scene = SceneManager::GetActiveScene();
			std::vector<std::shared_ptr<GameObject>>& objs = scene->GetGameObjects(_obj->GetLayerType());
			for (std::vector<std::shared_ptr<GameObject>>::iterator itr = objs.begin(); itr != objs.end(); itr++)
			{
				if ((*itr).get() == _obj)
					objs.erase(itr);
			}

			delete _obj;
			//object pool 완성시 pool에 반납.
		}
		static void Destroy(GameObject* _obj, float _time)
		{

		}

		template <typename T>
		T* AddComponent()
		{
			T* comp = new T();
			EComponentType type = comp->GetType();
			if (type != EComponentType::Script)
			{
				if (mComponents[type].size() != 0)
				{
					delete comp;
					return dynamic_cast<T*>(mComponents[type][0]);
				}
			}
			else
			{
				for (int i = 0; i < mComponents[type].size(); i++)
				{
					T* data = dynamic_cast<T*>(mComponents[type][i]);
					if (data != nullptr)
					{
						delete comp;
						return data;
					}
				}
			}
			mComponents[type].push_back(comp);
			comp->SetOwner(this);
			comp->Initialize();
			return comp;
		}
		template <typename T>
		T* GetComponent()
		{
			if (std::is_base_of<Script, T>())
			{
				return GetComponentScript<T>();
			}
			else
			{
				return GetComponentNomal<T>();
			}
		}
		template <typename T>
		void RemoveComponent()
		{
			EComponentType type = EComponentType::Script;
			if (std::is_same<Script, T>())
			{
				for (int i = 0; i < mComponents[type].size(); i++)
				{
					//script->Release();
					mComponents[type][i]->Release();
					delete mComponents[type][i];
					mComponents[type][i] = nullptr;
				}
				mComponents[type].resize(0);
				return;
			}
			else if (std::is_base_of<Script, T>())
			{
				for (auto i = mComponents[type].begin(); i < mComponents[type].end(); i++)
				{
					if (dynamic_cast<T*>(*i) != nullptr)
					{
						//script->Release();
						(*i)->Release();
						delete (*i);
						(*i) = nullptr;
						mComponents[type].erase(i);
						return;
					}
				}

			}

			for (int i = 0; i < mComponents.size(); i++)
			{
				type = (EComponentType)i;
				if (type == EComponentType::Script || type == EComponentType::NONE)
					continue;
				if (mComponents[type].size() != 0)
				{
					T* data = dynamic_cast<T*>(mComponents[type][0]);
					if (data != nullptr)
					{
						//mcomp[i][0]->Release();
						mComponents[type][0]->Release();
						delete mComponents[type][0];
						mComponents[type][0] = nullptr;
						mComponents[type].resize(0);
						return;
					}
				}
			}
		}

		virtual void InitComponent() {};

		template<typename T>
		void SetActive(bool _flag)
		{
			T* com = GetComponent<T>();
			if (com == nullptr)
				return;
			com->SetActive(_flag);
		}
		template<typename T>
		const bool& GetActive()
		{
			T* com = GetComponent<T>();
			//예외처리 안한 이유는 없는 컴포넌트일때 false 반환이 아니라 터지게 하려고 했음.

			return com->GetActive();
		}
		void SetActive(bool _flag)
		{
			mbActive = _flag;

		   /*for (std::shared_ptr<GameObject> child : mChilds)
			{
				if (child == nullptr)
					continue;
				child->SetActive(_flag);
			}*/

			//for (auto map : mComponents)
			//{
			//	for (auto comp : map.second)
			//	{
			//		comp->SetActive(_flag);
			//	}
			//}
		}
		const bool& GetActive()
		{
			return mbActive;
		}
		void SetParent(GameObject* _obj);
		GameObject* GetParent() { return mParent; }
		std::shared_ptr<GameObject> FindChild(std::wstring _name);
		std::shared_ptr<GameObject> FindChild(UINT _index);
		void SetLayerType(ELayerType _type) { mLayerType = _type; }
		ELayerType GetLayerType() { return mLayerType; }

		void AddChild(std::shared_ptr<GameObject> _child);
	
		UINT GetChildCount() { return mChilds.size(); }
		void RemoveChild(std::shared_ptr<GameObject> _child);
		void RemoveChilds();

		
		virtual void OnCollisionEnter(class Collider* other);
		virtual void OnCollisionStay(class Collider* other);
		virtual void OnCollisionExit(class Collider* other);

		virtual void MoveObjectSetting(float _speed, EDirType _dir_type = EDirType::LEFT);

		void SetCameraMoveActive(bool _flag) { mbCameraMove = _flag; }
		bool GetCameraMoveActive() { return mbCameraMove; }
		//자식도 같이 move
		//void MoveChild(const Vector2& _offset);
	protected:
		HBRUSH brush;
		HPEN pen;
		bool flag;
		//Image* mImage;

	private:
		template <typename T>
		T* GetComponentScript()
		{
			for (Component* script : mComponents[EComponentType::Script])
			{
				T* data = dynamic_cast<T*>(script);
				if (data != nullptr)
				{
					return data;
				}
			}
			return nullptr;
		}
		template <typename T>
		T* GetComponentNomal()
		{

			for (auto comp : mComponents)
			{
				if (comp.second.empty() == true)
					continue;
				if (dynamic_cast<T*>(comp.second[0]))
				{
					return dynamic_cast<T*>(comp.second[0]);
				}
			}
			return nullptr;
		}
	private:
		std::map<EComponentType, std::vector<Component*>> mComponents;
		std::vector<std::shared_ptr<GameObject>> mChilds;
		GameObject* mParent;
		bool mbActive;
		bool mbCameraMove;
		ELayerType mLayerType;
	};
}

