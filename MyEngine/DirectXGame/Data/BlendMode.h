#pragma once

/// <summary>
/// BlendMode.h
/// ブレンドモード構造体があるファイル
/// </summary>

enum BlendMode {
	//ブレンドなし
	kBlendModeNone,
	//通常ブレンド
	kBlendModeNormal,
	//加算ブレンド
	kBlendModeAdd,
	//減算ブレンド
	kBlendModeSubtract,
	//乗算ブレンド
	kBlendModeMultily,
	//スクリーンブレンド
	kBlendModeScreen,
	//ブレンドモードの数(絶対に使うな)
	kCountOfBlendMode,
};