#pragma once
#include <memory>
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"
#include "DirectXGame/Data/VertexData.h"
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Data/Material.h"
#include "DirectXGame/Data/MaterialInfo.h"
#include "DirectXGame/Data/RenderItem.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"

/// <summary>
/// WireFrameBox.h
/// 立方体のフレームを表示するためのクラスファイル
/// </summary>

namespace MyEngine {

	class WireFrameBox
	{
	public: //静的メンバ関数
		static std::shared_ptr<WireFrameBox> Create();

	private: //静的メンバ変数
		const static UINT kVertexNumber = 8;
		const static UINT kIndexNumber = 24;
		static std::shared_ptr<WireFrameBox> sWireFrameBox_;

	public: //メンバ関数
		WireFrameBox();
		~WireFrameBox();

		/// <summary>
		/// 初期化関数
		/// </summary>
		void Initialize();

		/// <summary>
		/// 描画関数
		/// </summary>
		/// <param name="renderItem"></param>
		void Draw(RenderItem& renderItem);

	public: //ゲッターセッター

		/// <summary>
		/// 頂点位置の設定関数
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="index"></param>
		void SetVertexPos(Vector4 pos, int index) { vertexData_[index].position = pos; }

	private: //メンバ変数
		//オブジェクト情報のResource
		ComPtr<ID3D12Resource> vertexResource_ = nullptr;
		ComPtr<ID3D12Resource> indexResource_ = nullptr;

		//VertexBufferView
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
		D3D12_INDEX_BUFFER_VIEW indexBufferView_;

		//オブジェクトのローカル情報
		VertexData* vertexData_;
		uint32_t* indexData_;
	};

}