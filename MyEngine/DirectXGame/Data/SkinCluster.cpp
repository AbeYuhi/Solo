#include "SkinCluster.h"

/// <summary>
/// SkinCluster.cpp
/// SkinClusterを処理する関数の実装ファイル
/// </summary>

SkinCluster CreateSkinCluster(const Skeleton& skeleton, const ModelData& modeldata) {
	MyEngine::DirectXCommon* dxCommon = MyEngine::DirectXCommon::GetInstance();
	SkinCluster skinCluster;

	uint32_t srvIndex = MyEngine::SrvManager::GetInstance()->Allocate();

	//palette用のResource確保
	skinCluster.paletteResource = MyEngine::CreateBufferResource(sizeof(WellForGPU) * skeleton.joints.size());
	WellForGPU* mappedPalette = nullptr;
	skinCluster.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	skinCluster.mappedPalette = { mappedPalette, skeleton.joints.size() };
	skinCluster.paletteSrvHandle.first = MyEngine::SrvManager::GetInstance()->GetCPUDescriptorHandle(srvIndex);
	skinCluster.paletteSrvHandle.second = MyEngine::SrvManager::GetInstance()->GetGPUDescriptorHandle(srvIndex);
	skinCluster.srvIndex = srvIndex;

	//palette用のsrvを作成
	D3D12_SHADER_RESOURCE_VIEW_DESC	paletteSrvDesc{};
	paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	paletteSrvDesc.Buffer.FirstElement = 0;
	paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	paletteSrvDesc.Buffer.NumElements = UINT(skeleton.joints.size());
	paletteSrvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);
	dxCommon->GetDevice()->CreateShaderResourceView(skinCluster.paletteResource.Get(), &paletteSrvDesc,	skinCluster.paletteSrvHandle.first);

	//influence用のResource確保
	skinCluster.influenceResource = MyEngine::CreateBufferResource(sizeof(VertexInfluence) * modeldata.vertices.size());
	VertexInfluence* mappedInfluence = nullptr;
	skinCluster.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modeldata.vertices.size());
	skinCluster.mappedInfluence = {mappedInfluence, modeldata.vertices.size()};

	//influence用のVBVを作成
	skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
	skinCluster.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modeldata.vertices.size());
	skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

	//InverceBindPoseMatrixの保存領域を作成
	skinCluster.inverseBindPoseMatrices.resize(skeleton.joints.size());
	for (auto& inverseBindPoseMatrix : skinCluster.inverseBindPoseMatrices) {
		inverseBindPoseMatrix = MakeIdentity4x4();
	}

	//ModelDataのSkinCluaster情報を解析してInfluenceの中身を埋める
	for (const auto& jointWeight : modeldata.skinClusterData) {
		auto it = skeleton.jointMap.find(jointWeight.first);
		if (it == skeleton.jointMap.end()) {
			continue;
		}
		//(*it).secondにはjointのindexが入っているので、該当のindexのinverseBindPoseMatrixを代入
		skinCluster.inverseBindPoseMatrices[it->second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			auto& currntInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex];
			for (uint32_t index = 0; index < kNumMaxInfluence; index++) {
				if (currntInfluence.weights[index] == 0.0f) {
					currntInfluence.weights[index] = vertexWeight.weight;
					currntInfluence.jointIndices[index] = it->second;
					break;
				}
			}
		}
	}

	return skinCluster;
}

void ClearSkinCluster(SkinCluster& skinCluster) {
	MyEngine::SrvManager::GetInstance()->UnLoadResource(skinCluster.srvIndex);
	skinCluster.paletteResource.Reset();
}
