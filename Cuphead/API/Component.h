#pragma once
#include "Entity.h"
namespace yeram_client
{
	class GameObject;
	class Component:public Entity
	{
	public:
		Component()=delete;
		Component(EComponentType _type);
		virtual ~Component();
		virtual void Initialize();
		virtual void Update();
		virtual void Render(HDC hdc);
		virtual void Release();
		const EComponentType& GetType() { return mType; }
		void SetOwner(GameObject* _owner) { mOwner = _owner; }
		GameObject* GetOwner() { return mOwner; }
		const bool& GetActive() { return mbActive; }
		void SetActive(const bool _flag) 
		{
			mbActive = _flag; 
		}
	
	protected:
		bool mbActive;
		const EComponentType mType;
		GameObject* mOwner;
	
	};
}


