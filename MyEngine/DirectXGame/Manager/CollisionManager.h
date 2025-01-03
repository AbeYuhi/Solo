#pragma once
#include <memory>
#include <list>
#include "DirectXGame/Data/Collider.h"
#include "DirectXGame/Object/WireFrameBox.h"
#include "DirectXGame/Object/WireFrameSphere.h"
#include "DirectXGame/Math/AABB.h"
#include "DirectXGame/Math/Math.h"
#include "Manager/DrawManager.h"

/// <summary>
/// CollisionManager.h
/// 衝突判定を管理しているマネージャークラス
/// </summary>

namespace MyEngine {

	class CollisionManager
	{
	public:

		static CollisionManager* GetInstance();

		void Initialize();

		void Update();

		void Draw();

		void SyncColliderList();

		void Reset();

	public:

		inline void ClearColliders() { colliders_.clear(); }
		inline void AddCollider(Collider* collider) { colliders_.push_back(collider); }

	private:
		CollisionManager() = default;
		~CollisionManager() = default;

		std::list<Collider*> colliders_;
		std::shared_ptr<WireFrameBox> wireFrameBox_;
		std::shared_ptr<WireFrameSphere> wireFrameSphere_;
	};

}