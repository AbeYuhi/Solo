#pragma once
#include "Base3DCamera.h"

/// <summary>
/// MenuCamera.h
/// メニューシーンのカメラを処理しているファイル
/// </summary>

class MenuCamera : public Base3DCamera
{
public:
	MenuCamera();
	~MenuCamera();

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

