#include "Scene.h"
#include "Application.h"
#include "Layer.h"
#include "ResourceDeleterThread.h"
extern yeram_client::Application application;
namespace yeram_client
{
	void Scene::Initialize()
	{
		for (Layer* layer : mLayers)
		{
			if (layer == nullptr)
				continue;
			layer->Initialize();
		}
	}

	void Scene::Update()
	{
		for (Layer* layer : mLayers)
		{
			if (layer == nullptr)
				continue;
			layer->Update();
		}
	}

	void Scene::Render(HDC hdc)
	{
		for (Layer* layer : mLayers)
		{
			if (layer == nullptr)
				continue;
			if (layer->GetActive() == false)
				continue;
			layer->Render(hdc);
		}
	}

	void Scene::Release()
	{
		for (Layer* layer : mLayers)
		{
			if (layer == nullptr)
				continue;
			layer->Release();
			delete layer;
			layer = nullptr;
		}
		mLayers.clear();
	}

	void Scene::OnEnter()
	{
		core::ResourceDeleterThread::SetResourceQueueCapacity(MAX_RESOURCE);
	}

	void Scene::OnExit()
	{
		Layer* cur_layer = mLayers[(UINT)ELayerType::Player];
		if (cur_layer != nullptr)
		{
			Scene* active = SceneManager::GetActiveScene();
			Layer* active_layer = active->mLayers[(UINT)ELayerType::Player];
			//player 정보는 다른씬에 옮겨준다.
			std::vector<std::shared_ptr<GameObject>>& recv_obj = active_layer->GetGameObjectList();
			if (recv_obj.size() == 0)
			{
				std::vector<std::shared_ptr<GameObject>>& send_obj = cur_layer->GetGameObjectList();
				for (auto player_obj : send_obj)
				{
					recv_obj.push_back(player_obj);
				}
				send_obj.clear();
			}
		}
		//해당 씬에서 사용한 오브젝트들 반납.
		for (auto layer : mLayers)
		{
			if (layer == nullptr)
				continue;
			layer->Release();
		}
		core::ResourceDeleterThread::SetResourceQueueCapacity(0);
	}

	void Scene::ChagePosGameObjects(const Vector2& _offset)
	{
		for (auto layer : mLayers)
		{
			if (layer == nullptr)
				continue;
			layer->ChagePosGameObjects(_offset);
		}
	}

	void Scene::ChageScaleGameObjects(const Vector2& _scale)
	{
		for (auto layer : mLayers)
		{
			if (layer == nullptr)
				continue;
			layer->ChageScaleGameObjects(_scale);
		}
	}

	std::shared_ptr<GameObject> Scene::FindObject(std::wstring _name)
	{
		for (auto layer : mLayers)
		{
			if (layer == nullptr)
				continue;
			std::shared_ptr<GameObject> obj=layer->FindObject(_name);
			if (obj != nullptr)
				return obj;
		}
		return nullptr;
	}

	void Scene::AddGameObject(std::shared_ptr<GameObject>& obj, ELayerType layer)
	{
		mLayers[(UINT)layer]->AddGameObject(obj);
		obj->SetLayerType(layer);
		obj.reset();
	}

	void Scene::RemoveGameObject(GameObject* _obj)
	{
		mLayers[(UINT)_obj->GetLayerType()]->RemoveGameObject(_obj);
	}

	void Scene::RemoveGameObject(GameObject* _obj, ELayerType _type)
	{
		mLayers[(UINT)_type]->RemoveGameObject(_obj);
	}

	void Scene::RemoveGameObject(ELayerType _type)
	{
		mLayers[(UINT)_type]->RemoveAll();
	}

	std::vector<std::shared_ptr<GameObject>>& Scene::GetGameObjects(ELayerType _layer_type)
	{
		return mLayers[(UINT)_layer_type]->GetGameObjectList();
	}

	bool Scene::UseingLayer(ELayerType _layer_type)
	{
		if (mLayers[(UINT)_layer_type] == nullptr) 
			return false; 
		return true;
	}

	Scene::Scene()
	{
		mLayers.resize((UINT)ELayerType::MAX);
	}

	Scene::~Scene()
	{
	}
}