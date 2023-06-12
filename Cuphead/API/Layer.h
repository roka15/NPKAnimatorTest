#pragma once
#include "CommonInclude.h"
#include "GameObject.h"
namespace yeram_client
{
	class Layer
	{
	public:
		Layer();
		virtual ~Layer();
		virtual void Initialize();
		virtual void Update();
		virtual void Render(HDC hdc);
		virtual void Release();
		std::shared_ptr<GameObject> FindObject(std::wstring _name);
		std::vector<std::shared_ptr<GameObject>>& GetGameObjectList();
		void ChagePosGameObjects(const Vector2& _offset);
		void ChageScaleGameObjects(const Vector2& _scale);
		void AddGameObject(std::shared_ptr<GameObject>& obj);
		void RemoveGameObject(GameObject* _obj);
		void RemoveAll();
		void SetActive(const bool _flag) { mbActive = _flag; }
		const bool& GetActive() { return mbActive; }
 	private:
		std::vector<std::shared_ptr<GameObject>> mObjs;
		bool mbActive;
	};
}


