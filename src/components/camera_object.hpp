#pragma once
#include <vector>
#include "components/transform_object.hpp"
#include "components/dynamic_object.hpp"
#include "components/gpu_program.hpp"
#include "components/component.hpp"


namespace camera {

struct CameraObject;
std::vector<CameraObject*> cameras;

struct CameraObject: public components::Component {
	CameraObject(): components::Component() {
		cameras.push_back(this);
	}
	virtual ~CameraObject() {}

	virtual transform::TransformObject* get_transform() {
		entity::Entity* e = get_entity();
		return e->get<transform::TransformObject>();
	}
	virtual dynamic::DynamicObject* get_dynamic() {
		entity::Entity* e = get_entity();
		return e->get<dynamic::DynamicObject>();
	}
	virtual shaders::ShaderUniformsObject* get_uniform() {
		entity::Entity* e = get_entity();
		return e->get<shaders::ShaderUniformsObject>();
	}
};


}