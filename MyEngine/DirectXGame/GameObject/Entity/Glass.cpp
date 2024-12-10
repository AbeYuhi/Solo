#include "Glass.h"

/// <summary>
/// Glass.cpp
/// ガラスクラスの実装ファイル
/// </summary>

Glass::Glass(){}
Glass::~Glass(){}

void Glass::Initialize(std::shared_ptr<MyEngine::Model> model,
	MyEngine::RenderItem* renderItem,
	Collider* collider,
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
	else if (info.moveType == "ALTERNATE_UP_DOWN") {
		type_ = ALTERNATE_UP_DOWN;
	}
	else if (info.moveType == "UPRIGHT") {
		type_ = UPRIGHT;
	}

	renderItem_ = renderItem;
	renderItem_->materialInfo_.material_->color.w = 0.5f;
	renderItem_->materialInfo_.material_->color.x = 0.5f;
	renderItem_->materialInfo_.material_->color.y = 0.5f;
	renderItem_->materialInfo_.material_->color.z = 0.5f;
	renderItem_->materialInfo_.material_->enableLightint = 1;

	divisionX_ = info.verticalDivisions;
	divisionY_ = info.horizontalDivisions;

	time_ = 0.0f;
	keepData_ = renderItem->worldTransform_.data_;
	if (info.moveType == "ALTERNATE_LEFT_RIGHT") {
		moveLimit_ = Vector3{ info.moveLimit, 0.0f, 0.0f };
		moveSpeed_ = info.moveSpeed;
	}
	else if (info.moveType == "ALTERNATE_UP_DOWN") {
		moveLimit_ = Vector3{ 0.0f, info.moveLimit, 0.0f };
		moveSpeed_ = info.moveSpeed;
	}
	isTurnAround_ = false;

	mainColldier_ = collider;

	// ガラス全体のサイズを取得（例としてX, Y, Z軸方向のサイズを sizeX, sizeY, sizeZ とする）
	size_.x = keepData_.scale_.x * 2.0f;
	size_.y = keepData_.scale_.y * 2.0f;
	size_.z = keepData_.scale_.z * 2.0f;

	// 分割片のサイズを計算
	segmentWidth_ = size_.x / divisionX_;
	segmentHeight_ = size_.y / divisionY_;

	// 基準点（ガラス全体の中心点）
	base_.x = keepData_.translate_.x;
	base_.y = keepData_.translate_.y;
	base_.z = keepData_.translate_.z;
	
	if (type_ == UPRIGHT) {
		Matrix4x4 rotateMatrix = MakeRotateMatrix(renderItem_->worldTransform_.data_.rotate_);
		Vector3 newPos = Transform({ 0.0f, 1.0f, 0.0f }, rotateMatrix);
		newPos *= renderItem_->worldTransform_.data_.scale_.y;
		renderItem_->worldTransform_.data_.translate_.x = base_.x + newPos.x;
		renderItem_->worldTransform_.data_.translate_.y = 1 + newPos.y;
		renderItem_->worldTransform_.data_.translate_.z = base_.z + newPos.z;

		// 基準点（ガラス全体の中心点）
		base_.x = renderItem_->worldTransform_.data_.translate_.x;
		base_.y = renderItem_->worldTransform_.data_.translate_.y;
		base_.z = renderItem_->worldTransform_.data_.translate_.z;
	}

	//小さい破片ごとの初期化
	for (unsigned int y = 0; y < divisionY_; y++) {
		renderItems_.push_back(std::vector<std::unique_ptr<MyEngine::RenderItem>>()); // 新しい行を追加
		colliders_.push_back(std::vector<GlassPiece>()); // 新しい行を追加
		for (unsigned int x = 0; x < divisionX_; x++) {
			std::unique_ptr<MyEngine::RenderItem> item = std::make_unique<MyEngine::RenderItem>();
			item->Initialize();
			if (renderItem_->worldTransform_.parent_) {
				// スケールの設定
				item->worldTransform_.data_.scale_ = {
					1.0f / divisionX_,
					1.0f / divisionY_,
					renderItem_->worldTransform_.data_.translate_.z
				};

				// 回転の引き継ぎ
				item->worldTransform_.data_.rotate_ = renderItem_->worldTransform_.data_.rotate_;

				// マテリアルの設定
				item->materialInfo_.material_->color = { 0.5f, 0.5f, 0.5f, 0.5f };
				item->materialInfo_.material_->enableLightint = 1;

				// ローカル位置を計算
				float scaleX = 1.0f / divisionX_;
				float scaleY = 1.0f / divisionY_;
				item->worldTransform_.data_.translate_ = {
					(x + 0.5f) * scaleX - 0.5f,
					(y + 0.5f) * scaleY - 0.5f,
					0.0f
				};

				// 親トランスフォームの設定
				item->worldTransform_.parent_ = &renderItem_->worldTransform_;
			}
			else {
				item->worldTransform_.data_.scale_.x = segmentWidth_ / 2.0f;
				item->worldTransform_.data_.scale_.y = segmentHeight_ / 2.0f;
				item->worldTransform_.data_.scale_.z = size_.z / 2.0f;
				item->worldTransform_.data_.rotate_ = renderItem_->worldTransform_.data_.rotate_;
				item->materialInfo_.material_->color.w = 0.5f;
				item->materialInfo_.material_->color.x = 0.5f;
				item->materialInfo_.material_->color.y = 0.5f;
				item->materialInfo_.material_->color.z = 0.5f;
				item->materialInfo_.material_->enableLightint = 1;

				// ローカル位置を計算
				float localX = -size_.x / 2.0f + (x + 0.5f) * segmentWidth_;
				float localY = -size_.y / 2.0f + (y + 0.5f) * segmentHeight_;
				float localZ = 0.0f; // Z方向は固定

				if (type_ == UPRIGHT) {
					Vector3 localPosition = { localX, localY, localZ };

					// 回転を適用
					Matrix4x4 rotationMatrix = MakeRotateMatrix(renderItem_->worldTransform_.data_.rotate_);
					Vector3 rotatedPosition = Transform(localPosition, rotationMatrix);

					item->worldTransform_.data_.translate_.x = base_.x + rotatedPosition.x;
					item->worldTransform_.data_.translate_.y = base_.y + rotatedPosition.y;
					item->worldTransform_.data_.translate_.z = base_.z + rotatedPosition.z;
				}
				else {
					item->worldTransform_.data_.translate_.x = base_.x + localX;
					item->worldTransform_.data_.translate_.y = base_.y + localY;
					item->worldTransform_.data_.translate_.z = base_.z + localZ;
				}
			}

			GlassPiece colliderItem;
			colliderItem.particle = std::make_unique<GlassPieceParticle>(100);
			colliderItem.particle->Initialize();
			colliderItem.emitter.transform.Initialize();
			colliderItem.emitter.transform.rotate_ = item->worldTransform_.data_.rotate_;
			colliderItem.emitter.transform.scale_ = item->worldTransform_.data_.scale_;
			colliderItem.isConnected = false;
			colliderItem.isBreaked = false;
			colliderItem.breakTime = 0.0f;
			colliderItem.collider = std::make_unique<Collider>();
			colliderItem.collider->Initialize(&item->worldTransform_, { .scale_ = {2.0f, 2.0f, 2.0f}, .rotate_ = {0.0f, 0.0f, 0.0f}, .translate_ = {0.0f, 0.0f, 0.0f} }, GLASS, kOBB, true);

			colliders_[y].push_back(std::move(colliderItem));
			renderItems_[y].push_back(std::move(item));
		}
	}
}

void Glass::Update() {

	if (mainColldier_->isContact_[BULLET] && !isBreak) {
		isBreak = true;
		mainColldier_->isDelete_ = true;
		for (unsigned int y = 0; y < divisionY_; y++) {
			for (unsigned int x = 0; x < divisionX_; x++) {
				MyEngine::CollisionManager::GetInstance()->AddCollider(colliders_[y][x].collider.get());
			}
		}
	}

	//動くタイプ別の挙動
	//本体のガラスを動かす処理
	switch (type_)
	{
	case Glass::DONTMOVE:
		DontMoveGlass();
		break;
	case Glass::ALTERNATE_LEFT_RIGHT:
		MoveGlassAlternateLeftRight();
		break;
	case Glass::ALTERNATE_UP_DOWN:
		MoveGlassAlternateUpDown();
		break;
	case Glass::UPRIGHT:
		MoveGlassUpRight();
		break;
	}

	//本体のガラスの移動に基づいて細分化されているガラスも同期させる処理
	for (unsigned int y = 0; y < divisionY_; y++) {
		for (unsigned int x = 0; x < divisionX_; x++) {
			if (!colliders_[y][x].isBreaked) {
				if (renderItem_->worldTransform_.parent_) {
					float invDivX = 1.0f / divisionX_;
					float invDivY = 1.0f / divisionY_;

					float baseX = -0.5f + 0.5f * invDivX;
					float baseY = -0.5f + 0.5f * invDivY;

					float scaleZ = renderItem_->worldTransform_.data_.translate_.z;

					renderItems_[y][x]->worldTransform_.data_.scale_ = { invDivX, invDivY, scaleZ };
					renderItems_[y][x]->worldTransform_.data_.translate_ = {
						baseX + x * invDivX,
						baseY + y * invDivY,
						0.0f
					};
				}
				else {
					renderItems_[y][x]->worldTransform_.data_.rotate_ = renderItem_->worldTransform_.data_.rotate_;

					// ローカル位置を計算
					float localX = -size_.x / 2.0f + (x + 0.5f) * segmentWidth_;
					float localY = -size_.y / 2.0f + (y + 0.5f) * segmentHeight_;
					float localZ = 0.0f; // Z方向は固定

					if (type_ == UPRIGHT) {
						Vector3 localPosition = { localX, localY, localZ };
						// 回転を適用
						Matrix4x4 rotationMatrix = MakeRotateMatrix(renderItem_->worldTransform_.data_.rotate_);
						Vector3 rotatedPosition = Transform(localPosition, rotationMatrix);

						renderItems_[y][x]->worldTransform_.data_.translate_.x = renderItem_->worldTransform_.data_.translate_.x + rotatedPosition.x;
						renderItems_[y][x]->worldTransform_.data_.translate_.y = renderItem_->worldTransform_.data_.translate_.y + rotatedPosition.y;
						renderItems_[y][x]->worldTransform_.data_.translate_.z = renderItem_->worldTransform_.data_.translate_.z + rotatedPosition.z;
					}
					else {
						renderItems_[y][x]->worldTransform_.data_.translate_.x = renderItem_->worldTransform_.data_.translate_.x + localX;
						renderItems_[y][x]->worldTransform_.data_.translate_.y = renderItem_->worldTransform_.data_.translate_.y + localY;
						renderItems_[y][x]->worldTransform_.data_.translate_.z = renderItem_->worldTransform_.data_.translate_.z + localZ;
					}
				}

				//パーティクル
				colliders_[y][x].emitter.count = 20;
				colliders_[y][x].emitter.frequency = 9999.0f;
				colliders_[y][x].emitter.frequencyTime = 0.0f;
				colliders_[y][x].emitter.transform.translate_ = renderItems_[y][x]->worldTransform_.GetWorldPos();
			}
		}
	}

	//本体のガラスが割れたときに接地面とくっついていないガラスを破壊する処理
	if (isBreak) {
		for (unsigned int y = 0; y < divisionY_; y++) {
			for (unsigned int x = 0; x < divisionX_; x++) {
				if (colliders_[y][x].collider->isContact_[BULLET]) {
					colliders_[y][x].isBreaked = true;
				}
				colliders_[y][x].isConnected = false;
			}
		}

		if (groudingInfo_.up) {
			for (unsigned int x = 0; x < divisionX_; x++) {
				if (!colliders_[divisionY_ - 1][x].isBreaked) {
					colliders_[divisionY_ - 1][x].isConnected = true;
				}
			}
		}
		if (groudingInfo_.down) {
			for (unsigned int x = 0; x < divisionX_; x++) {
				if (!colliders_[0][x].isBreaked) {
					colliders_[0][x].isConnected = true;
				}
			}
		}
		if (groudingInfo_.left) {
			for (unsigned int y = 0; y < divisionY_; y++) {
				if (!colliders_[y][0].isBreaked) {
					colliders_[y][0].isConnected = true;
				}
			}
		}
		if (groudingInfo_.rigft) {
			for (unsigned int y = 0; y < divisionY_; y++) {
				if (!colliders_[y][divisionX_ - 1].isBreaked) {
					colliders_[y][divisionX_ - 1].isConnected = true;
				}
			}
		}

		bool isChange = false;
		while (true) {
			isChange = false;
			for (unsigned int y = 0; y < divisionY_; y++) {
				for (unsigned int x = 0; x < divisionX_; x++) {
					if (!colliders_[y][x].isBreaked && colliders_[y][x].isConnected) {
						
						if (y == 0 && x == 0) {	//左下の場合
							if (!colliders_[y + 1][x].isBreaked && !colliders_[y + 1][x].isConnected) {
								colliders_[y + 1][x].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y][x + 1].isBreaked && !colliders_[y][x + 1].isConnected) {
								colliders_[y][x + 1].isConnected = true;
								isChange = true;
							}
						}
						else if (y == 0 && x == divisionX_ - 1) {	//右下の場合
							if (!colliders_[y + 1][x].isBreaked && !colliders_[y + 1][x].isConnected) {
								colliders_[y + 1][x].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y][x - 1].isBreaked && !colliders_[y][x - 1].isConnected) {
								colliders_[y][x - 1].isConnected = true;
								isChange = true;
							}
						}
						else if (y == divisionY_ - 1 && x == 0) {	//左上の場合
							if (!colliders_[y - 1][x].isBreaked && !colliders_[y - 1][x].isConnected) {
								colliders_[y - 1][x].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y][x + 1].isBreaked && !colliders_[y][x + 1].isConnected) {
								colliders_[y][x + 1].isConnected = true;
								isChange = true;
							}
						}
						else if (y == divisionY_ - 1 && x == divisionX_ - 1) {	//右上の場合
							if (!colliders_[y - 1][x].isBreaked && !colliders_[y - 1][x].isConnected) {
								colliders_[y - 1][x].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y][x - 1].isBreaked && !colliders_[y][x - 1].isConnected) {
								colliders_[y][x - 1].isConnected = true;
								isChange = true;
							}
						}
						else if (y == 0) {	//下のラインの場合
							if (!colliders_[y + 1][x].isBreaked && !colliders_[y + 1][x].isConnected) {
								colliders_[y + 1][x].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y][x - 1].isBreaked && !colliders_[y][x - 1].isConnected) {
								colliders_[y][x - 1].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y][x + 1].isBreaked && !colliders_[y][x + 1].isConnected) {
								colliders_[y][x + 1].isConnected = true;
								isChange = true;
							}
						}
						else if (y == divisionY_ - 1) {	//上のラインの場合
							if (!colliders_[y - 1][x].isBreaked && !colliders_[y - 1][x].isConnected) {
								colliders_[y - 1][x].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y][x - 1].isBreaked && !colliders_[y][x - 1].isConnected) {
								colliders_[y][x - 1].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y][x + 1].isBreaked && !colliders_[y][x + 1].isConnected) {
								colliders_[y][x + 1].isConnected = true;
								isChange = true;
							}
						}
						else if (x == 0) {	//左のラインの場合
							if (!colliders_[y - 1][x].isBreaked && !colliders_[y - 1][x].isConnected) {
								colliders_[y - 1][x].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y + 1][x].isBreaked && !colliders_[y + 1][x].isConnected) {
								colliders_[y + 1][x].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y][x + 1].isBreaked && !colliders_[y][x + 1].isConnected) {
								colliders_[y][x + 1].isConnected = true;
								isChange = true;
							}
						}
						else if (x == divisionX_ - 1) {	//右のラインの場合
							if (!colliders_[y - 1][x].isBreaked && !colliders_[y - 1][x].isConnected) {
								colliders_[y - 1][x].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y + 1][x].isBreaked && !colliders_[y + 1][x].isConnected) {
								colliders_[y + 1][x].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y][x - 1].isBreaked && !colliders_[y][x - 1].isConnected) {
								colliders_[y][x - 1].isConnected = true;
								isChange = true;
							}
						}
						else {	//それ以外
							if (!colliders_[y - 1][x].isBreaked && !colliders_[y - 1][x].isConnected) {
								colliders_[y - 1][x].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y + 1][x].isBreaked && !colliders_[y + 1][x].isConnected) {
								colliders_[y + 1][x].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y][x - 1].isBreaked && !colliders_[y][x - 1].isConnected) {
								colliders_[y][x - 1].isConnected = true;
								isChange = true;
							}
							if (!colliders_[y][x + 1].isBreaked && !colliders_[y][x + 1].isConnected) {
								colliders_[y][x + 1].isConnected = true;
								isChange = true;
							}
						}
					}
				}
			}

			if (!isChange) {
				break;
			}
		}
		for (unsigned int y = 0; y < divisionY_; y++) {
			for (unsigned int x = 0; x < divisionX_; x++) {
				if (!colliders_[y][x].isConnected) {
					colliders_[y][x].isBreaked = true;
				}
			}
		}
	}

	const float gravity_ = 2.8f;
	//細分化されたガラスが破壊された後の演出面
	for (unsigned int y = 0; y < divisionY_; y++) {
		for (unsigned int x = 0; x < divisionX_; x++) {
			if (colliders_[y][x].isBreaked) {
				if (colliders_[y][x].breakTime == 0.0f) {
					colliders_[y][x].collider->isDelete_ = true;
					colliders_[y][x].velocity = (colliders_[y][x].collider->normal_ * 3.0f) * -1.0f;
					colliders_[y][x].velocity.z *= 1.5f;
					colliders_[y][x].emitter.frequency = 0.0f;
				}
				else {
					colliders_[y][x].emitter.frequency = 9999.9999f;
				}
				colliders_[y][x].breakTime += 1.0f / 60.0f;
				colliders_[y][x].velocity.y -= gravity_ * (1.0f / 60.0f);
				renderItems_[y][x]->worldTransform_.data_.translate_ += colliders_[y][x].velocity * (1.0f / 60.0f);

				if (colliders_[y][x].breakTime >= 10.0f) {
					renderItems_[y][x]->materialInfo_.isInvisible_ = false;
				}
			}
		}
	}

	//壊れたガラスが壁と当たったら止める処理(仮)
	for (unsigned int y = 0; y < divisionY_; y++) {
		for (unsigned int x = 0; x < divisionX_; x++) {
			if (colliders_[y][x].isBreaked) {
				if (colliders_[y][x].collider->isContact_[WALL]) {
					renderItems_[y][x]->worldTransform_.data_.translate_ -= colliders_[y][x].velocity * (1.0f / 60.0f);
				}
			}
		}
	}

	//ガラスがカメラの裏側に行ったらコライダーから消すように
	renderItem_->worldTransform_.UpdateWorld();
	if (MainCamera::GetInstance()->GetWorldPos().z >= renderItem_->worldTransform_.worldPos_.z + 1.0f) {
		for (unsigned int y = 0; y < divisionY_; y++) {
			for (unsigned int x = 0; x < divisionX_; x++) {
				mainColldier_->isDelete_ = true;
				colliders_[y][x].collider->isDelete_ = true;
			}
		}
	}

	for (unsigned int y = 0; y < divisionY_; y++) {
		for (unsigned int x = 0; x < divisionX_; x++) {
			colliders_[y][x].particle->SetEmitter(colliders_[y][x].emitter);
			colliders_[y][x].particle->Update();
		}
	}
}

void Glass::Draw() {

	if (isBreak) {
		for (unsigned int y = 0; y < divisionY_; y++) {
			for (unsigned int x = 0; x < divisionX_; x++) {
				if (!colliders_[y][x].isBreaked) {
					model_->Draw(*renderItems_[y][x], 1);
				}
			}
		}
	}
	else {
		if (Length(renderItem_->worldTransform_.GetWorldPos() - MainCamera::GetInstance()->GetWorldPos()) <= 80.0f) {
			model_->Draw(*renderItem_, 1);
		}
	}
}

void Glass::ParticleDraw() {
	for (unsigned int y = 0; y < divisionY_; y++) {
		for (unsigned int x = 0; x < divisionX_; x++) {
			colliders_[y][x].particle->Draw();
		}
	}
}

void Glass::DontMoveGlass() {

}

void Glass::MoveGlassUpRight() {
	if (MainCamera::GetInstance()->GetWorldPos().z >= renderItem_->worldTransform_.data_.translate_.z - 50.0f) {
		time_ += 1.0f / 60.0f;
		if (time_ >= 1.0f) {
			time_ = 1.0f;
		}
		renderItem_->worldTransform_.data_.rotate_.x = (1.0f - time_) * keepData_.rotate_.x + time_ * 0.0f;
	}

	Matrix4x4 rotateMatrix = MakeRotateMatrix(renderItem_->worldTransform_.data_.rotate_);
	Vector3 newPos = Transform({ 0.0f, 1.0f, 0.0f }, rotateMatrix);
	newPos *= renderItem_->worldTransform_.data_.scale_.y;
	renderItem_->worldTransform_.data_.translate_.x = base_.x + newPos.x;
	renderItem_->worldTransform_.data_.translate_.y = 1 + newPos.y;
	renderItem_->worldTransform_.data_.translate_.z = base_.z + newPos.z;
}

void Glass::MoveGlassAlternateLeftRight() {
	if (!isTurnAround_) {
		renderItem_->worldTransform_.data_.translate_.x += moveSpeed_ * (1.0f / 60.0f);
		if (renderItem_->worldTransform_.data_.translate_.x >= keepData_.translate_.x + moveLimit_.x) {
			isTurnAround_ = true;
		}
	}
	else {
		renderItem_->worldTransform_.data_.translate_.x -= moveSpeed_ * (1.0f / 60.0f);
		if (renderItem_->worldTransform_.data_.translate_.x <= keepData_.translate_.x - moveLimit_.x) {
			isTurnAround_ = false;
		}
	}
}

void Glass::MoveGlassAlternateUpDown() {
	if (!isTurnAround_) {
		renderItem_->worldTransform_.data_.translate_.y += moveSpeed_ * (1.0f / 60.0f);
		if (renderItem_->worldTransform_.data_.translate_.y >= keepData_.translate_.y + moveLimit_.y) {
			isTurnAround_ = true;
		}
	}
	else {
		renderItem_->worldTransform_.data_.translate_.y -= moveSpeed_ * (1.0f / 60.0f);
		if (renderItem_->worldTransform_.data_.translate_.y <= keepData_.translate_.y - moveLimit_.y) {
			isTurnAround_ = false;
		}
	}
}