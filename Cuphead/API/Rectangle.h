#pragma once
#include "GameObject.h"

namespace yeram_client
{
	class Rectangle:public GameObject
	{
	public:
		Rectangle();
		~Rectangle();
		virtual void Initialize();
		virtual void Update();
		virtual void Render(HDC hdc);
		virtual void Release();
		/*void SetPos(Vector2 _start, Vector2 _end)
		{
			mStartPos = _start;
			mEndPos = _end;
			offset.x = std::abs(_start.x - _end.x);
			offset.y = std::abs(_start.y - _end.y);
		}*/
		void aniCompleteEvent();

		virtual void OnCollisionEnter(class Collider* other)override;
		virtual void OnCollisionStay(class Collider* other)override;
		virtual void OnCollisionExit(class Collider* other)override;
	private:
		/*Vector2 mStartPos;
		Vector2 mEndPos;
		Vector2 offset;*/
	    // color 클래스 만들어서 멤버 변수로 두기.
	};
}


