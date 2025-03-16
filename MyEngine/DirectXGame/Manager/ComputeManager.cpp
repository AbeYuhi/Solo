#include "ComputeManager.h"

namespace MyEngine {

	ComputeManager* ComputeManager::GetInstance() {
		static ComputeManager instance;
		return &instance;
	}

	void ComputeManager::Initialize() {
		computePipelineManager_ = ComputePipelineManager::GetInstance();


	}



}