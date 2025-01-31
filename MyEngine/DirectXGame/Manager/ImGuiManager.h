#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "SrvManager.h"
#include "Data/RenderItem.h"
#include "Data/SpriteItem.h"
#include "External/imgui/imgui.h"
#include "External/imgui/imgui_impl_dx12.h"
#include "External/imgui/imgui_impl_win32.h"

/// <summary>
/// ImGuiManager.h
/// ImGuiを表示するためのマネージャークラス
/// </summary>

#ifdef _DEBUG
using namespace ImGui;
namespace MyEngine {
	class ImGuiManager
	{
	public:

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static ImGuiManager* GetInstance();

		/// <summary>
		/// 初期化関数
		/// </summary>
		void Initialize();

		/// <summary>
		/// ゲームループ中の初期化処理
		/// </summary>
		void Begin();

		/// <summary>
		/// ゲームループ中の処理終了
		/// </summary>
		void End();

		/// <summary>
		/// 描画関数
		/// </summary>
		void Draw();

		/// <summary>
		/// 解放処理
		/// </summary>
		void ReleseProcess();

		/// <summary>
		/// レンダーアイテムの情報をImGuiで編集
		/// </summary>
		/// <param name="name"></param>
		/// <param name="renderItem"></param>
		void RenderItemDebug(std::string name, RenderItem& renderItem);

		/// <summary>
		/// スプライトアイテムの情報をImGuiで編集
		/// </summary>
		/// <param name="name"></param>
		/// <param name="spriteItem"></param>
		void SpriteItemDebug(std::string name, SpriteItem& spriteItem);

	private:
		ImGuiManager() = default;
		~ImGuiManager() = default;
	};
}
#endif // _DEBUG