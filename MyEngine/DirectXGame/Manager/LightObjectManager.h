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

		static LightObjectManager* GetInstance();

		void Initialize();

		void InitData();

		void Update();

		void Draw();

	public: //ゲッターセッター

		DirectionalLightData GetDirectionalLightData(int index) { return lightData_->directionalLight[index]; }
		PointLightData GetPointLightData(int index) { return lightData_->pointLight[index]; }
		SpotLightData GetSpotLightData(int index) { return lightData_->spotLight[index]; }
		void SetDirectionalLightData(int index, DirectionalLightData lightData) { lightData_->directionalLight[index] = lightData; }
		void SetPointLightData(int index, PointLightData lightData) { lightData_->pointLight[index] = lightData; }
		void SetSpotLightData(int index, SpotLightData lightData) { lightData_->spotLight[index] = lightData; }

	private:
		LightObjectManager() = default;
		~LightObjectManager() = default;

		ComPtr<ID3D12Resource> lightResource_;
		LightData* lightData_;
	};

}
