#pragma once
#include "glm/glm/vec4.hpp"
#include "components/component.hpp"
#include <vector>

namespace color {

struct ColoredObject;

std::vector<ColoredObject*> coloreds;

struct ColoredObject: public components::Component {
	ColoredObject(): components::Component() {
		coloreds.push_back(this);
	}
	virtual ~ColoredObject(){}
	virtual glm::vec4 get_color() = 0;
};



}
