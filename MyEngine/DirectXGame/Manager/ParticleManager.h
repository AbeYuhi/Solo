#pragma once
#include <map>
#include <format>
#include <numbers>
#include "SrvManager.h"
#include "DirectXGame/Data/RenderItem.h"
#include "DirectXGame/Data/ResourceHandles.h"
#include "DirectXGame/Data/ParticleDrawInfo.h"
#include "DirectXGame/Data/ParticleForGPU.h"
#include "DirectXGame/Data/AccelerationField.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"
#include "DirectXGame/Manager/RandomManager.h"
#include "DirectXGame/Object/WireFrameBox.h"
#include "DirectXGame/Object/WireFrameSpriteBox.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"

/// <summary>
/// ParticleManager.h
/// パーティクルを生成するための基底クラス
/// </summary>

using namespace std;

struct ParticleInfo {
	EulerTransformData srtData;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currenttime;
};

struct Emitter {
	EulerTransformData transform;
	int count; //発生数
	float frequency;
	float frequencyTime;
};

namespace MyEngine {

	class Particle {
	public:
		std::function<void()> draw; // 描画関数を保持
		bool isAlive_;
		bool isDelete_;
		bool isDraw_;
		Particle(std::function<void()> drawFunction) : draw(drawFunction), isAlive_(false), isDelete_(false), isDraw_(false) {}
		void Draw() {
			if (draw) draw(); // 関数が設定されていれば呼び出し
		}
	};

	class ParticleManager
	{
	public: //メンバ関数
		ParticleManager(int maxParticleCount);
		~ParticleManager();

		/// <summary>
		/// 初期化処理
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// 更新処理
		/// </summary>
		virtual void Update();

		/// <summary>
		/// エミッターの描画処理
		/// </summary>
		virtual void EmitterDraw();

		/// <summary>
		/// パーティクルの描画処理
		/// </summary>
		virtual void Draw();

		/// <summary>
		/// 粒子の発生関数
		/// </summary>
		/// <returns></returns>
		virtual ParticleInfo MakeNewParticle() = 0;

		/// <summary>
		/// パーティクルの発生処理
		/// </summary>
		void PopParticle();

		/// <summary>
		/// パーティクルを削除する処理
		/// </summary>
		void UnloadParticle();

	private: //メンバ関数

		/// <summary>
		/// SRVを作成する処理
		/// </summary>
		void CreateSRV();

		/// <summary>
		/// 粒子を複数発生させる処理
		/// </summary>
		/// <returns></returns>
		std::list<ParticleInfo> Emission();

	public: //ゲッターセッター

		/// <summary>
		/// パーティクルの発生を有効にするか無効にするか
		/// </summary>
		/// <returns></returns>
		inline bool GetIsPopParticle() { return isPopParticle_; }
		inline void SetIsPopParticle(bool isPopParticle) { isPopParticle_ = isPopParticle; }

		/// <summary>
		/// パーティクルのテクスチャ設定処理
		/// </summary>
		/// <param name="textureHandle"></param>
		inline void SetTextureHandle(uint32_t textureHandle) { textureHandle_ = textureHandle; }

		/// <summary>
		/// パーティクルの発生位置の調整処理
		/// </summary>
		/// <param name="pos"></param>
		inline void SetEmitterPos(Vector3 pos) { emitter_.transform.translate_ = pos; }

	protected: //メンバ変数
		//乱数マネージャー
		RandomManager* randomManager_;
		//1つのパーティクルの最大粒子数
		const int kMaxParticleCount_;
		//現在のパーティクルの粒子数
		int particleCount_;
		//テクスチャハンドル
		uint32_t textureHandle_;

		//リソース
		ComPtr<ID3D12Resource> resource_;
		int srvIndex_;
		//Resourceハンドル
		ResourceHandles srvHandle_;
		//エミッターの情報
		Emitter emitter_;
		std::shared_ptr<WireFrameBox> emitterObj_;
		std::shared_ptr<WireFrameSpriteBox> emitterSprite_;
		RenderItem emitterObjInfo_;
		SpriteItem emitterSpriteInfo_;
		//データ
		ParticleForGPU* particleData_;
		ParticleMaterialInfo materialInfo_;
		std::list<ParticleInfo> particles_;

		//スプライトのパーティクルかどうか
		bool isSpriteParticle_;
		//パーティクルを発生させるか
		bool isPopParticle_;
		//ビルボードするかどうか
		bool isBillboard_;

		//描画の際に必要なパーティクルデータ
		ParticleDrawInfo drawInfo_;
		//時間
		const float kDeltaTime_ = 1.0f / 60.0f;
	};

}