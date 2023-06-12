#include "Arsmagna.h"
#include "GameObject.h"
#include "Script.h"
namespace yeram_client
{
	void Arsmagna::SetAnimations()
	{
		std::string key;
		roka::file::CSVInfo* csv;
		roka::file::PackInfo* pack;

		std::wstring imagename;
		std::wstring filename;
		Transform* tf;
		SpriteRenderer* sprite;

		std::shared_ptr<GameObject> background_a3 = std::make_shared<GameObject>();
		GetOwner()->AddChild(background_a3);
		key = "bga";
		csv = mCsvInfos[key];
		pack = mPackInfos[key];

		imagename = s2ws(pack->binbuf[2]->name);
		filename = s2ws(key) + imagename;
		sprite = background_a3->AddComponent<SpriteRenderer>();
		sprite->CreateImage(filename, imagename, Vector2(csv->size[2]), Vector2(csv->pos[2]), Vector2(csv->base_size), (BYTE*)pack->binbuf[2]->buffer);
		sprite->SetRenderType(ERenderType::TransParentBlt);
		sprite->SetActive(false);
		mSprites.insert(std::make_pair(filename, sprite));
		tf = background_a3->GetComponent<Transform>();
		tf->SetPos(Vector2(0, 300));
		tf->SetScale(Vector2(2.0f, 1.5f));

		std::shared_ptr<GameObject> background_a2 = std::make_shared<GameObject>();
		GetOwner()->AddChild(background_a2);
		key = "bga";
		csv = mCsvInfos[key];
		pack = mPackInfos[key];

		imagename = s2ws(pack->binbuf[1]->name);
		filename = s2ws(key) + imagename;
		sprite = background_a2->AddComponent<SpriteRenderer>();
		sprite->CreateImage(filename, imagename, Vector2(csv->size[1]), Vector2(csv->pos[1]), Vector2(csv->base_size), (BYTE*)pack->binbuf[1]->buffer);
		sprite->SetRenderType(ERenderType::TransParentBlt);
		sprite->SetActive(false);
		mSprites.insert(std::make_pair(filename, sprite));
		tf = background_a2->GetComponent<Transform>();
		tf->SetPos(Vector2(0, 600));
		tf->SetScale(Vector2(2.0f, 1.5f));

		std::shared_ptr<GameObject> background_a1 = std::make_shared<GameObject>();
		GetOwner()->AddChild(background_a1);
		key = "bga";
		csv = mCsvInfos[key];
		pack = mPackInfos[key];

		imagename = s2ws(pack->binbuf[0]->name);
		filename = s2ws(key) + imagename;;

		sprite = background_a1->AddComponent<SpriteRenderer>();
		sprite->CreateImage(filename, imagename, Vector2(csv->size[0]), Vector2(csv->pos[0]), Vector2(csv->base_size), (BYTE*)pack->binbuf[0]->buffer);
		sprite->SetRenderType(ERenderType::TransParentBlt);
		sprite->SetActive(false);
		mSprites.insert(std::make_pair(filename, sprite));
		tf = background_a1->GetComponent<Transform>();
		tf->SetPos(Vector2(0, 450));
		tf->SetScale(Vector2(2.0f, 1.5f));


		//마지막 애니 컴플리트 이벤트에 mstart = false 만드는 이벤트 연결하기.
		//+ owner 의 removechild 해주기.
		CreateAnimation(L"dothomunculus", 0, 14,L"_start", mInfo.dutation + 0.03f);
		mStartKey = s2ws("awakebody_witch_start");
		CreateAnimation(L"awakebody_witch_",0,21,L"start");
		CreateAnimation(L"dotelevator", 0, 0);

		CreateAnimation(L"awakebody_witch_", 0, 21, L"run");
		
		Animator* event_ani = mAnimators[mStartKey];
		event_ani->GetStartEvent(mStartKey)
			= std::bind([this]()->void
				{
					AddPlay(L"dotelevator");
					AddPlay(L"dothomunculus_start");
					ActiveSprite(L"bga0.bmp");
					ActiveSprite(L"bga1.bmp");
					ActiveSprite(L"bga2.bmp");
				});

		event_ani = mAnimators[L"dothomunculus_start"];
		event_ani->GetCompleteEvent(L"dothomunculus_start") =
			std::bind([event_ani]()->
				void 
				{
					event_ani->SetActive(false);
				});
		
	}
	void Arsmagna::Update()
	{
		Skill::Update();
	}
	void Arsmagna::Render(HDC _hdc)
	{
		Skill::Render(_hdc);
	}

	void Arsmagna::Play()
	{
		mbStart = true;
		mAnimators[mStartKey]->Play(mStartKey, false, 0, 21);
	}
	
}