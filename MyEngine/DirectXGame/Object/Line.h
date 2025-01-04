#pragma once
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
/// Line.h
/// ラインを表示するためのクラスファイル
/// </summary>

namespace MyEngine {

	class LineObj
	{
	public: //静的メンバ関数
		static std::shared_ptr<LineObj> Create();

	private: //静的メンバ変数
		const static UINT kVertexNumber = 2;

	public: //メンバ関数
		LineObj();
		~LineObj();

		/// <summary>
		/// ラインの初期化関数
		/// </summary>
		void Initialize();

		/// <summary>
		/// ラインの描画関数
		/// </summary>
		/// <param name="posA">始点</param>
		/// <param name="posB">終点</param>
		void Draw(const Vector3& posA, const Vector3& posB);

	public: //ゲッターセッター

		/// <summary>
		/// ラインの頂点位置の設定関数
		/// </summary>
		/// <param name="pos">場所</param>
		/// <param name="index">始点か終点かのインデックス</param>
		void SetVertexPos(const Vector4& pos, int index) { vertexData_[index].position = pos; }

	private: //メンバ変数
		//オブジェクト情報のResource
		ComPtr<ID3D12Resource> vertexResource_ = nullptr;

		//VertexBufferView
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

		//オブジェクトのローカル情報
		VertexData* vertexData_;

		RenderItem renderItem_;
	};

}