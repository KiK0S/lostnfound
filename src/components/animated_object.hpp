#pragma once
#include <vector>
#include <string>
#include "components/component.hpp"

namespace animation {

struct AnimatedObject;
std::vector<AnimatedObject*> animateds;

struct AnimatedObject : public components::Component {
	AnimatedObject(): components::Component() {
		animateds.push_back(this);
	}
	virtual ~AnimatedObject() {}
	virtual void update(float dt) = 0;
};

}