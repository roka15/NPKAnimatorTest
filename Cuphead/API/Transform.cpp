#include "Transform.h"
#include "GameObject.h"
yeram_client::Transform::Transform() :Component(EComponentType::Transform)
, mPos(Vector2::Zero)
, mScale(Vector2::One)
, mSize(Vector2::Zero)
{
	SetName(L"Transform");
}

yeram_client::Transform::~Transform()
{
}

void yeram_client::Transform::Initialize()
{
	mbActive = true;
	mPos = Vector2::Zero;
	mScale = Vector2::One;
	mSize = Vector2::Zero;
}

void yeram_client::Transform::Update()
{
	/*GameObject* parent = GetOwner()->GetParent();
	if (parent != nullptr)
	{
		mPos = parent->GetComponent<Transform>()->GetPos();
		mPos += mSize;
	}*/
	GameObject* owner = GetOwner();
	int size = owner->GetChildCount();

	GameObject* parent =owner->GetParent();
	Vector2 pos=Vector2::Zero;
	if (parent != nullptr)
	{
		Transform* p_tf = parent->GetComponent<Transform>();
		pos = p_tf->GetPos();
	}
	mPos =mOriginPos + pos;
}

void yeram_client::Transform::Render(HDC hdc)
{
	if (mbActive == false)
		return;
}

void yeram_client::Transform::Release()
{
}

void yeram_client::Transform::SetPos(Vector2 _pos)
{
	/*GameObject* parent = GetOwner()->GetParent();
	if (parent != nullptr)
	{
		mPos = parent->GetComponent<Transform>()->GetPos();
		mPos += _pos;
	}
	else
	{*/
	mOriginPos = _pos;
	//}
}

void yeram_client::Transform::SetScale(Vector2 _scale)
{
	mScale = _scale;
	int size = mOwner->GetChildCount();
	for (int i = 0; i < size; i++)
	{
		Transform* tf = mOwner->FindChild(i)->GetComponent<Transform>();
		tf->SetScale(_scale);
		Vector2 offset = tf->GetOffset();
		offset *= _scale;
		tf->SetOffset(offset);
	}
}

void yeram_client::Transform::CaluatePos(const Vector2& _offset)
{
	int size = mOwner->GetChildCount();
	/*for (int i = 0; i < size; i++)
	{
		Vector2& pos = mOwner->FindChild(i)->GetComponent<Transform>()->GetPos();
		pos-=_offset;
	}*/
	mPos -= _offset;
}

void yeram_client::Transform::CaluateSclae(const Vector2& _value)
{
	int size = mOwner->GetChildCount();
	for (int i = 0; i < size; i++)
	{
		mOwner->FindChild(i)->GetComponent<Transform>()->CaluateSclae(_value);
	}
	GameObject* parent = mOwner->GetParent();
	if (parent != nullptr)
	{
		Transform* tf = parent->GetComponent<Transform>();
		Vector2 point = tf->GetPos();

		Vector2 offset = (point - mPos);
		offset.x = -offset.x;
		offset.y = -offset.y;
		mPos = point + (offset * _value);
	}
	mScale *= _value;
}


