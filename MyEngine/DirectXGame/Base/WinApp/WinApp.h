#pragma once
#include <Windows.h>
#include <cstdint>
#include "Math/Vector2.h"

/// <summary>
/// WinApp.h
/// ウィンドウ表示に関するファイル
/// </summary>

namespace MyEngine {

	class WinApp
	{
	public: //静的関数
		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static WinApp* GetInstance();

		/// <summary>
		/// ウィンドウ情報の設定
		/// </summary>
		/// <param name="hwnd"></param>
		/// <param name="msg"></param>
		/// <param name="wparam"></param>
		/// <param name="lparam"></param>
		/// <returns></returns>
		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	public: //静的メンバ変数
		static const wchar_t className[];
		static const int32_t kWindowWidth = 1280;
		static const int32_t kWindowHeight = 720;

	public: //メンバ関数

		/// <summary>
		/// ウィンドウの生成処理
		/// </summary>
		/// <param name="title"></param>
		/// <param name="windowStyle"></param>
		/// <param name="windowWidth"></param>
		/// <param name="windowHeight"></param>
		void CreateGameWindow(const wchar_t* title = L"MyEngine", UINT windowStyle = WS_OVERLAPPEDWINDOW, int32_t windowWidth = kWindowWidth, int32_t windowHeight = kWindowHeight);

		/// <summary>
		/// ウィンドウの開放処理
		/// </summary>
		void DiscardingWindow();

		/// <summary>
		/// ウィンドウにされた操作の取得処理
		/// </summary>
		/// <returns></returns>
		bool ProcessMessage();

		/// <summary>
		/// カーソルに制限をつける
		/// </summary>
		void LockCursorToWindow();

		// カーソルの制限を解除
		void UnlockCursor();

		/// <summary>
		/// ウィンドウサイズの取得
		/// </summary>
		/// <param name="hwnd"></param>
		/// <returns></returns>
		Vector2 GetWindowSize() {
			RECT rect;
			GetClientRect(hwnd_, &rect);
			float width = (float)rect.right - (float)rect.left;
			float height = (float)rect.bottom - (float)rect.top;
			return { width, height };
		}

		/// <summary>
		/// HWNDのゲッター
		/// </summary>
		/// <returns></returns>
		inline HWND GetHWND() { return hwnd_; }

		/// <summary>
		/// WNDCクラスのゲッター
		/// </summary>
		/// <returns></returns>
		inline WNDCLASS GetWNDCLASS() { return wc_; }

	private: //メンバ関数
		WinApp() = default;
		~WinApp() = default;

	private: //メンバ変数
		HWND hwnd_;
		UINT windowStyle_;
		WNDCLASS wc_;
	};

}