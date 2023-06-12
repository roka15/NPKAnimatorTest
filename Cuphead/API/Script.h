#pragma once
#include "Component.h"
namespace yeram_client
{
	class Script :public Component
	{
	public:
		Script();
		virtual ~Script();
		virtual void Initialize()override;
		virtual void Update();
		virtual void Render(HDC hdc)override;
		virtual void Release()override;
		
		virtual void OnCollisionEnter(class Collider* other);
		virtual void OnCollisionStay(class Collider* other);
		virtual void OnCollisionExit(class Collider* other);
		
		
	private:
		
	};
}


