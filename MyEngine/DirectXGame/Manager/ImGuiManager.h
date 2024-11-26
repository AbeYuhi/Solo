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
		static ImGuiManager* GetInstance();

		void Initialize();

		void Begin();

		void End();

		void Draw();

		void ReleseProcess();

		void RenderItemDebug(std::string name, RenderItem& renderItem);

		void SpriteItemDebug(std::string name, SpriteItem& spriteItem);

	private:
		ImGuiManager() = default;
		~ImGuiManager() = default;
	};
}
#endif // _DEBUG