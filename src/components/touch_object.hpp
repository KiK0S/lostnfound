#pragma once
#include "components/component.hpp"
#include <vector>

namespace touchscreen {

struct TouchObject;
std::vector<TouchObject*> touchables;

struct TouchObject: public components::Component {
	TouchObject(): components::Component() {
		touchables.push_back(this);
	}
	virtual ~TouchObject(){}
	virtual void handle_touch(glm::vec2 point) = 0;
};


}