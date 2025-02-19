#pragma once
#include <d3d12.h>

/// <summary>
/// Resourcehandle.h
/// GPUとCPUのハンドルを収納する構造体のファイル
/// </summary>

struct  ResourceHandles {
	D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle;
};