#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include <fstream>
#include <map>
#pragma region XAudio

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")

#pragma endregion

/// <summary>
/// AudioManager.h
/// 音を管理しているマネージャークラス
/// </summary>

struct ChunkHeader {
	char id[4];
	int32_t size;
};

struct RiffHeader {
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk {
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

namespace MyEngine {

	/// <summary>
	/// 音データ構造体
	/// </summary>
	struct SoundData
	{
		IXAudio2SourceVoice* pSourceVoice = {};
		WAVEFORMATEX wfex = {};
		BYTE* pBuffer = {};
		unsigned int bufferSize;
		std::vector<BYTE>mediaData;
		IMFSourceReader* MFSourceReader;
		IMFMediaType* mediaType;
		XAUDIO2_BUFFER buf = {};
	};

	class AudioManager
	{
	public:

		/// <summary>
		/// インスタンスの取得関数
		/// </summary>
		/// <returns></returns>
		static AudioManager* GetInstance();

		/// <summary>
		/// 初期化関数
		/// </summary>
		void Initialize();

		/// <summary>
		/// 解放処理
		/// </summary>
		void Finalize();

		/// <summary>
		/// サウンドの読み込み関数
		/// </summary>
		/// <param name="filename"></param>
		/// <returns></returns>
		uint32_t SoundLoadWave(const std::string filename);

		/// <summary>
		/// 指定したサウンドを再生する関数
		/// </summary>
		/// <param name="index"></param>
		/// <param name="soundVolume"></param>
		/// <param name="isLoop"></param>
		void SoundPlayWave(const uint32_t index, const float soundVolume = 1.0f, bool isLoop = false);

		/// <summary>
		/// mp3の読み込み
		/// </summary>
		/// <param name="fileName"></param>
		/// <returns></returns>
		uint32_t SoundLoadMp3(const string& fileName);

		/// <summary>
		/// mp3再生
		/// </summary>
		/// <param name="FileName"></param>
		/// <param name="Volume"></param>
		void SoundPlayMp3(const uint32_t index, const float& Volume = 1.0f, bool isLoop = false);

		/// <summary>
		/// ループしているサウンドを停止する処理
		/// </summary>
		/// <param name="index"></param>
		void StopLoopWave(const uint32_t index);

		/// <summary>
		/// サウンドが再生中かを取得する処理
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		bool IsSoundPlaying(const uint32_t index);

		/// <summary>
		/// サウンド情報を消す処理
		/// </summary>
		/// <param name="index"></param>
		void SoundUnload(const uint32_t index);

		/// <summary>
		/// サウンド音量を調整する処理
		/// </summary>
		/// <param name="index"></param>
		/// <param name="soundVolume"></param>
		inline void SetVolume(const uint32_t index, const float soundVolume) {
			if (soundDatas_[index].pSourceVoice) {
				soundDatas_[index].pSourceVoice->SetVolume(soundVolume);
			}
		}

	private: //メンバ関数

	private:
#pragma region Singleton
		AudioManager() = default;
		~AudioManager() = default;
		AudioManager(const AudioManager&) = delete;
		const AudioManager& operator=(const AudioManager&) = delete;
#pragma endregion

		ComPtr<IXAudio2> xAudio2_;
		IXAudio2MasteringVoice* masterVoice_;
		std::map<uint32_t, SoundData> soundDatas_;
		int textureNum;
	};

}