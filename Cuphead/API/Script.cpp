#include "Script.h"

namespace yeram_client
{
	Script::Script() :Component(EComponentType::Script)
	{
		
	}
	Script::~Script()
	{
	}
	void Script::Initialize()
	{
	}

	void Script::Update()
	{

	}

	void Script::Render(HDC hdc)
	{
		Component::Render(hdc);
	}

	void Script::Release()
	{

	}
	

	void Script::OnCollisionEnter(Collider* other)
	{
	}
	void Script::OnCollisionStay(Collider* other)
	{
	}
	void Script::OnCollisionExit(Collider* other)
	{
	}
	
}
