#pragma once
#include <vector>
#include "glm/glm/vec2.hpp"
#include "components/drawable_object.hpp"
#include "components/component.hpp"
#include <memory>

namespace visibility {
struct BlockingObject;
std::vector<BlockingObject*> blocking_objects;

struct BlockingObject : public components::Component {
	BlockingObject(): components::Component() {
		// assert(dynamic_cast<render::DrawableObject*>(this) != null);
		blocking_objects.push_back(this);
	}
	virtual render::DrawableObject* get_drawable() {
		entity::Entity* e = get_entity();
		return e->get<render::DrawableObject>();
	}
	virtual ~BlockingObject() {}
};


struct BlockingEntity: public BlockingObject {
	BlockingEntity(std::unique_ptr<entity::Entity> e): e(std::move(e)), BlockingObject() {}
	~BlockingEntity() {}
	std::unique_ptr<entity::Entity> e;
	virtual render::DrawableObject* get_drawable() {
		return e->get<render::DrawableObject>();
	}
};

struct BlockingEntityPtr: public BlockingObject {
	BlockingEntityPtr(entity::Entity* e): e(e), BlockingObject() {}
	~BlockingEntityPtr() {}
	entity::Entity* e;
	virtual render::DrawableObject* get_drawable() {
		return e->get<render::DrawableObject>();
	}
};
}