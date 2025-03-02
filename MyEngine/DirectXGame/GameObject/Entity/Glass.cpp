#include "Glass.h"

/// <summary>
/// Glass.cpp
/// ガラスクラスの実装ファイル
/// </summary>

Glass::Glass(){}
Glass::~Glass(){}

void Glass::Initialize(std::shared_ptr<MyEngine::Model> model,
	std::shared_ptr<MyEngine::RenderItem> renderItem,
	Collider* collider,
	GlassInfo info) {

	mainInfo_.Initialize(model, renderItem, 1);
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

	moveState_ = std::make_unique<GlassMove>(this, type_);

	mainInfo_.renderItem->materialInfo_.material_->color.w = 0.5f;
	mainInfo_.renderItem->materialInfo_.material_->color.x = 0.5f;
	mainInfo_.renderItem->materialInfo_.material_->color.y = 0.5f;
	mainInfo_.renderItem->materialInfo_.material_->color.z = 0.5f;
	mainInfo_.renderItem->materialInfo_.material_->enableLightint = 1;
	mainInfo_.renderItem->materialInfo_.material_->isSpecularReflection = true;

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
		Matrix4x4 rotateMatrix = MakeRotateMatrix(mainInfo_.renderItem->worldTransform_.data_.rotate_);
		Vector3 newPos = Transform({ 0.0f, 1.0f, 0.0f }, rotateMatrix);
		newPos *= mainInfo_.renderItem->worldTransform_.data_.scale_.y;
		mainInfo_.renderItem->worldTransform_.data_.translate_.x = base_.x + newPos.x;
		mainInfo_.renderItem->worldTransform_.data_.translate_.y = 1 + newPos.y;
		mainInfo_.renderItem->worldTransform_.data_.translate_.z = base_.z + newPos.z;

		// 基準点（ガラス全体の中心点）
		base_.x = mainInfo_.renderItem->worldTransform_.data_.translate_.x;
		base_.y = mainInfo_.renderItem->worldTransform_.data_.translate_.y;
		base_.z = mainInfo_.renderItem->worldTransform_.data_.translate_.z;
	}

	//小さい破片ごとの初期化
	for (unsigned int y = 0; y < divisionY_; y++) {
		infos_.push_back(std::vector<std::unique_ptr<ModelDrawInfo>>()); // 新しい行を追加
		colliders_.push_back(std::vector<GlassPiece>()); // 新しい行を追加
		for (unsigned int x = 0; x < divisionX_; x++) {
			std::unique_ptr<ModelDrawInfo> item = std::make_unique<ModelDrawInfo>();
			item->Initialize(model, nullptr, 1);
			if (mainInfo_.renderItem->worldTransform_.parent_) {
				float invDivX = 1.0f / divisionX_;
				float invDivY = 1.0f / divisionY_;
				float scaleZ = 1.0f;

				float baseX = -1.0f + invDivX;
				float baseY = -1.0f + invDivY;

				Vector3 translate;
				translate.x = baseX + invDivX * x * 2.0f;
				translate.y = baseY + invDivY * y * 2.0f;
				translate.z = mainInfo_.renderItem->worldTransform_.data_.translate_.z;

				item->renderItem->worldTransform_.data_.scale_ = { invDivX, invDivY, scaleZ };
				item->renderItem->worldTransform_.data_.translate_ = translate;
				item->renderItem->materialInfo_.material_->color.w = 0.5f;
				item->renderItem->materialInfo_.material_->color.x = 0.5f;
				item->renderItem->materialInfo_.material_->color.y = 0.5f;
				item->renderItem->materialInfo_.material_->color.z = 0.5f;
				item->renderItem->materialInfo_.material_->enableLightint = 1;
				item->renderItem->materialInfo_.material_->isSpecularReflection = true;

				// 親トランスフォームの設定
				item->renderItem->worldTransform_.parent_ = &mainInfo_.renderItem->worldTransform_;
			}
			else {
				item->renderItem->worldTransform_.data_.scale_.x = segmentWidth_ / 2.0f;
				item->renderItem->worldTransform_.data_.scale_.y = segmentHeight_ / 2.0f;
				item->renderItem->worldTransform_.data_.scale_.z = size_.z / 2.0f;
				item->renderItem->worldTransform_.data_.rotate_ = mainInfo_.renderItem->worldTransform_.data_.rotate_;
				item->renderItem->materialInfo_.material_->color.w = 0.5f;
				item->renderItem->materialInfo_.material_->color.x = 0.5f;
				item->renderItem->materialInfo_.material_->color.y = 0.5f;
				item->renderItem->materialInfo_.material_->color.z = 0.5f;
				item->renderItem->materialInfo_.material_->enableLightint = 1;
				item->renderItem->materialInfo_.material_->isSpecularReflection = true;

				// ローカル位置を計算
				float localX = -size_.x / 2.0f + (x + 0.5f) * segmentWidth_;
				float localY = -size_.y / 2.0f + (y + 0.5f) * segmentHeight_;
				float localZ = 0.0f; // Z方向は固定

				if (type_ == UPRIGHT) {
					Vector3 localPosition = { localX, localY, localZ };

					// 回転を適用
					Matrix4x4 rotationMatrix = MakeRotateMatrix(mainInfo_.renderItem->worldTransform_.data_.rotate_);
					Vector3 rotatedPosition = Transform(localPosition, rotationMatrix);

					item->renderItem->worldTransform_.data_.translate_.x = base_.x + rotatedPosition.x;
					item->renderItem->worldTransform_.data_.translate_.y = base_.y + rotatedPosition.y;
					item->renderItem->worldTransform_.data_.translate_.z = base_.z + rotatedPosition.z;
				}
				else {
					item->renderItem->worldTransform_.data_.translate_.x = base_.x + localX;
					item->renderItem->worldTransform_.data_.translate_.y = base_.y + localY;
					item->renderItem->worldTransform_.data_.translate_.z = base_.z + localZ;
				}
			}

			GlassPiece colliderItem;
			colliderItem.particle = std::make_unique<GlassPieceParticle>(100);
			colliderItem.particle->Initialize();
			colliderItem.particle->GetEmitterPointer()->transform = item->renderItem->worldTransform_.worldData_;
			colliderItem.particle->SetIsPopParticle(false);
			colliderItem.isConnected = false;
			colliderItem.isBreaked = false;
			colliderItem.breakTime = 0.0f;
			colliderItem.collider = std::make_unique<Collider>();
			colliderItem.collider->Initialize(&item->renderItem->worldTransform_, { .scale_ = {2.0f, 2.0f, 2.0f}, .rotate_ = {0.0f, 0.0f, 0.0f}, .translate_ = {0.0f, 0.0f, 0.0f} }, GLASS, kOBB, true);

			colliders_[y].push_back(std::move(colliderItem));
			infos_[y].push_back(std::move(item));
		}
	}

	//ガラスが割れた時の音
	int glassSoundNum = MyEngine::RandomManager::GetInstance()->GetRandomNumber(0, 2);
	if (glassSoundNum == 0) {
		glassSound_ = MyEngine::AudioManager::GetInstance()->SoundLoadMp3("glassSound01.mp3");
	}
	if (glassSoundNum == 1) {
		glassSound_ = MyEngine::AudioManager::GetInstance()->SoundLoadMp3("glassSound02.mp3");
	}
	if (glassSoundNum == 2) {
		glassSound_ = MyEngine::AudioManager::GetInstance()->SoundLoadMp3("glassSound03.mp3");
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
	moveState_->Move();

	//本体のガラスの移動に基づいて細分化されているガラスも同期させる処理
	for (unsigned int y = 0; y < divisionY_; y++) {
		for (unsigned int x = 0; x < divisionX_; x++) {
			if (!colliders_[y][x].isBreaked) {
				if (mainInfo_.renderItem->worldTransform_.parent_) {
					float invDivX = 1.0f / divisionX_;
					float invDivY = 1.0f / divisionY_;
					float scaleZ = 1.0f;

					float baseX = -1.0f + invDivX;
					float baseY = -1.0f + invDivY;

					Vector3 translate;
					translate.x = baseX + invDivX * x * 2.0f;
					translate.y = baseY + invDivY * y * 2.0f;
					translate.z = mainInfo_.renderItem->worldTransform_.data_.translate_.z;

					infos_[y][x]->renderItem->worldTransform_.data_.scale_ = { invDivX, invDivY, scaleZ };
					infos_[y][x]->renderItem->worldTransform_.data_.translate_ = translate;
				}
				else {
					infos_[y][x]->renderItem->worldTransform_.data_.rotate_ = mainInfo_.renderItem->worldTransform_.data_.rotate_;

					// ローカル位置を計算
					float localX = -size_.x / 2.0f + (x + 0.5f) * segmentWidth_;
					float localY = -size_.y / 2.0f + (y + 0.5f) * segmentHeight_;
					float localZ = 0.0f; // Z方向は固定

					if (type_ == UPRIGHT) {
						Vector3 localPosition = { localX, localY, localZ };
						// 回転を適用
						Matrix4x4 rotationMatrix = MakeRotateMatrix(mainInfo_.renderItem->worldTransform_.data_.rotate_);
						Vector3 rotatedPosition = Transform(localPosition, rotationMatrix);

						infos_[y][x]->renderItem->worldTransform_.data_.translate_.x = mainInfo_.renderItem->worldTransform_.data_.translate_.x + rotatedPosition.x;
						infos_[y][x]->renderItem->worldTransform_.data_.translate_.y = mainInfo_.renderItem->worldTransform_.data_.translate_.y + rotatedPosition.y;
						infos_[y][x]->renderItem->worldTransform_.data_.translate_.z = mainInfo_.renderItem->worldTransform_.data_.translate_.z + rotatedPosition.z;
					}
					else {
						infos_[y][x]->renderItem->worldTransform_.data_.translate_.x = mainInfo_.renderItem->worldTransform_.data_.translate_.x + localX;
						infos_[y][x]->renderItem->worldTransform_.data_.translate_.y = mainInfo_.renderItem->worldTransform_.data_.translate_.y + localY;
						infos_[y][x]->renderItem->worldTransform_.data_.translate_.z = mainInfo_.renderItem->worldTransform_.data_.translate_.z + localZ;
					}
				}

				//パーティクル
				colliders_[y][x].particle->GetEmitterPointer()->transform.translate_ = infos_[y][x]->renderItem->worldTransform_.GetWorldPos();
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

		//接触面とくっついていないガラスピースを検出し壊す処理
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
					MyEngine::AudioManager::GetInstance()->SoundPlayMp3(glassSound_, kGlassSoundVolume_);
					colliders_[y][x].velocity = (colliders_[y][x].collider->normal_ * 3.0f) * -1.0f;
					colliders_[y][x].velocity.z *= 1.5f;
					colliders_[y][x].particle->SetIsPopParticle(true);
				}
				else {
					colliders_[y][x].particle->SetIsPopParticle(false);
				}
				colliders_[y][x].breakTime += 1.0f / 60.0f;
				colliders_[y][x].velocity.y -= gravity_ * (1.0f / 60.0f);
				infos_[y][x]->renderItem->worldTransform_.data_.translate_ += colliders_[y][x].velocity * (1.0f / 60.0f);

				if (colliders_[y][x].breakTime >= 10.0f) {
					infos_[y][x]->renderItem->materialInfo_.isInvisible_ = false;
				}
			}
		}
	}

	//ガラスがカメラの裏側に行ったらコライダーから消すように
	mainInfo_.renderItem->worldTransform_.UpdateWorld();
	if (MainCamera::GetInstance()->GetWorldPos().z >= mainInfo_.renderItem->worldTransform_.worldPos_.z + 1.0f) {
		for (unsigned int y = 0; y < divisionY_; y++) {
			for (unsigned int x = 0; x < divisionX_; x++) {
				mainColldier_->isDelete_ = true;
				colliders_[y][x].collider->isDelete_ = true;
			}
		}
	}

	//ガラスが割れるパーティクルの更新
	for (unsigned int y = 0; y < divisionY_; y++) {
		for (unsigned int x = 0; x < divisionX_; x++) {
			colliders_[y][x].particle->Update();
		}
	}
}

void Glass::Draw() {
	if (isBreak) {
		for (unsigned int y = 0; y < divisionY_; y++) {
			for (unsigned int x = 0; x < divisionX_; x++) {
				if (!colliders_[y][x].isBreaked) {
					DrawManager::GetInstance()->PushBackTranslucentObject(infos_[y][x].get());
				}
			}
		}
	}
	else {
		DrawManager::GetInstance()->PushBackTranslucentObject(&mainInfo_);
	}

	for (unsigned int y = 0; y < divisionY_; y++) {
		for (unsigned int x = 0; x < divisionX_; x++) {
			DrawManager::GetInstance()->PushBackParticle(
				MyEngine::Particle([particle = colliders_[y][x].particle.get()]() { particle->Draw(); })
			);
		}
	}
}

void DontMove::Move([[maybe_unused]] Glass* glass) {
	
}
	
void UpRight::Move(Glass* glass) {
	if (MainCamera::GetInstance()->GetWorldPos().z >= glass->GetModelData()->translate_.z - 50.0f) {
		time_ += 1.0f / 60.0f;
		if (time_ >= 1.0f) {
			time_ = 1.0f;
		}
		glass->GetModelData()->rotate_.x = (1.0f - time_) * glass->GetKeepData().rotate_.x + time_ * 0.0f;
	}

	Matrix4x4 rotateMatrix = MakeRotateMatrix(glass->GetModelData()->rotate_);
	Vector3 newPos = Transform({ 0.0f, 1.0f, 0.0f }, rotateMatrix);
	newPos *= glass->GetModelData()->scale_.y;
	glass->GetModelData()->translate_.x = glass->GetBaseData().x + newPos.x;
	glass->GetModelData()->translate_.y = 1 + newPos.y;
	glass->GetModelData()->translate_.z = glass->GetBaseData().z + newPos.z;
}

void AlternateLeftRight::Move(Glass* glass) {
	if (!isTurnAround_) {
		glass->GetModelData()->translate_.x += glass->GetMoveSpeed() * (1.0f / 60.0f);
		if (glass->GetModelData()->translate_.x >= glass->GetKeepData().translate_.x + glass->GetMoveLimit().x) {
			isTurnAround_ = true;
		}
	}
	else {
		glass->GetModelData()->translate_.x -= glass->GetMoveSpeed() * (1.0f / 60.0f);
		if (glass->GetModelData()->translate_.x <= glass->GetKeepData().translate_.x - glass->GetMoveLimit().x) {
			isTurnAround_ = false;
		}
	}
}

void AlternateUpDown::Move(Glass* glass) {
	if (!isTurnAround_) {
		glass->GetModelData()->translate_.y += glass->GetMoveSpeed() * (1.0f / 60.0f);
		if (glass->GetModelData()->translate_.y >= glass->GetKeepData().translate_.y + glass->GetMoveLimit().y) {
			isTurnAround_ = true;
		}
	}
	else {
		glass->GetModelData()->translate_.y -= glass->GetMoveSpeed() * (1.0f / 60.0f);
		if (glass->GetModelData()->translate_.y <= glass->GetKeepData().translate_.y - glass->GetMoveLimit().y) {
			isTurnAround_ = false;
		}
	}
}

GlassMove::GlassMove(Glass* glass, Glass::MoveType type) {
	glass_ = glass;
	switch (type)
	{
	case Glass::DONTMOVE:
	default:
		state_ = std::make_unique<DontMove>();
		break;
	case Glass::ALTERNATE_LEFT_RIGHT:
		state_ = std::make_unique<AlternateLeftRight>();
		break;
	case Glass::ALTERNATE_UP_DOWN:
		state_ = std::make_unique<AlternateUpDown>();
		break;
	case Glass::UPRIGHT:
		state_ = std::make_unique<UpRight>();
		break;
	}
}

GlassMove::~GlassMove() {
	state_.reset();
}

void GlassMove::Move() {
	state_->Move(glass_);
}
