#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "Math/Vector4.h"

/// <summary>
/// CreateResource.h
/// DirectX12の処理に必要なメモリを確保しているファイル
/// </summary>

/// <summary>
/// 頂点のメモリ確保
/// </summary>
ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);