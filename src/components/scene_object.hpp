#pragma once
#include <string>
#include <map>
#include <vector>
#include "components/drawable_object.hpp"
#include "components/dynamic_object.hpp"
#include "components/init_object.hpp"
#include "components/component.hpp"

namespace scene {

struct Scene;
struct SceneObject;

struct cmp {
	bool operator()(SceneObject* a, SceneObject* b) const;
};
std::map<std::string, std::vector<SceneObject*>> scene_objects;

struct SceneObject : public components::Component {
	SceneObject(std::string scene_name): components::Component() {
		scene_objects[scene_name].push_back(this);

	}
	virtual ~SceneObject() {}
	virtual render::DrawableObject* get_drawable() {
		entity::Entity* e = get_entity();
		return e->get<render::DrawableObject>();
	}
};


void init() {
	for (auto& it : scene_objects) {
		auto& v = it.second;
		std::sort(v.begin(), v.end(), cmp());
	}
}
init::CallbackOnStart init_scenes(&init, 2);

bool cmp::operator()(SceneObject* a, SceneObject* b) const {
	return a->get_drawable()->get_layer()->get_layer() < b->get_drawable()->get_layer()->get_layer();
}

}