#pragma once
#include <directXMath.h>
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Math/Math.h"

/// <summary>
/// Shadow.h
/// 射影をするために作成したクラス
/// </summary>

class Shadow
{
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="lightVec"></param>
	void Update(Vector3 lightVec);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	ComPtr<ID3D12Resource> shadowResorce_ = nullptr;
	Matrix4x4* shadowData_;
};

