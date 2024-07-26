#pragma once
#include <iostream>


namespace components {

struct Component;
}
namespace entity {
struct Entity;

void reg_component(Entity*, components::Component*);
}

namespace components {

struct Component {
	virtual void bind(entity::Entity* entity) {
		// std::cout << "bind " << this << ' ' << entity << '\n';
		this->entity = entity;
		entity::reg_component(entity, this);
	}
	virtual ~Component() {}
	entity::Entity* get_entity() {
		return entity;
	}
	entity::Entity* entity = nullptr;
};

}

#include "components/entity_system.hpp"