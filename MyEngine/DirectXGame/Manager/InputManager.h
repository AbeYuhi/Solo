#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <WRL/client.h>
#include <xinput.h>
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Math/Vector2.h"

/// <summary>
/// InputManager.h
/// 入力情報を検出するためのマネージャークラス
/// </summary>

using namespace Microsoft::WRL;

namespace MyEngine {

	class InputManager {
	public: //静的メンバ関数

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static InputManager* GetInstance();

	public: //メンバ関数

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// キーの入力状態の取得関数
		/// </summary>
		/// <param name="keyNum">DIK_</param>
		/// <returns></returns>
		inline bool IsPushKey(BYTE keyNum) {
			if (keys_[keyNum]) {
				return true;
			}
			return false;
		}
		inline bool IsTriggerKey(BYTE keyNum) {
			if (keys_[keyNum] && !preKeys_[keyNum]) {
				return true;
			}
			return false;
		}
		inline bool IsReleaseKey(BYTE keyNum) {
			if (!keys_[keyNum] && preKeys_[keyNum]) {
				return true;
			}
			return false;
		}

		/// <summary>
		/// マウスが入力されているかを検知
		/// 左ボタン[0]
		/// 右ボタン[1]
		/// ホイール[2]
		/// サイドボタン[3]
		/// </summary>
		/// <param name="clickNum"></param>
		/// <returns></returns>
		inline bool IsMousePush(BYTE clickNum) {
			if (mouseState_.rgbButtons[clickNum]) {
				return true;
			}
			return false;
		}

		/// <summary>
		/// マウストリガーの検知
		/// </summary>
		/// <param name="clickNum"></param>
		/// <returns></returns>
		inline bool IsMouseTrigger(BYTE clickNum) {
			if (mouseState_.rgbButtons[clickNum] && !preMouseState_.rgbButtons[clickNum]) {
				return true;
			}
			return false;
		}

		/// <summary>
		/// マウスの入力が外されたか検知
		/// </summary>
		/// <param name="clickNum"></param>
		/// <returns></returns>
		inline bool IsMouseRelsease(BYTE clickNum) {
			if (!mouseState_.rgbButtons[clickNum] && preMouseState_.rgbButtons[clickNum]) {
				return true;
			}
			return false;
		}

		/// <summary>
		/// マウスホイールの量を検知
		/// </summary>
		/// <returns></returns>
		inline float GetMouseWheelDelta() {
			return static_cast<float>(mouseState_.lZ);
		}

		/// <summary>
		/// マウスのポジションを検出
		/// </summary>
		/// <returns></returns>
		inline Vector2 GetMousePos() {
			return clientMousePos_;
		}

		/// <summary>
		/// マウスの移動量を検出
		/// </summary>
		/// <returns></returns>
		inline Vector2 GetMouseMovement() {
			return { (float)mouseState_.lX, (float)mouseState_.lY };
		}

		//コントローラー
		/// <summary>
		/// コントローラーのボタンが押されているか検出
		/// </summary>
		/// <param name="buttons"></param>
		/// <returns></returns>
		inline WORD IsPushGamePadbutton(WORD buttons) {
			if (gamePadState_.Gamepad.wButtons & buttons) {
				return true;
			}
			return false;
		}

		/// <summary>
		/// コントローラーのボタンがトリガーされたか検出
		/// </summary>
		/// <param name="buttons"></param>
		/// <returns></returns>
		inline WORD IsTriggerGamePadbutton(WORD buttons) {
			if (gamePadState_.Gamepad.wButtons & buttons && !(preGamePadState_.Gamepad.wButtons & buttons)) {
				return true;
			}
			return false;
		}

		/// <summary>
		/// コントローラーのボタンの入力が無効になったかを検出
		/// </summary>
		/// <param name="buttons"></param>
		/// <returns></returns>
		inline WORD IsReleaseGamePadbutton(WORD buttons) {
			if (!(gamePadState_.Gamepad.wButtons & buttons) && preGamePadState_.Gamepad.wButtons & buttons) {
				return true;
			}
			return false;
		}
		
		/// <summary>
		/// コントローラーの左トリガーが入力されているか検出
		/// </summary>
		/// <returns></returns>
		inline WORD IsPushGamePadLTrigger() {
			if (gamePadState_.Gamepad.bLeftTrigger) {
				return true;
			}
			return false;
		}

		/// <summary>
		/// コントローラーの左トリガーがトリガーされたかを検出
		/// </summary>
		/// <returns></returns>
		inline WORD IsTriggerGamePadLTrigger() {
			if (gamePadState_.Gamepad.bLeftTrigger && !preGamePadState_.Gamepad.bLeftTrigger) {
				return true;
			}
			return false;
		}

		/// <summary>
		/// コントローラー左トリガーが無効になったかを検出
		/// </summary>
		/// <returns></returns>
		inline WORD IsReleaseGamePadLTrigger() {
			if (!gamePadState_.Gamepad.bLeftTrigger && preGamePadState_.Gamepad.bLeftTrigger) {
				return true;
			}
			return false;
		}

		/// <summary>
		/// 右トリガーに入力がある間反応
		/// </summary>
		/// <returns></returns>
		inline WORD IsPushGamePadRTrigger() {
			if (gamePadState_.Gamepad.bRightTrigger) {
				return true;
			}
			return false;
		}
		/// <summary>
		/// 右トリガーにトリガーしたときに反応
		/// </summary>
		/// <returns></returns>
		inline WORD IsTriggerGamePadRTrigger() {
			if (gamePadState_.Gamepad.bRightTrigger && !preGamePadState_.Gamepad.bRightTrigger) {
				return true;
			}
			return false;
		}
		/// <summary>
		/// 右トリガーに入力がなくなったときに反応
		/// </summary>
		/// <returns></returns>
		inline WORD IsReleaseGamePadRTrigger() {
			if (!gamePadState_.Gamepad.bRightTrigger && preGamePadState_.Gamepad.bRightTrigger) {
				return true;
			}
			return false;
		}

		/// <summary>
		/// 左スティックの入力値(-1～1)の間で出力
		/// </summary>
		/// <returns></returns>
		inline Vector2 GetGamePadLStick() {
			Vector2 sThumb = { (float)gamePadState_.Gamepad.sThumbLX, (float)gamePadState_.Gamepad.sThumbLY };
			sThumb.x /= SHRT_MAX;
			sThumb.y /= SHRT_MAX;
			if (fabsf(sThumb.x) < 0.2f) {
				sThumb.x = 0.0f;
			}
			if (fabsf(sThumb.y) < 0.2f) {
				sThumb.y = 0.0f;
			}
			return sThumb;
		}
		/// <summary>
		/// 右スティックの入力値(-1～1)の間で出力
		/// </summary>
		/// <returns></returns>
		inline Vector2 GetGamePadRStick() {
			Vector2 sThumb = { (float)gamePadState_.Gamepad.sThumbRX, (float)gamePadState_.Gamepad.sThumbRY };
			sThumb.x /= SHRT_MAX;
			sThumb.y /= SHRT_MAX;
			if (fabsf(sThumb.x) < 0.2f) {
				sThumb.x = 0.0f;
			}
			if (fabsf(sThumb.y) < 0.2f) {
				sThumb.y = 0.0f;
			}
			return sThumb;
		}
		/// <summary>
		/// コントローラーの振動をセットする関数
		/// </summary>
		/// <param name="leftVib">Min:0, Max:65535</param>
		/// <param name="rightVib">Min:0, Max:65535</param>
		inline void SetVibration(int leftVib, int rightVib) {
			ZeroMemory(&gamePadVibration_, sizeof(XINPUT_VIBRATION));
			gamePadVibration_.wLeftMotorSpeed = static_cast<WORD>(leftVib);
			gamePadVibration_.wRightMotorSpeed = static_cast<WORD>(rightVib);
			XInputSetState(0, &gamePadVibration_);
		}
		/// <summary>
		/// コントローラーが接続できているかの関数
		/// </summary>
		/// <returns></returns>
		inline bool IsControllerConnected() {
			return isConnection_;
		}

	private: //メンバ関数
		InputManager() = default;
		~InputManager() { SetVibration(0, 0); }
	private: //メンバ変数
		ComPtr<IDirectInput8> directInput_;
		ComPtr<IDirectInputDevice8> keyboard_;
		ComPtr<IDirectInputDevice8> mouse_;

		//キーボード情報の格納変数
		BYTE keys_[256];
		BYTE preKeys_[256];
		//マウス情報の格納変数
		DIMOUSESTATE mouseState_;
		DIMOUSESTATE preMouseState_;
		Vector2 clientMousePos_;
		Vector2 preClientMousePos_;
		//ゲームパッド情報の格納変数
		XINPUT_STATE gamePadState_;
		XINPUT_STATE preGamePadState_;
		XINPUT_VIBRATION gamePadVibration_;
		bool isConnection_;
	};

}