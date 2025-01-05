#pragma once
#include "DirectXGame/Data/DirectionalLight.h"
#include "DirectXGame/Data/PointLight.h"
#include "DirectXGame/Data/SpotLight.h"
#include "DirectXGame/Manager/ImGuiManager.h"

/// <summary>
/// LightObjectManager.h
/// ライトをシェーダーに送るためにまとめているファイル
/// </summary>

static const int kDirectionLightNum = 1;
static const int kPointLightNum = 3;
static const int kSpotLightNum = 3;

struct LightData {
	DirectionalLightData directionalLight[kDirectionLightNum];
	PointLightData pointLight[kPointLightNum];
	SpotLightData spotLight[kSpotLightNum];
};

namespace MyEngine {

	class LightObjectManager
	{
	public:
		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static LightObjectManager* GetInstance();
		
		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize();

		/// <summary>
		/// 初期状態のデータに戻す処理
		/// </summary>
		void InitData();

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// 描画処理
		/// </summary>
		void Draw();

	public: //ゲッターセッター

		/// <summary>
		/// 平行光源の取得処理
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		DirectionalLightData GetDirectionalLightData(int index) { return lightData_->directionalLight[index]; }

		/// <summary>
		/// 点光源の取得処理
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		PointLightData GetPointLightData(int index) { return lightData_->pointLight[index]; }

		/// <summary>
		/// スポットライトの取得処理
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		SpotLightData GetSpotLightData(int index) { return lightData_->spotLight[index]; }

		/// <summary>
		/// 平行光源のセッター
		/// </summary>
		/// <param name="index"></param>
		/// <param name="lightData"></param>
		void SetDirectionalLightData(int index, DirectionalLightData lightData) { lightData_->directionalLight[index] = lightData; }

		/// <summary>
		/// 点光源のセッター
		/// </summary>
		/// <param name="index"></param>
		/// <param name="lightData"></param>
		void SetPointLightData(int index, PointLightData lightData) { lightData_->pointLight[index] = lightData; }

		/// <summary>
		/// スポットライトのセッター
		/// </summary>
		/// <param name="index"></param>
		/// <param name="lightData"></param>
		void SetSpotLightData(int index, SpotLightData lightData) { lightData_->spotLight[index] = lightData; }

	private:
		LightObjectManager() = default;
		~LightObjectManager() = default;

		ComPtr<ID3D12Resource> lightResource_;
		LightData* lightData_;
	};

}
