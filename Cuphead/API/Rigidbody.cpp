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
		
		//�߷� ���ӵ� �ִ�ӵ� ����

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
		//���� �ӵ� ����
		if (mLimitedVelocity.x < sideVelocity.Length())
		{
			sideVelocity.Normalize();
			sideVelocity *= mLimitedVelocity.x;
		}
		mVelocity.x = sideVelocity.x;
		//������ ���� ( ����� ���� ���� , �ӵ��� 0�� �ƴ�)
		if (!(mVelocity == Vector2::Zero))
		{
			//�ӵ��� �ݴ�������� �������� ����
			Vector2 friction = -mVelocity;
			friction = friction.Normalize() * mFriction * mMass * Time::DeltaTime();

			//���������� ���� �ӵ� ���Ҵ� ���� �ӵ����� ū ���
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