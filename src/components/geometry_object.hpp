#pragma once
#include <vector>
#include <string>
#include "glm/glm/vec2.hpp"
#include "glm/glm/mat4x4.hpp"
#include "components/component.hpp"

namespace geometry {

struct GeometryObject;


std::vector<GeometryObject*> geometry_objects;

// quad, area, triangle, etd
struct GeometryObject : public components::Component {
	GeometryObject(): components::Component() {
		geometry_objects.push_back(this);
	}
	virtual ~GeometryObject() {}


	virtual std::vector<glm::vec2> get_pos() = 0;
	virtual int get_size() = 0;
	virtual std::vector<glm::vec2> get_uv() = 0;
	virtual std::string get_name() const = 0;
};

}