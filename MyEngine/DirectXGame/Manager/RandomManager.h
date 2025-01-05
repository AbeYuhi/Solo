#pragma once
#include <random>

/// <summary>
/// RandomManager.h
/// ランダムを生成するためのマネージャークラス
/// </summary>

namespace MyEngine {

	class RandomManager
	{
	public:

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static RandomManager* GetInstance();

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize();

	public: //ゲッターセッター

		/// <summary>
		/// 整数の乱数を取得
		/// </summary>
		/// <param name="min"></param>
		/// <param name="max"></param>
		/// <returns></returns>
		int GetRandomNumber(int min, int max);

		/// <summary>
		/// 小数の乱数を取
		/// </summary>
		/// <param name="min"></param>
		/// <param name="max"></param>
		/// <returns></returns>
		float GetRandomNumber(float min, float max);

	private:
		RandomManager() = default;
		~RandomManager() = default;

	private: //メンバ変数
		std::mt19937 randomEngine_;
	};

}