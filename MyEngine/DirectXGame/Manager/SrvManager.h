#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "External/DirectXTex/d3dx12.h"
#include <WRL/client.h>
#include <format>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <cassert>
#include <map>

/// <summary>
/// SrvManager.h
/// SRVをまとめているマネージャークラス
/// </summary>

using namespace Microsoft::WRL;


namespace MyEngine {

	class DirectXCommon;
	class SrvManager
	{
	public:
		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static SrvManager* GetInstance();

	public:

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize();

		/// <summary>
		/// 空いてるSRVインデックスを探す処理
		/// </summary>
		/// <returns></returns>
		uint32_t Allocate();

		/// <summary>
		/// SRVのでスクリプターヒープの取得関数
		/// </summary>
		/// <returns></returns>
		inline ID3D12DescriptorHeap* GetSrvDescriptorHeap() { return descriptorHeap_.Get(); }

		/// <summary>
		/// リソースをデリートする処理
		/// </summary>
		/// <param name="index"></param>
		void UnLoadResource(int index);

		/// <summary>
		/// GPUのSRVインデックスを取得する処理
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(int index);

		/// <summary>
		/// CPUのSRVインデックスを取得する処理
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(int index);

	private:
		SrvManager() = default;
		~SrvManager() = default;

	private:
		DirectXCommon* dxCommon_;
		static const uint32_t kMaxCount_;
		uint32_t descriptorSize_;
		ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
		std::map<uint32_t, bool> isUseIndex_;
	};

}