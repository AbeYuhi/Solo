#pragma once
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"

/// <summary>
/// ReleaseCheck.h
/// ゲームエンジン上のメモリを解放できているか確認しているファイル
/// </summary>

/// <summary>
/// リソースリークが発生していないかを検知する処理
/// </summary>
struct D3DResoruceLeakCheaker {
	~D3DResoruceLeakCheaker() {
		ComPtr<IDXGIDebug1> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};