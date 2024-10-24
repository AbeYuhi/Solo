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

	void Initialize() override;

	void Update() override;

private:

};

