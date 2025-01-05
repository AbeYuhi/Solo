#pragma once
#include "Base3DCamera.h"

/// <summary>
/// TitleCamera.h
/// タイトルシーンのカメラを処理しているファイル
/// </summary>

class TitleCamera : public Base3DCamera
{
public:
	TitleCamera();
	~TitleCamera();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:

};

