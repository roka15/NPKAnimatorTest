#pragma once
#include "Component.h"
namespace yeram_client
{
	class Transform :public Component
	{
	public:
		Transform();
		~Transform();
		virtual void Initialize()override;
		virtual void Update()override;
		virtual void Render(HDC hdc)override;
		virtual void Release()override;

		void SetPos(Vector2 _pos);
		void SetScale(Vector2 _scale); 
		void SetSize(Vector2 _size) { mSize = _size; }
		void SetOffset(Vector2 _offset) { mOffset = _offset; }
		Vector2& GetPos() { return mPos; }
		Vector2& GetScale() { return mScale; }
		Vector2& GetSize() { return mSize; }
		const Vector2& GetOffset() { return mOffset; }
		void CaluatePos(const Vector2& _offset);
		void CaluateSclae(const Vector2& _value);
		void SetPivot(Vector2 _pivot) { mpivot = _pivot; }
		Vector2 GetPivot() { return mpivot; }
	private:
		Vector2 mOriginPos;
		Vector2 mPos;
		Vector2 mScale;
		Vector2 mSize;
		Vector2 mOffset;	
		Vector2 mpivot;
	};
}


