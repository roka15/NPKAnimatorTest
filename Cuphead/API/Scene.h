#pragma once
#include "Entity.h"
namespace yeram_client
{
	class Layer;
	class Component;
	class GameObject;
	class Scene :public Entity
	{
	public:
		Scene();
		virtual ~Scene();

	   virtual void Initialize();
	   virtual void Update();
	   virtual void Render(HDC hdc);
	   virtual void Release();

	   virtual void OnEnter();
	   virtual void OnExit();

	   void ChagePosGameObjects(const Vector2& _offset);
	   void ChageScaleGameObjects(const Vector2& _scale);
	   std::shared_ptr<GameObject> FindObject(std::wstring _name);
	   void AddGameObject(std::shared_ptr<GameObject>& obj, ELayerType layer);
	   void RemoveGameObject(GameObject* _obj);
	   void RemoveGameObject(GameObject* _obj,ELayerType _type);
	   void RemoveGameObject(ELayerType _type);
	   const ESceneType& GetSceneType() { return mCurType; }
	   std::vector<std::shared_ptr<GameObject>>& GetGameObjects(ELayerType _layer_type);
	   bool UseingLayer(ELayerType _layer_type);
	protected:
		std::vector<Layer*> mLayers;
		std::vector<EWindowType> mUseWindows;
		ESceneType mCurType;
	};

}

