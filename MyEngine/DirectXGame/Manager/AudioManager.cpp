#include "AudioManager.h"
#pragma comment(lib, "xaudio2.lib")

/// <summary>
/// AudioManager.h
/// 音を管理しているクラスの実装ファイル
/// </summary>

namespace MyEngine {

	AudioManager* AudioManager::GetInstance() {
		static AudioManager instance;
		return &instance;
	}

	void AudioManager::Initialize() {

		CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
		MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

		HRESULT hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
		assert(SUCCEEDED(hr));
		hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
		assert(SUCCEEDED(hr));

		textureNum = 0;
	}

	void AudioManager::Finalize() {
		xAudio2_.Reset();

		for (int index = 0; index < textureNum; index++) {
			SoundUnload(index);
		}
	}

	uint32_t AudioManager::SoundLoadWave(const std::string filename) {
		textureNum++;
		uint32_t index = textureNum - 1;

		//ファイルを開く
		std::ifstream file;
		file.open("Resources/Sounds/" + filename, std::ios_base::binary);
		assert(file.is_open());

		//データ読み込み
		RiffHeader riff;
		file.read((char*)&riff, sizeof(riff));
		//ファイルがriffかチェック
		if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
			assert(0);
		}
		//タイプがWAVEかチェック
		if (strncmp(riff.type, "WAVE", 4) != 0) {
			assert(0);
		}
		FormatChunk format = {};
		file.read((char*)&format, sizeof(ChunkHeader));
		if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
			assert(0);
		}
		//本体の読み込み
		assert(format.chunk.size <= sizeof(format.fmt));
		file.read((char*)&format.fmt, format.chunk.size);

		//Dataチャンクの読み込み
		ChunkHeader data;
		file.read((char*)&data, sizeof(data));
		//JUNKチャンクを検出した場合
		if (strncmp(data.id, "JUNK", 4) == 0) {
			file.seekg(data.size, std::ios_base::cur);
			file.read((char*)&data, sizeof(data));
		}
		//データチャンクの読み込み
		if (strncmp(data.id, "data", 4) != 0) {
			assert(0);
		}
		//Dataチャンクのデータ部の読み込み
		char* pBuffer = new char[data.size];
		file.read(pBuffer, data.size);

		//Waveファイルを閉じる
		file.close();

		//returnするための音声データ
		SoundData soundData = {};
		soundData.wfex = format.fmt;
		soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
		soundData.bufferSize = data.size;

		soundDatas_[index] = soundData;
		return index;
	}

	void AudioManager::SoundPlayWave(const uint32_t index, const float soundVolume, bool isLoop) {
		HRESULT hr;

		hr = xAudio2_->CreateSourceVoice(&soundDatas_[index].pSourceVoice, &soundDatas_[index].wfex);
		assert(SUCCEEDED(hr));

		//再生する波形データの設定
		soundDatas_[index].buf = {};
		soundDatas_[index].buf.pAudioData = soundDatas_[index].pBuffer;
		soundDatas_[index].buf.AudioBytes = soundDatas_[index].bufferSize;
		soundDatas_[index].buf.Flags = XAUDIO2_END_OF_STREAM;

		if (isLoop) {
			soundDatas_[index].buf.LoopCount = XAUDIO2_LOOP_INFINITE;
		}

		//波形データの再生
		hr = soundDatas_[index].pSourceVoice->SubmitSourceBuffer(&soundDatas_[index].buf);
		hr = soundDatas_[index].pSourceVoice->Start();
		soundDatas_[index].pSourceVoice->SetVolume(soundVolume);
	}

	uint32_t AudioManager::SoundLoadMp3(const string& fileName) {
		textureNum++;
		uint32_t index = textureNum - 1;

		SoundData soundData;
		soundDatas_[index] = soundData;

		string filePath = "Resources/Sounds/" + fileName;

		//文字列変換
		int wideStrSize = MultiByteToWideChar(CP_UTF8, 0, filePath.c_str(), -1, NULL, 0);
		WCHAR* wideStr = new WCHAR[wideStrSize];
		HRESULT hr = MultiByteToWideChar(CP_UTF8, 0, filePath.c_str(), -1, wideStr, wideStrSize);
		assert(SUCCEEDED(hr));

		//ソースリーダーの作成
		MFCreateSourceReaderFromURL(wideStr, nullptr, &soundDatas_[index].MFSourceReader);

		//メディアタイプ
		MFCreateMediaType(&soundDatas_[index].mediaType);
		soundDatas_[index].mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		soundDatas_[index].mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);

		soundDatas_[index].MFSourceReader->SetCurrentMediaType(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), nullptr, soundDatas_[index].mediaType);

		soundDatas_[index].mediaType->Release();
		soundDatas_[index].mediaType = nullptr;
		soundDatas_[index].MFSourceReader->GetCurrentMediaType(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &soundDatas_[index].mediaType);

		//mp3の中身解析
		while (true)
		{
			IMFSample* sample = nullptr;
			DWORD dwStreamFlags = 0;

			hr = soundDatas_[index].MFSourceReader->ReadSample(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &dwStreamFlags, nullptr, &sample);
			assert(SUCCEEDED(hr));

			if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
			{
				break;
			}

			IMFMediaBuffer* mediaBuff = nullptr;
			DWORD cbCurrentLength = 0;
			hr = sample->ConvertToContiguousBuffer(&mediaBuff);
			assert(SUCCEEDED(hr));

			BYTE* pbuffer = nullptr;
			hr = mediaBuff->Lock(&pbuffer, nullptr, &cbCurrentLength);
			assert(SUCCEEDED(hr));

			soundDatas_[index].mediaData.resize(soundDatas_[index].mediaData.size() + cbCurrentLength);
			memcpy(soundDatas_[index].mediaData.data() + soundDatas_[index].mediaData.size() - cbCurrentLength, pbuffer, cbCurrentLength);

			hr = mediaBuff->Unlock();
			assert(SUCCEEDED(hr));

			mediaBuff->Release();
			sample->Release();
		}

		soundDatas_[index].buf.pAudioData = soundDatas_[index].mediaData.data();
		soundDatas_[index].buf.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(soundDatas_[index].mediaData.size());

		return index;
	}

	void AudioManager::SoundPlayMp3(const uint32_t index, const float& Volume, bool isLoop) {
		WAVEFORMATEX* waveFormat{};
		MFCreateWaveFormatExFromMFMediaType(soundDatas_[index].mediaType, &waveFormat, nullptr);

		xAudio2_->CreateSourceVoice(&soundDatas_[index].pSourceVoice, waveFormat);

		if (isLoop) {
			soundDatas_[index].buf.LoopCount = XAUDIO2_LOOP_INFINITE;
		}

		soundDatas_[index].pSourceVoice->SubmitSourceBuffer(&soundDatas_[index].buf);
		soundDatas_[index].pSourceVoice->SetVolume(Volume);
		soundDatas_[index].pSourceVoice->Start();
	}

	void AudioManager::StopLoopWave(const uint32_t index) {
		if (soundDatas_[index].pSourceVoice) {
			soundDatas_[index].pSourceVoice->Stop();
			soundDatas_[index].pSourceVoice->FlushSourceBuffers();
		}
	}

	bool AudioManager::IsSoundPlaying(const uint32_t index) {
		if (soundDatas_[index].pSourceVoice) {
			XAUDIO2_VOICE_STATE voiceState;
			soundDatas_[index].pSourceVoice->GetState(&voiceState);
			return voiceState.BuffersQueued > 0;
		}
		return false;
	}


	void AudioManager::SoundUnload(const uint32_t index) {
		delete[] soundDatas_[index].pBuffer;

		soundDatas_[index].pBuffer = 0;
		soundDatas_[index].bufferSize = 0;
		soundDatas_[index].wfex = {};
	}

}