#include "Glass.h"

Glass::Glass(){}
Glass::~Glass(){}

void Glass::Initialize(std::shared_ptr<Model> model,
	RenderItem* renderItem,
	GlassInfo info) {

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

	division_.x = info.verticalDivisions;
	division_.y = info.horizontalDivisions;

	mainColldier_.Initialize(renderItem_.worldTransform_.GetPEulerTransformData(), { .scale_ = {2.0f, 2.0f, 2.0f}, .rotate_ = {0.0f, 0.0f, 0.0f}, .translate_ = {0.0f, 0.0f, 0.0f}}, GLASS, kOBB, true);
	CollisionManager::GetInstance()->AddCollider(&mainColldier_);

	for (int y = 0; y < division_.y; y++) {
		renderItems_.push_back(std::vector<std::unique_ptr<RenderItem>>()); // 新しい行を追加
		for (int x = 0; x < division_.x; x++) {
			std::unique_ptr<RenderItem> renderItem = std::make_unique<RenderItem>();
			renderItem->Initialize();
			renderItem->worldTransform_.data_.translate_.x = renderItem_.worldTransform_.data_.translate_.;
			renderItem->worldTransform_.data_.translate_.y = renderItem_.worldTransform_.data_.translate_.;
			renderItem->worldTransform_.data_.translate_.z = renderItem_.worldTransform_.data_.translate_.z;

			renderItems_[y].push_back(std::move(renderItem));
		}
	}

}

void Glass::Update() {



}

void Glass::Draw() {



}