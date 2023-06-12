#pragma once
#include "Component.h"
namespace yeram_client
{
	class Rigidbody : public Component
	{
	public:
		Rigidbody();
		~Rigidbody();

		virtual void Initialize()override;
		virtual void Update()override;
		virtual void Render(HDC hdc)override;
		virtual void Release()override;

		void SetMass(float _mass) { mMass = _mass; }
		void AddForce(Vector2 _force);
		void SetGround(bool _ground) { mbGround = _ground; }
		void SetVelocity(Vector2 _velocity) 
		{ 
			mVelocity =_velocity; 
		}
		void Use_Gravity(bool _flag) { mbGravity = _flag; }
		void SetGravity(float _gravity) { mGravity.y = _gravity; }
		Vector2 GetVelocity() { return mVelocity; }
		bool GetGround() { return mbGround; }
	private:
		float mMass;
		Vector2 mForce;
		Vector2 mAccelation;
		Vector2 mVelocity;
		Vector2 mLimitedVelocity;

		Vector2 mGravity;
		
		float mFriction;
		bool mbGround;
		bool mbGravity;

		/*
		float mStaticFriction;
		float mKineticFriction;
		float mCoefficientFriction;

		float mNormalForce;
		*/
	};

}

