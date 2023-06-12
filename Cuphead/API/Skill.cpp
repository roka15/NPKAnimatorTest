#include "Skill.h"
#include "GameObject.h"
namespace yeram_client
{
	Skill::Skill():mbStart(false)
	{
	}
	Skill::~Skill()
	{
	}
	void Skill::SetInfo(SkillInfo& _skillInfo,GameObject* _owner)
	{
		mInfo = _skillInfo;
		mOwner = _owner;
	}
	void Skill::LoadResources()
	{
		roka::file::NPKSystem npk;
		
		std::string ani_path = ws2s(mInfo.ani_path);
		npk.ReadNPK(ani_path, mCsvInfos, mPackInfos);
		
		SetAnimations();
	}
	void Skill::SetAnimations()
	{
		int loop = mCsvInfos.size();
		auto Csvitr = mCsvInfos.begin();
		auto Packitr = mPackInfos.begin();
		for (int i = 0; i < loop; i++)
		{
			std::shared_ptr<GameObject> skill = std::make_shared<GameObject>();
			mOwner->AddChild(skill);
			Animator* ani = skill->AddComponent<Animator>();
			ani->CreateAnimations(L"",Csvitr->second, Packitr->second, mInfo.offset, mInfo.dutation,0,0);
			mAnimators.insert(std::make_pair(s2ws(Csvitr->first), ani));
			Csvitr++;
			Packitr++;
		}
	}
	void Skill::Update()
	{
		if (mbStart == false)
			return;
		for (auto& ani : mAnimators)
		{
			ani.second->Update();
		}
		for (auto& sprite : mSprites)
		{
			if (sprite.second->GetActive() == true)
			{
				sprite.second->Update();
			}
		}
	}
	void Skill::Render(HDC _hdc)
	{
		if (mbStart == false)
			return;
		for (auto& ani : mAnimators)
		{
			ani.second->Render(_hdc);
		}
		for (auto& sprite : mSprites)
		{
			if (sprite.second->GetActive() == true)
			{
				sprite.second->Update();
			}
		}
	}
	void Skill::Play()
	{
		mbStart = true;
		
		mAnimators[mStartKey]->Play(mStartKey, false);
	}
	void Skill::AddPlay(std::wstring _key, UINT startIndex, UINT endIndex)
	{
		Animator* ani = mAnimators[_key];
		if (ani == nullptr)
			return;
		ani->Play(_key,false,startIndex,endIndex);
	}
	void Skill::ActiveSprite(std::wstring _key)
	{
		SpriteRenderer* sprite = mSprites[_key];
		if (sprite == nullptr)
			return;
		sprite->SetActive(true);
	}
	std::shared_ptr<GameObject> Skill::CreateAnimation(std::wstring _filekey,  UINT _startIndex, UINT _endIndex, std::wstring _addAnikey, float _duration)
	{
			std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
			GetOwner()->AddChild(obj);
			std::wstring ResultKey = _filekey + _addAnikey;
			obj->SetName(ResultKey);
			std::string key = ws2s(_filekey);
			roka::file::CSVInfo* csv = mCsvInfos[key];
			roka::file::PackInfo* pack = mPackInfos[key];

			float duration = _duration <= 0.0f ? mInfo.dutation : _duration;
			Animator* ani = obj->AddComponent<Animator>();
			ani->CreateAnimations(_addAnikey,csv, pack, mInfo.offset, duration,_startIndex,_endIndex);
			mAnimators.insert(std::make_pair(ResultKey, ani));

			return obj;
	}
}
