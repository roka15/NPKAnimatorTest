#include "TitleScene.h"
#include "Input.h"
#include "SceneManager.h"
#include "Image.h"
#include "Resources.h"
#include "Application.h"
#include "Rectangle.h"

#include "Transform.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Animator.h"
#include "Layer.h"
#include "ObjectPool.h"
#include "Skill.h"
#include "Arsmagna.h"
extern yeram_client::Application application;

yeram_client::TitleScene::TitleScene()
{
	mCurType = ESceneType::Title;
}

yeram_client::TitleScene::TitleScene(std::wstring _name)
{
	mCurType = ESceneType::Title;
	SetName(_name);
}

yeram_client::TitleScene::~TitleScene()
{
}

void yeram_client::TitleScene::Initialize()
{
	mLayers.resize((UINT)ELayerType::MAX);
	mLayers[(UINT)ELayerType::BackObject] = new Layer();
	mLayers[(UINT)ELayerType::Player] = new Layer();
	mLayers[(UINT)ELayerType::FrontObject] = new Layer();
	Vector2 pos = application.GetWindowSize() / 2.0f;
	
	
}

void yeram_client::TitleScene::Update()
{
	if (core::Input::GetKeyDown(core::EKeyCode::N))
	{
		SkillTest();
	}
	//if(skill->Get)
	Scene::Update();
}

void yeram_client::TitleScene::Render(HDC hdc)
{
	Scene::Render(hdc);
}

void yeram_client::TitleScene::Release()
{
	Scene::Release();
}
void yeram_client::TitleScene::SkillTest()
{
	skill->Play();
}
void yeram_client::TitleScene::OnEnter()
{
	Vector2 size = application.GetWindowSize();

	Image* image = Resources::Load<Image>(L"bmpimage",L"..\\Resources\\0000.bmp");
	SkillInfo info = {};
	info.ani_path = L"..\\Resources\\ImagePackage\\arsmagna.npk";
	info.offset = Vector2::Zero;
	info.dutation = 0.05f;
    
	player=std::make_shared<GameObject>();
	Transform* tf = player->GetComponent<Transform>();
	tf->SetPos(Vector2{ 800.0f,650.0f });
	skill = new Arsmagna();
	skill->SetInfo(info,player.get());
	skill->LoadResources();
	

	AddGameObject(player, ELayerType::BackObject);
	Scene::OnEnter();
}

void yeram_client::TitleScene::OnExit()
{
	Scene::OnExit();
}
