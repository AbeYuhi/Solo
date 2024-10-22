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


}

void Glass::Update() {

}

void Glass::Draw() {

}