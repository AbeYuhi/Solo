#pragma once
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Manager/ComputePipelineManager.h"

namespace MyEngine {
	class ComputeManager
	{
	private:
		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static ComputeManager* GetInstance();

		/// <summary>
		/// 初期化関数
		/// </summary>
		void Initialize();

	public:
		ComputePipelineManager* computePipelineManager_;

	};
}
