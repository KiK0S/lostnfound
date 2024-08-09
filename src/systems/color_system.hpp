#pragma once

#include "components/color_object.hpp"

namespace color {

struct OneColor: public ColoredObject {
	OneColor(glm::vec4 color): ColoredObject(), color(color) {}
	

	glm::vec4 get_color() {
		return color;
	}
	glm::vec4 color;
};

struct NoColorObject: public ColoredObject {
	NoColorObject(): ColoredObject() {}
	glm::vec4 get_color() {
		return glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
	}
};

NoColorObject no_color;

OneColor white({1.0f, 1.0f, 1.0f, 1.0f});
OneColor red({1.0f, 0.0f, 0.0f, 1.0f});
OneColor green({0.0f, 1.0f, 0.0f, 1.0f});
OneColor blue({0.0f, 0.0f, 1.0f, 1.0f});
OneColor light_grey({0.5f, 0.5f, 0.5f, 0.5f});
OneColor lighter_grey({0.5f, 0.5f, 0.5f, 0.3f});
OneColor pink({191.0 / 256, 4.0 / 256, 151.0 / 256, 1.0f});

}