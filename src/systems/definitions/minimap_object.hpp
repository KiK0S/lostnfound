#pragma once

#include <vector>
#include <memory>
#include "systems/definitions/drawable_object.hpp"

namespace minimap {
	

struct MiniMapObject;

std::vector<MiniMapObject*> minimap_objects;

struct MiniMapObject : public render::DrawableObject {
	MiniMapObject() {
		minimap_objects.push_back(this);
	}
	~MiniMapObject() {}

};

}