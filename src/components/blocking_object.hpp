#pragma once
#include <vector>
#include "glm/glm/vec2.hpp"
#include "components/component.hpp"
#include <memory>

namespace visibility {
struct BlockingObject;
std::vector<BlockingObject*> blocking_objects;

struct BlockingObject : public components::Component {
	BlockingObject(): components::Component() {
		blocking_objects.push_back(this);
	}
	virtual entity::Entity* get_entity() {
		return get_entity();
	}
	virtual ~BlockingObject() {}
};


struct BlockingEntity: public BlockingObject {
	BlockingEntity(std::unique_ptr<entity::Entity> e): e(std::move(e)), BlockingObject() {}
	~BlockingEntity() {}
	std::unique_ptr<entity::Entity> e;
	virtual entity::Entity* get_entity() {
		return e.get();
	}
};

struct BlockingEntityPtr: public BlockingObject {
	BlockingEntityPtr(entity::Entity* e): e(e), BlockingObject() {}
	~BlockingEntityPtr() {}
	entity::Entity* e;
	virtual entity::Entity* get_entity() {
		return e;
	}
};
}