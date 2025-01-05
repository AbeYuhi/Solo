#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/DebugSytem/DebugLog.h"
#include "Math/Vector4.h"
#include "External/DirectXTex/d3dx12.h"
#include <WRL/client.h>
#include <format>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <cassert>
#include <compare>
#include <chrono>
#include <thread>

/// <summary>
/// DirectXCommon.h
/// DirectX12に関する機能の処理をまとめているファイル
/// </summary>

using namespace Microsoft::WRL;
using namespace std;

namespace MyEngine {
	class SrvManager;
	class DirectXCommon {
	public: //静的関数
		static DirectXCommon* GetInstance();

	public: //メンバ関数
		// コピーコンストラクタとコピー代入演算子を禁止
		DirectXCommon(const DirectXCommon&) = delete;
		DirectXCommon& operator=(const DirectXCommon&) = delete;

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize();

		/// <summary>
		/// 描画後の処理
		/// </summary>
		void PostDraw();

		/// <summary>
		/// 画面のクリア
		/// </summary>
		void ClearRenderTarget(Vector4 clearScreenColor, CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle);

		/// <summary>
		/// 深度バッファのクリア
		/// </summary>
		void ClearDepthStencilBuffer();

		/// <summary>
		/// コマンドのキックから次のコマンドリストの準備
		/// </summary>
		void TransferCommandList();

		/// <summary>
		/// コマンドリストのキック、スワップチェインの実行
		/// </summary>
		void SwapChainProcessing();

		/// <summary>
		/// FPSの固定化
		/// </summary>
		void UpdateFixFPS();

		/// <summary>
		/// Dxcのコンパイルシェーダー
		/// </summary>
		ComPtr<IDxcBlob> CompilerShader(const wstring& filePath, const wchar_t* profile);

		/// <summary>
		/// ディスクリプターヒープの生成関数
		/// </summary>
		/// <param name="heapType"></param>
		/// <param name="numDescriptors"></param>
		/// <param name="shaderVisible"></param>
		/// <returns></returns>
		ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

		/// <summary>
		/// 深度情報を収納するためのリソース確保関数
		/// </summary>
		/// <returns></returns>
		ComPtr<ID3D12Resource> CreateDepthStencilTextureResource();

	public: //ゲッターセッター

		/// <summary>
		/// デバイスのゲッター
		/// </summary>
		/// <returns></returns>
		inline ID3D12Device* GetDevice() { return device_.Get(); }

		/// <summary>
		/// コマンドリストのゲッター
		/// </summary>
		/// <returns></returns>
		inline ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

		/// <summary>
		/// スワップチェインのゲッター
		/// </summary>
		/// <returns></returns>
		inline IDXGISwapChain4* GetSwapShain() { return swapChain_.Get(); }

		/// <summary>
		/// RTVデスクリプターヒープのゲッター
		/// </summary>
		/// <returns></returns>
		inline ID3D12DescriptorHeap* GetRtvDescriptorHeap() { return rtvDescriptorHeap_.Get(); }

		/// <summary>
		/// DSVデスクリプターヒープのゲッター
		/// </summary>
		/// <returns></returns>
		inline ID3D12DescriptorHeap* GetDsvDescriptorHeap() { return dsvDescriptorHeap_.Get(); }

		/// <summary>
		/// DSVリソースのゲッター
		/// </summary>
		/// <returns></returns>
		inline ID3D12Resource* GetDsvResource() { return depthStencilResource_.Get(); }

		/// <summary>
		/// RTVデスクのゲッター
		/// </summary>
		/// <returns></returns>
		inline D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc() { return rtvDesc_; }

		/// <summary>
		/// DSVデスクのゲッター
		/// </summary>
		/// <returns></returns>
		inline D3D12_DEPTH_STENCIL_VIEW_DESC GetDsvDesc() { return dsvDesc_; }

		/// <summary>
		/// RTVハンドルのゲッター
		/// </summary>
		/// <returns></returns>
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtvHandle();

		/// <summary>
		/// DSVハンドルのゲッター
		/// </summary>
		/// <returns></returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetDsvHandle();

		/// <summary>
		/// バックバッファーのゲッター
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		inline ID3D12Resource* GetBackBuffer(int index) { return backBuffers[index].Get(); }

		/// <summary>
		/// RTVハンドルの取得
		/// 0～1 : スワップチェイン
		/// 2 : レンダーテクスチャー
		/// </summary>
		/// <param name="index">
		/// </param>
		/// <returns></returns>
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtvHandle(int index);

	private: //メンバ関数
		DirectXCommon() = default;
		~DirectXCommon() = default;

		/// <summary>
		/// DXGIの初期化
		/// </summary>
		void InitializeDxgiDevice();

		/// <summary>
		/// コマンドの初期化
		/// </summary>
		void InitializeCommand();

		/// <summary>
		/// スワップチェインの初期化
		/// </summary>
		void InitializeSwapChain();

		/// <summary>
		/// RenderTargetViewの作成関数
		/// </summary>
		void CreateRenderTargetView();

		/// <summary>
		/// ShaderResourceViewの作成関数
		/// </summary>
		void CreateShaderResourceView();

		/// <summary>
		/// 深度情報の作成関数
		/// </summary>
		void CreateDepthStencilView();

		/// <summary>
		/// フェンス機能の作製
		/// </summary>
		void CreateFence();

		/// <summary>
		/// DXCの初期化
		/// </summary>
		void InitializeDXC();

		/// <summary>
		/// FPS計算機能の初期化
		/// </summary>
		void InitializeFixFPS();

	private: //メンバ変数
		ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
		ComPtr<ID3D12Device> device_ = nullptr;
		ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
		ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
		ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
		ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
		ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;
		ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_;
		SrvManager* srvManager_;
		vector<ComPtr<ID3D12Resource>> backBuffers;
		ComPtr<ID3D12Fence> fence_ = nullptr;
		ComPtr<IDxcUtils> dxcUtils_ = nullptr;
		ComPtr<IDxcCompiler3> dxcCompiler_ = nullptr;
		ComPtr<IDxcIncludeHandler> includeHandler_ = nullptr;
		ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;
		uint64_t fenceValue_ = 0;
		chrono::steady_clock::time_point reference_;
	};

}