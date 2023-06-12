#include "Layer.h"
namespace yeram_client
{
	Layer::Layer()
	{
		mbActive = true;
	}
	Layer::~Layer()
	{

	}
	void Layer::Initialize()
	{
		for (std::shared_ptr<GameObject> obj : mObjs)
		{
			if (obj == nullptr)
				continue;
			obj->Initialize();
		}
	}
	void Layer::Update()
	{
		for (std::shared_ptr<GameObject> obj : mObjs)
		{
			if (obj == nullptr)
				continue;
			if (obj->GetActive() == false)
				continue;
			obj->Update();
		}
	}
	void Layer::Render(HDC hdc)
	{
		for (std::shared_ptr<GameObject> obj : mObjs)
		{
			if (obj == nullptr)
				continue;
			if (obj->GetActive() == false)
				continue;
			obj->Render(hdc);
		}
	}
	void Layer::Release()
	{
		for (std::shared_ptr<GameObject>& obj : mObjs)
		{
			if (obj == nullptr)
				continue;
			obj.reset();
		}
		mObjs.clear();
		mObjs.~vector();
	}
	std::shared_ptr<GameObject> Layer::FindObject(std::wstring _name)
	{
		for (auto obj : mObjs)
		{
			if (obj->GetName() == _name)
				return obj;
		}
		return nullptr;
	}
	std::vector<std::shared_ptr<GameObject>>& Layer::GetGameObjectList()
	{
		return mObjs;
	}
	void Layer::ChagePosGameObjects(const Vector2& _offset)
	{
		for (auto obj : mObjs)
		{
			if (obj == nullptr)
				continue;
			if (obj->GetCameraMoveActive() == false)
				continue;

			Transform* tf = obj->GetComponent<Transform>();
			Vector2 pos = tf->GetPos();
			tf->CaluatePos(_offset);

		}
	}
	void Layer::ChageScaleGameObjects(const Vector2& _scale)
	{
		for (auto obj : mObjs)
		{
			if (obj == nullptr)
				continue;
			size_t str_pos = obj->GetName().find(L"BackGround");
			if (str_pos == std::wstring::npos)
			{
				Transform* tf = obj->GetComponent<Transform>();
				Vector2 pos = tf->GetPos();
				//tf->SetScale(_scale);
				tf->CaluateSclae(_scale);
			}
		}
	}
	void Layer::AddGameObject(std::shared_ptr<GameObject>& obj)
	{
		int cnt = obj.use_count();
		mObjs.push_back(obj);
	}
	void Layer::RemoveGameObject(GameObject* _obj)
	{
		for (auto itr = mObjs.begin(); itr != mObjs.end(); itr++)
		{
			if ((*itr).get() == _obj)
			{
				(*itr).reset();
				mObjs.erase(itr);
				break;
			}
		}
	}
	void Layer::RemoveAll()
	{
		for (auto itr = mObjs.begin(); itr != mObjs.end(); itr++)
		{
			(*itr)->SetActive(false);
			(*itr).reset();
			mObjs.erase(itr);
			itr = mObjs.begin();
			if (itr == mObjs.end())
				break;
		}
	}
}