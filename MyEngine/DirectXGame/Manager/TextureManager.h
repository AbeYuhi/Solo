#pragma once
#include <vector>
#include <map>
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Data/TextureData.h"
#include "DirectXGame/Manager/SrvManager.h"
#include "External/DirectXTex/DirectXTex.h"
#include "External/DirectXTex/d3dx12.h"

/// <summary>
/// TextureManager.h
/// テクスチャーの情報を格納されているマネージャークラス
/// </summary>

namespace MyEngine {

	class TextureManager
	{
	public:
		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static TextureManager* GetInstance();
		static const UINT kMaxTextureNum_ = 1000;
		static UINT sTextureNum_;

		/// <summary>
		/// テクスチャの読み込み処理
		/// </summary>
		/// <param name="textureName"></param>
		/// <returns></returns>
		static uint32_t Load(const std::string& textureName);

		/// <summary>
		/// テクスチャの読み込み処理
		/// </summary>
		/// <param name="textureName"></param>
		/// <returns></returns>
		static uint32_t Load(const std::string& filePath, const std::string& textureName);

		/// <summary>
		/// テクスチャハンドルのGPUのインデックスを取得
		/// </summary>
		/// <param name="textureHandle"></param>
		/// <returns></returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandleGPU(uint32_t textureHandle) { return textureDatas_[textureHandles_[textureHandle]].textureSrvHandleGPU; }

		/// <summary>
		/// テクスチャデスクの取得
		/// </summary>
		/// <param name="textureHandle"></param>
		/// <returns></returns>
		D3D12_RESOURCE_DESC GetTextureDesc(uint32_t textureHandle) { return textureDatas_[textureHandles_[textureHandle]].textureResource->GetDesc(); }

	private: //メンバ関数

		/// <summary>
		/// テクスチャインデックスの管理処理
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="textureName"></param>
		/// <returns></returns>
		uint32_t LoadInternal(const std::string& filePath, const std::string& textureName);

		/// <summary>
		/// テクスチャをGPU近くに配置する処理
		/// </summary>
		/// <param name="filePath"></param>
		void TransferTexture(const std::string& filePath);

		/// <summary>
		/// テクスチャの読み込み処理
		/// </summary>
		/// <param name="filePath"></param>
		/// <returns></returns>
		DirectX::ScratchImage LoadTexture(const std::string& filePath);

		/// <summary>
		/// テクスチャリソースの作成処理
		/// </summary>
		/// <param name="metadata"></param>
		/// <returns></returns>
		ComPtr<ID3D12Resource> CreateTextureResource(DirectX::TexMetadata metadata);

		/// <summary>
		/// テクスチャデータ転送処理
		/// </summary>
		/// <param name="filePath"></param>
		/// <returns></returns>
		[[nodiscard]]
		ComPtr<ID3D12Resource> UploadTextureData(const std::string& filePath);

		/// <summary>
		/// SRVの作成処理
		/// </summary>
		/// <param name="filePath"></param>
		void CreateShaderResourceView(const std::string& filePath);

	private:
		TextureManager() = default;
		~TextureManager();

		std::map<std::string, TextureData> textureDatas_;
		std::map<uint32_t, std::string> textureHandles_;
	};

}