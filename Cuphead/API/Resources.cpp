#include "Resources.h"
#include "Time.h"
namespace yeram_client
{
	std::map<std::wstring, My_Resource*> Resources::mResources = {};
	double Resources::mTime = 0.0;
	
	void Resources::Update()
	{
		mTime += Time::DeltaTime();
	}
}