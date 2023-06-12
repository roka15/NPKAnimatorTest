#include "ColliderManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
namespace yeram_client
{
	WORD ColliderManager::mMatrix[(UINT)ELayerType::MAX] = {};
	std::map<UINT64, bool> ColliderManager::mCollisionMap;
	void ColliderManager::Update()
	{
		Scene* scene = SceneManager::GetActiveScene();

		for (UINT row = 0; row < (UINT)ELayerType::MAX; row++)
		{
			for (UINT col = 0; col < (UINT)ELayerType::MAX; col++)
			{
				if (mMatrix[row] & (1 << col))
				{
					LayerCollision(scene, (ELayerType)row, (ELayerType)col);
				}
			}
		}
	}

	void ColliderManager::LayerCollision(Scene* scene, ELayerType left, ELayerType right)
	{
		if (scene->UseingLayer(left) == false || scene->UseingLayer(right) == false)
			return;
		
		std::vector<std::shared_ptr<GameObject>>& lefts = scene->GetGameObjects(left);
		std::vector<std::shared_ptr<GameObject>>& rights = scene->GetGameObjects(right);

		for (auto leftObject : lefts)
		{
			Collider* leftCollider = leftObject->GetComponent<Collider>();
			
			if (leftCollider == nullptr)
				continue;
			if (leftCollider->GetOwner()->GetActive() == false)
				continue;

			for (auto rightObject : rights)
			{
				Collider* rightCollider = rightObject->GetComponent<Collider>();
				
				if (rightCollider == nullptr)
					continue;
				if (rightCollider->GetOwner()->GetActive() == false)
					continue;
				if (leftObject == rightObject)
					continue;

				ColliderCollision(leftCollider, rightCollider, left, right);
			}
		}
		
	}

	void ColliderManager::ColliderCollision(Collider* leftCol, Collider* rightCol, ELayerType left, ELayerType right)
	{
		ColliderID colliderID = {};
		colliderID.left = (UINT)leftCol->GetID();
		colliderID.right = (UINT)rightCol->GetID();

		//static std::map<UINT64, bool> mCollisionMap;
		std::map<UINT64, bool>::iterator iter
			= mCollisionMap.find(colliderID.id);

		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(colliderID.id, false));
			iter = mCollisionMap.find(colliderID.id);
		}

		if (Intersect(leftCol, rightCol))
		{
			// 최초 충돌 시작했을때 enter
			if (iter->second == false)
			{
				leftCol->OnCollisionEnter(rightCol);
				rightCol->OnCollisionEnter(leftCol);

				iter->second = true;
			}
			else // 충돌 중인상태 stay
			{
				leftCol->OnCollisionStay(rightCol);
				rightCol->OnCollisionStay(leftCol);
			}
		}
		else
		{
			// Exit
			// 이전프레임 충돌 O
			// 현재는 충돌 X 
			if (iter->second == true)
			{
				leftCol->OnCollisionExit(rightCol);
				rightCol->OnCollisionExit(leftCol);

				iter->second = false;
			}
		}
	}

	bool ColliderManager::Intersect(Collider* left, Collider* right)
	{
		Vector2 leftPos = left->GetPos();
		Vector2 rightPos = right->GetPos();

		// 두 충돌체 간의 거리와, 각면적의 절반끼리의 합을 비교해서
		// 거리가 더 길다면 충돌 X, 거리가 더 짧다면 충돌 O
		Vector2 leftSize = left->GetSize();
		Vector2 rightSize = right->GetSize();

		leftPos.x = leftPos.x + leftSize.x / 2.0f;
		leftPos.y = leftPos.y + leftSize.y / 2.0f;

		rightPos.x = rightPos.x + rightSize.x / 2.0f;
		rightPos.y = rightPos.y + rightSize.y / 2.0f;

		if (fabs(leftPos.x - rightPos.x) < (leftSize.x / 2.0f) + (rightSize.x / 2.0f)
			&& fabs(leftPos.y - rightPos.y) < (leftSize.y / 2.0f) + (rightSize.y / 2.0f))
		{
			return true;
		}

		return false;
	}

	void ColliderManager::SetLayer(ELayerType left, ELayerType right, bool value)
	{
		UINT row = 0;
		UINT col = 0;

		UINT ileft = (UINT)left;
		UINT iright = (UINT)right;

		if (left <= right)
		{
			row = ileft;
			col = iright;
		}
		else
		{
			row = iright;
			col = ileft;
		}

		if (value == true)
			mMatrix[row] |= (1 << col);
		else
			mMatrix[row] &= ~(1 << col);
	}

	
	void ColliderManager::Clear()
	{
		memset(mMatrix, 0, sizeof(WORD) * (UINT)ELayerType::MAX);
		mCollisionMap.clear();	
	}

}

