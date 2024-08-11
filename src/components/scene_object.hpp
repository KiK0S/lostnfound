#pragma once
#include <string>
#include <map>
#include <vector>
#include "components/dynamic_object.hpp"
#include "components/init_object.hpp"
#include "components/component.hpp"
#include "components/layered_object.hpp"

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
};


void init() {
	for (auto& it : scene_objects) {
		auto& v = it.second;
		std::sort(v.begin(), v.end(), cmp());
	}
}
init::CallbackOnStart init_scenes(&init, 2);

bool cmp::operator()(SceneObject* a, SceneObject* b) const {
	return a->get_entity()->get<layers::LayeredObject>()->get_layer() < b->get_entity()->get<layers::LayeredObject>()->get_layer();
}

}