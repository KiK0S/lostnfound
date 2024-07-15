#pragma once
#include <vector>
#include "glm/glm/vec2.hpp"
#include "systems/definitions/drawable_object.hpp"

namespace visibility {
struct BlockingObject;
std::vector<BlockingObject*> blocking_objects;

struct BlockingObject: public render::DrawableObject {
	BlockingObject() {
		blocking_objects.push_back(this);
	}
	// virtual glm::vec2 get_center_position() = 0;
	// virtual float get_radius() { return 0.2; }
	virtual ~BlockingObject() {}
};
}