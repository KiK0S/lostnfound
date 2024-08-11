#pragma once
#include "components/dynamic_object.hpp"
#include "components/scene_object.hpp"
#include <map>
#include <string>
#include <vector>
#include "systems/gpu_program_system.hpp"


namespace scene {
std::map<std::string, Scene*> scenes;

struct Scene: public dynamic::DynamicObject {
	Scene(std::string name): name(name), dynamic::DynamicObject(10) {
		scenes[name] = this;
		is_active = false;
	}
	std::string get_name() {
		return name;
	}
	void update() {
		if (!is_active)
			return;
		for (auto* object : scene_objects[get_name()]) {
			render::display(object->get_entity(), object->get_entity()->get<shaders::ProgramArgumentObject>()->get_program());
		}
	}
	void activate() {
		for (auto it = scenes.begin(); it != scenes.end(); ++it) {
			it->second->is_active = false;
		}
		is_active = true;
	}
	bool is_active;
	std::string name;
};

Scene main("main");

}