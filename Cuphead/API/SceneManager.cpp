#include "SceneManager.h"

#include "TitleScene.h"

#include "Camera.h"
#include "Rectangle.h"

#include "Animator.h"
#include "Time.h"


#include "ObjectPool.h"
namespace yeram_client
{
	std::vector<Scene*> SceneManager::mScenes = {};
	Scene* SceneManager::mActiveScene = nullptr;
	GameObject* SceneManager::mLoadingScreen = nullptr;
	bool SceneManager::mbLoadScreenFlag;
	HANDLE SceneManager::mHThread;
	ESceneType SceneManager::mLoadSceneType;
	SceneManager::ELoadingState SceneManager::mLoadState;
	bool SceneManager::mbCompleteLoad;
	std::function<void()> SceneManager::mLoadMessageEvent;
	bool SceneManager::mbUseUI;
	std::queue<SceneManager::RemoveObjectInfo> SceneManager::mRemoveRequestObjs;

	void SceneManager::Initalize()
	{

		mScenes.resize((UINT)ESceneType::MAX);
		mScenes[(UINT)ESceneType::Title] = new TitleScene(L"Title");


		for (Scene* scene : mScenes)
		{
			if (scene == nullptr)
				continue;
			scene->Initialize();
		}


		core::ObjectPool<Animator>::Initialize();
		mActiveScene = mScenes[(UINT)ESceneType::Title];
		mActiveScene->OnEnter();

	}

	void SceneManager::Update()
	{
		mActiveScene->Update();
	}

	void SceneManager::Render(HDC hdc)
	{
		mActiveScene->Render(hdc);
	}

	void SceneManager::Release()
	{
		for (Scene* scene : mScenes)
		{
			if (scene == nullptr)
				continue;
			scene->Release();
			delete scene;
			scene = nullptr;
		}

		core::ObjectPool<Animator>::Release();

		//core::ObjectPool<Ground>::Release();
	}

	void SceneManager::LoadScene(ESceneType _type)
	{

	}

	Scene* SceneManager::GetActiveScene()
	{
		return mActiveScene;
	}

	std::shared_ptr<GameObject> SceneManager::FindObject(std::wstring _name)
	{
		return mActiveScene->FindObject(_name);
	}

	void SceneManager::RemoveObject(GameObject* _obj)
	{
		mActiveScene->RemoveGameObject(_obj);
	}

	void SceneManager::RemoveObject(GameObject* _obj, ELayerType _type)
	{
		mActiveScene->RemoveGameObject(_obj, _type);
	}

	void SceneManager::RemoveObject(ELayerType _type)
	{
		mActiveScene->RemoveGameObject(_type);
	}

	void SceneManager::ChagePosGameObjects(const Vector2& _offset)
	{
		mActiveScene->ChagePosGameObjects(_offset);
	}

	void SceneManager::ChangeScaleGameObjects(const Vector2& _scale)
	{
		mActiveScene->ChageScaleGameObjects(_scale);
	}

	void SceneManager::OpenLodingScreen()
	{
		mLoadingScreen->SetActive(true);
		mbLoadScreenFlag = true;
		Animator* ani = mLoadingScreen->GetComponent<Animator>();
		ani->Play(L"LoadingOpen", false);
	}

	void SceneManager::CloseLodingScreen()
	{
		Animator* ani = mLoadingScreen->GetComponent<Animator>();
		ani->Play(L"LoadingClose", false);
	}

	void SceneManager::SetActivScene()
	{
		yeram_client::Scene* cur = yeram_client::SceneManager::GetActiveScene();
		mActiveScene = mScenes[(UINT)mLoadSceneType];
		if (cur != nullptr)
			cur->OnExit();
		mActiveScene->OnEnter();
	}

	void SceneManager::ChageLayer(GameObject* _obj, ELayerType _add)
	{
		std::shared_ptr<GameObject> obj = mActiveScene->FindObject(_obj->GetName());
		RemoveObjectRequest(obj.get(), obj->GetLayerType());
		mActiveScene->AddGameObject(obj, _add);
	}



	void SceneManager::RemoveObjectRequestRelease()
	{
		while (mRemoveRequestObjs.empty() == false)
		{
			RemoveObjectInfo remove = mRemoveRequestObjs.front();
			mRemoveRequestObjs.pop();
			if (remove.mtype == ELayerType::NONE)
			{
				RemoveObject(remove.mobj);
			}
			else if (remove.mobj == nullptr)
			{
				RemoveObject(remove.mtype);
			}
			else
			{
				RemoveObject(remove.mobj, remove.mtype);
			}

		}
	}

	SceneManager::~SceneManager()
	{
		CloseHandle(mHThread);
	}

}
