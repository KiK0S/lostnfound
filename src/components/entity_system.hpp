#pragma once
#include "components/component.hpp"
#include "components/all_components.hpp"
#include <string>

namespace entity {

std::vector<Entity*> entities;

struct Entity {

	Entity(){
		entities.push_back(this);
	}
	~Entity(){}

	Entity& add(components::Component* comp) {
		pre_bind_components.push_back(comp);
		return *this;
	}

	Entity& bind() {	
		for (components::Component* c : pre_bind_components) {
			c->bind(this);
		}
		return *this;
	}

	template <typename T>
	T* get() {
		for (auto x : components) {
			T* casted_ptr = dynamic_cast<T*>(x);
			if (casted_ptr != nullptr)
				return casted_ptr;
		}
		for (auto x : pre_bind_components) {
			T* casted_ptr = dynamic_cast<T*>(x);
			if (casted_ptr != nullptr)
				return casted_ptr;
		}
		return nullptr;
	}

	std::vector<components::Component*> pre_bind_components;
	std::vector<components::Component*> components;
};

void reg_component(Entity* e, components::Component* c) {
	e->components.push_back(c);
}


}
