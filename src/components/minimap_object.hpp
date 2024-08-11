#pragma once

#include <vector>
#include <memory>
#include "components/component.hpp"

namespace minimap {
	

struct MiniMapObject;

std::vector<MiniMapObject*> minimap_objects;

struct MiniMapObject: public components::Component {
	MiniMapObject(): components::Component() {
		minimap_objects.push_back(this);
	}
	~MiniMapObject() {}
	virtual entity::Entity* get_entity() {
		return get_entity();
	}
};

struct MiniMapEntity: public MiniMapObject {
	MiniMapEntity(std::unique_ptr<entity::Entity> e): e(std::move(e)), MiniMapObject() {}
	~MiniMapEntity() {}
	std::unique_ptr<entity::Entity> e;
	virtual entity::Entity* get_entity() {
		return e.get();
	}
};


struct MiniMapEntityPtr: public MiniMapObject {
	MiniMapEntityPtr(entity::Entity* e): e(e), MiniMapObject() {}
	~MiniMapEntityPtr() {}
	entity::Entity* e;
	virtual entity::Entity* get_entity() {
		return e;
	}
};


}