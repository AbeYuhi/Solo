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

		/// <summary>
		/// インスタンスの取得関数
		/// </summary>
		/// <returns></returns>
		static CollisionManager* GetInstance();
		
		/// <summary>
		/// 初期化関数
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新関数
		/// </summary>
		void Update();

		/// <summary>
		/// 描画関数
		/// </summary>
		void Draw();

		/// <summary>
		/// IsDeleteが有効なコライダーを除去する処理
		/// </summary>
		void SyncColliderList();

		/// <summary>
		/// すべてのコライダーデータを除去する処理
		/// </summary>
		void Reset();

	public:

		/// <summary>
		/// すべてのコライダーデータを除去する処理
		/// </summary>
		inline void ClearColliders() { colliders_.clear(); }

		/// <summary>
		/// コライダーマネージャーにコライダーを追加する処理
		/// </summary>
		/// <param name="collider"></param>
		inline void AddCollider(Collider* collider) { colliders_.push_back(collider); }

	private:
		CollisionManager() = default;
		~CollisionManager() = default;

		std::list<Collider*> colliders_;
		std::shared_ptr<WireFrameBox> wireFrameBox_;
		std::shared_ptr<WireFrameSphere> wireFrameSphere_;
	};

}