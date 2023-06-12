#pragma once
#include "CommonInclude.h"
#include "Enums.h"
#include "Collider.h"
#include "Script.h"
namespace yeram_client
{
	union ColliderID
	{
		struct
		{
			UINT32 left;
			UINT32 right;
		};
		UINT64 id;
	};
	class Scene;
	class ColliderManager
	{
	public:
		static void Update();
		static void LayerCollision(Scene* scene, ELayerType left, ELayerType right);
		
		static void ColliderCollision(Collider* leftCol, Collider* rightCol, ELayerType left, ELayerType right);
		static bool Intersect(Collider* left, Collider* right);

		static void SetLayer(ELayerType left, ELayerType right, bool value);
		static void Clear();
	private:
		static WORD mMatrix[(UINT)ELayerType::MAX];
		static std::map<UINT64, bool> mCollisionMap;
	};
}
