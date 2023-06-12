#include "Audio.h"
#include "Resources.h"
yeram_client::Audio::Audio():Component(EComponentType::Audio)
{
	SetName(L"Audio");
}
yeram_client::Audio::~Audio()
{
}
void yeram_client::Audio::Render(HDC hdc)
{
	if (mbActive == false)
		return;
}

void yeram_client::Audio::Play(std::wstring _key, std::wstring _path,bool _flag,int _volume)
{
	Sound* find_sound = Resources::Find<Sound>(_key);
	if (find_sound == nullptr)
	{
		find_sound = Resources::Load<Sound>(_key, _path);
	}
	find_sound->Play(_flag);
}
