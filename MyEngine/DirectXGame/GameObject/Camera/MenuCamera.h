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

	void Initialize() override;

	void Update() override;

private:
};

