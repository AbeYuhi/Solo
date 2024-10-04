#pragma once
#include "Manager/CollisionManager.h"
#include "Object/Model.h"
#include "Data/RenderItem.h"

class Glass
{
public:
	enum MoveType {
		DONTMOVE,
		ALTERNATE_LEFT_RIGHT,
		UPRIGHT
	};
	struct GroundingInfo {
		bool up;
		bool down;
		bool rigft;
		bool left;
	};

public:
	Glass();
	~Glass();

	void Initialize(std::shared_ptr<Model> model,
		RenderItem* renderItem);

	void Update();

	void Draw();


private:

	std::shared_ptr<Model> model_;
	RenderItem renderItem_;


};

