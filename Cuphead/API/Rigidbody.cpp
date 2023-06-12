#include "Rigidbody.h"
#include "Time.h"
#include "Transform.h"
#include "GameObject.h"
namespace yeram_client
{
	Rigidbody::Rigidbody() :Component(EComponentType::Rigidbody)
		, mMass(0.0f), mAccelation(Vector2::Zero), mVelocity(Vector2::Zero), mForce(Vector2::Zero)
	{
		mLimitedVelocity.x = 200.0f;
		mLimitedVelocity.y = 3600.0f;

		mbGround = true;
		mGravity = Vector2(0.0f, 3000.0f);
		mFriction = 100.0f;
	}
	Rigidbody::~Rigidbody()
	{
	}
	void Rigidbody::Initialize()
	{

	}
	void Rigidbody::Update()
	{   //F = MA
		//A = M/F
		GameObject* owner = GetOwner();
		mAccelation = mForce /mMass;

		mVelocity += mAccelation * Time::DeltaTime();
		if (mbGravity == true)
		{
			if (mbGround)
			{
				Vector2 gravity = mGravity;
				gravity.Normalize();

				float dot = math::Dot(mVelocity, gravity);
				mVelocity -= gravity * dot;
			}
			else
			{ 
				mVelocity += mGravity * Time::DeltaTime();
			}
		}
		else
		{
		
		}
		
		//중력 가속도 최대속도 제한

		Vector2 gravity = mGravity;
		gravity.Normalize();
		float dot = math::Dot(mVelocity, gravity);
		gravity = gravity * dot;

		Vector2 sideVelocity = mVelocity - gravity;
		if (mLimitedVelocity.y < gravity.Length())
		{
			gravity.Normalize();
			gravity *= mLimitedVelocity.y;
		}
		mVelocity.y = gravity.y;
		//가로 속도 제한
		if (mLimitedVelocity.x < sideVelocity.Length())
		{
			sideVelocity.Normalize();
			sideVelocity *= mLimitedVelocity.x;
		}
		mVelocity.x = sideVelocity.x;
		//마찰력 조건 ( 적용된 힘이 없고 , 속도가 0이 아님)
		if (!(mVelocity == Vector2::Zero))
		{
			//속도의 반대방향으로 마찰력이 적용
			Vector2 friction = -mVelocity;
			friction = friction.Normalize() * mFriction * mMass * Time::DeltaTime();

			//마찰력으로 인한 속도 감소는 현재 속도본다 큰 경우
			if (mVelocity.Length() < friction.Length())
			{
				mVelocity = Vector2::Zero;
			}
			else
			{
				mVelocity += friction;
			}
		}

		Transform* tf = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tf->GetPos();
		pos = pos + mVelocity * Time::DeltaTime();
		/*pos += mVelocity * Time::DeltaTime();*/
		if (pos.y > 900.0f)
			mbGround = true;

		tf->SetPos(pos);

		mForce.Clear();
	}
	void Rigidbody::Render(HDC hdc)
	{

	}
	void Rigidbody::Release()
	{
	}
	void Rigidbody::AddForce(Vector2 _force)
	{
		mForce += _force;
	}
}