#pragma once
#include <vector>
#include <string>
#include "components/gpu_program.hpp"
#include "glm/glm/vec4.hpp"
#include "components/geometry_object.hpp"
#include "components/transform_object.hpp"
#include "components/layered_object.hpp"
#include "components/textured_object.hpp"
#include "components/color_object.hpp"
#include "components/component.hpp"

namespace render {

struct DrawableObject;

struct DrawableObject: public components::Component {
	DrawableObject(): components::Component() {}
	virtual ~DrawableObject() {}

	virtual geometry::GeometryObject* get_geometry() {
		entity::Entity* e = get_entity();
		return e->get<geometry::GeometryObject>();
	}
	virtual transform::TransformObject* get_transform() {
		entity::Entity* e = get_entity();
		return e->get<transform::TransformObject>();
	}
	virtual color::ColoredObject* get_color() {
		entity::Entity* e = get_entity();
		return e->get<color::ColoredObject>();
	}
	virtual layers::LayeredObject* get_layer() {
		entity::Entity* e = get_entity();
		return e->get<layers::LayeredObject>();
	}
	virtual texture::TexturedObject* get_texture() {
		entity::Entity* e = get_entity();
		return e->get<texture::TexturedObject>();
	}
	virtual shaders::ShaderUniformsObject* get_uniform() {
		entity::Entity* e = get_entity();
		return e->get<shaders::ShaderUniformsObject>();
	}
	virtual shaders::Program* get_program() = 0;
	virtual bool hide() {
		return false;
	}


	void bind(entity::Entity* entity) {
		this->entity = entity;
		get_geometry()->bind(entity);
		get_uniform()->bind(entity);
		get_texture()->bind(entity);
		get_layer()->bind(entity);
		get_transform()->bind(entity);
	}
};

struct DummyDrawable: public DrawableObject {
	DummyDrawable(shaders::Program* ptr): program(ptr), DrawableObject() {}
	shaders::Program* get_program() { return program; }
	shaders::Program* program;
	void bind(entity::Entity* entity) {
		this->entity = entity;
	}
};
}