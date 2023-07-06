#pragma once
#include "Engine.h"
namespace yeram_client
{
	//���߿� datatable�� �������� �ӽ�.test
	struct SkillInfo
	{
		UINT id;
		std::wstring ani_path;
		std::wstring sound_path;
		Vector2 offset;
		float dutation;
	};
	//��ҿ��� ������ ��� �ִٰ� �ʿ��� Scene�� ���� �� 
	//LoadResources �ϱ�.
	class GameObject;
	class Skill
	{
	public:
		Skill();
		virtual ~Skill();
		void SetInfo(SkillInfo& _skillInfo, GameObject* _owner);
		void LoadResources();
		virtual void SetAnimations();
		virtual void Update();
		virtual void Render(HDC _hdc);
		virtual void Play();
		void AddPlay(std::wstring _key,UINT startIndex=0,UINT endIndex=0);
		void ActiveSprite(std::wstring _key);
	protected:
		GameObject* GetOwner() { return mOwner; }
		void SetStartFlag(bool _flag) { mbStart = _flag; }
		std::shared_ptr<GameObject> CreateAnimation(std::wstring _filekey,UINT _startIndex=0, UINT _endIndex=0, std::wstring _addAnikey=L"", float _duration = 0.0f);
	protected:
		std::map<std::string, roka::file::CSVInfo*> mCsvInfos;
		std::map<std::string, roka::file::PackInfo*> mPackInfos;
		std::map<std::wstring, class Animator*> mAnimators;
		std::map<std::wstring, class SpriteRenderer*> mSprites;
		SkillInfo mInfo;
		std::wstring mStartKey;
		bool mbStart;
	private:
		GameObject* mOwner;
	};
}


