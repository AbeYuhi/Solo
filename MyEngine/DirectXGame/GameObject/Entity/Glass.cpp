#include "Glass.h"

Glass::Glass(){}
Glass::~Glass(){}

void Glass::Initialize(std::shared_ptr<Model> model,
	RenderItem* renderItem,
	GlassInfo info) {

	model_ = model;
	isBreak = false;

	groudingInfo_.up = info.groundingInfosUp;
	groudingInfo_.down = info.groundingInfosDown;
	groudingInfo_.left = info.groundingInfosLeft;
	groudingInfo_.rigft = info.groundingInfosRight;
	if (info.moveType == "DONTMOVE") {
		type_ = DONTMOVE;
	}
	else if (info.moveType == "ALTERNATE_LEFT_RIGHT") {
		type_ = ALTERNATE_LEFT_RIGHT;
	}
	else if (info.moveType == "UPRIGHT") {
		type_ = UPRIGHT;
	}

	renderItem_.Initialize();
	renderItem_.worldTransform_.data_ = renderItem->worldTransform_.data_;
	renderItem_.materialInfo_.material_->color.w = 0.1f;

	divisionX_ = info.verticalDivisions;
	divisionY_ = info.horizontalDivisions;

	mainColldier_.Initialize(renderItem_.worldTransform_.GetPEulerTransformData(), { .scale_ = {2.0f, 2.0f, 2.0f}, .rotate_ = {0.0f, 0.0f, 0.0f}, .translate_ = {0.0f, 0.0f, 0.0f}}, GLASS, kOBB, true);
	//CollisionManager::GetInstance()->AddCollider(&mainColldier_);

	// ガラス全体のサイズを取得（例としてX, Y, Z軸方向のサイズを sizeX, sizeY, sizeZ とする）
	float sizeX = renderItem_.worldTransform_.data_.scale_.x * 2.0f;
	float sizeY = renderItem_.worldTransform_.data_.scale_.y * 2.0f;
	float sizeZ = renderItem_.worldTransform_.data_.scale_.z * 2.0f;

	// 分割片のサイズを計算
	float segmentWidth = sizeX / divisionX_;
	float segmentHeight = sizeY / divisionY_;

	// 基準点（ガラス全体の中心点）
	float baseX = renderItem_.worldTransform_.data_.translate_.x;
	float baseY = renderItem_.worldTransform_.data_.translate_.y;
	float baseZ = renderItem_.worldTransform_.data_.translate_.z;

	for (unsigned int y = 0; y < divisionY_; y++) {
		renderItems_.push_back(std::vector<std::unique_ptr<RenderItem>>()); // 新しい行を追加
		colliders_.push_back(std::vector<std::unique_ptr<Collider>>()); // 新しい行を追加
		for (unsigned int x = 0; x < divisionX_; x++) {
			std::unique_ptr<RenderItem> item = std::make_unique<RenderItem>();
			item->Initialize();
			item->worldTransform_.data_.scale_.x = segmentWidth / 2.0f;
			item->worldTransform_.data_.scale_.y = segmentHeight / 2.0f;
			item->worldTransform_.data_.scale_.z = sizeZ;
			item->materialInfo_.material_->color.w = 0.1f;

			item->worldTransform_.data_.translate_.x = baseX - (sizeX / 2) + (x + 0.5f) * segmentWidth;
			item->worldTransform_.data_.translate_.y = baseY - (sizeY / 2) + (y + 0.5f) * segmentHeight;
			item->worldTransform_.data_.translate_.z = baseZ;

			std::unique_ptr<Collider> colliderItem = std::make_unique<Collider>();
			colliderItem->Initialize(item->worldTransform_.GetPEulerTransformData(), { .scale_ = {2.0f, 2.0f, 2.0f}, .rotate_ = {0.0f, 0.0f, 0.0f}, .translate_ = {0.0f, 0.0f, 0.0f} }, GLASS, kOBB, true);

			colliders_[y].push_back(std::move(colliderItem));
			renderItems_[y].push_back(std::move(item));
		}
	}
}

void Glass::Update() {

	if (mainColldier_.isContact_[BULLET] && !isBreak) {
		isBreak = true;
		mainColldier_.isDelete_ = true;

		for (unsigned int y = 0; y < divisionY_; y++) {
			for (unsigned int x = 0; x < divisionX_; x++) {
				CollisionManager::GetInstance()->AddCollider(colliders_[y][x].get());
			}
		}
	}

	if (isBreak) {
		for (unsigned int y = 0; y < divisionY_; y++) {
			for (unsigned int x = 0; x < divisionX_; x++) {
				if (colliders_[y][x]->isContact_[BULLET]) {
					colliders_[y][x]->isDelete_ = true;
				}
			}
		}

		if (groudingInfo_.up) {
			for (unsigned int x = 0; x < divisionX_; x++) {

			}
		}
		if (groudingInfo_.down) {

		}
		if (groudingInfo_.left) {

		}
		if (groudingInfo_.rigft) {

		}
		for (unsigned int y = 0; y < divisionY_; y++) {
			for (unsigned int x = 0; x < divisionX_; x++) {
				

			}
		}
	}

	if (MainCamera::GetInstance()->GetWorldPos().z >= renderItem_.worldTransform_.data_.translate_.z + 1.0f) {
		for (unsigned int y = 0; y < divisionY_; y++) {
			for (unsigned int x = 0; x < divisionX_; x++) {
				colliders_[y][x]->isDelete_ = true;
			}
		}
	}

}

void Glass::Draw() {

	/*if (isBreak) {
		for (unsigned int y = 0; y < divisionY_; y++) {
			for (unsigned int x = 0; x < divisionX_; x++) {

				model_->Draw(*renderItems_[y][x], 1);

			}
		}
	}
	else {
		model_->Draw(renderItem_, 1);
	}*/

}