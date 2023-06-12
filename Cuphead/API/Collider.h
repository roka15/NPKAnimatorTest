#pragma once
#include "Component.h"
namespace yeram_client
{
	class Collider :
		public Component
	{
	public:
		Collider();
		virtual ~Collider();
		
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnCollisionEnter(Collider* other);
		virtual void OnCollisionStay(Collider* other);
		virtual void OnCollisionExit(Collider* other);


		void SetCenter(Vector2 center) { mCenter = center; };
		void SetSize(Vector2 size) { mSize = size; };
		Vector2 GetCenter() { return mCenter; }
		Vector2 GetPos() { return mPos; }
		Vector2 GetSize() { return mSize; }
		UINT GetID() { return mID; }
		std::function<void()>& GetEnterEvent() { return mEnterEvent; }
		std::function<void()>& GetStayEvent() { return mStayEvent; }
		std::function<void()>& GetExitEvent() { return mExitEvent; }
	private:
		static UINT ColliderNumber;
		std::function<void()> mEnterEvent;
		std::function<void()> mStayEvent;
		std::function<void()> mExitEvent;
		UINT mCollisionCount;
		UINT mID;
		Vector2 mCenter;
		Vector2 mSize;
		Vector2 mPos;
	};
}

