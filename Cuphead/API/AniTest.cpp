#include "AniTest.h"

void yeram_client::AniTest::SetAnimations()
{
	std::string key;
	roka::file::CSVInfo* csv;
	roka::file::PackInfo* pack;
	key = "summer_2018_seria.img";
	csv = mCsvInfos[key];
	pack = mPackInfos[key];

	CreateAnimation(L"summer_2018_seria.img");
	mStartKey = s2ws(key);
}

void yeram_client::AniTest::Update()
{
	Skill::Update();
}

void yeram_client::AniTest::Render(HDC _hdc)
{
	Skill::Render(_hdc);
}

void yeram_client::AniTest::Play()
{
	Skill::Play();
}
