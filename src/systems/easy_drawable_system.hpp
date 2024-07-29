#pragma once
#include "components/drawable_object.hpp"
#include "systems/texture_system.hpp"
#include "systems/color_system.hpp"

namespace render {


struct ModelMatrix : public shaders::ShaderUniformsObject {
	ModelMatrix(): shaders::ShaderUniformsObject({}) {}
	~ModelMatrix(){}

	void reg_uniforms(GLuint program_id) {
		entity::Entity* e = get_entity();
		transform::TransformObject* t = e->get<transform::TransformObject>();
		auto modelLocation = glGetUniformLocation(program_id, "uModelMatrix");
		glm::mat4 modelMatrix = t->get_model_matrix();
		glUniformMatrix4fv(modelLocation, 1, GL_TRUE, glm::value_ptr(modelMatrix));
	}

};

struct CombinedUniforms: public shaders::ShaderUniformsObject {
CombinedUniforms(std::vector<shaders::ShaderUniformsObject*> uniforms): uniforms(std::move(uniforms)), shaders::ShaderUniformsObject({}) {}

	void reg_uniforms(GLuint program_id) {
		for (const auto& obj : uniforms) {
			obj->reg_uniforms(program_id);
		}
	}
	void bind(entity::Entity* e) {
		for (const auto& obj : uniforms) {
			obj->bind(e);
		}
	}
	void add(shaders::ShaderUniformsObject* x) {
		uniforms.push_back(x);
	}
	std::vector<shaders::ShaderUniformsObject*> uniforms;
};	


struct SolidDrawable: public render::DrawableObject {
	SolidDrawable(geometry::GeometryObject* geometry, shaders::Program* program): program(program), geometry(geometry), layer(1),
		uniforms({&model_matrix}) {}
	SolidDrawable(geometry::GeometryObject* geometry): SolidDrawable(geometry, &shaders::raycast_program) {}
	virtual ~SolidDrawable() {}

	geometry::GeometryObject* geometry;
	shaders::Program* program;
	virtual geometry::GeometryObject* get_geometry() {
		return geometry;
	}
	virtual transform::TransformObject* get_transform() {
		return &transform;
	}
	virtual layers::LayeredObject* get_layer() {
		return &layer;
	}
	virtual texture::TexturedObject* get_texture() {
		return &texture::no_texture;
	}
	virtual shaders::ShaderUniformsObject* get_uniform() {
		return &uniforms;
	}
	virtual shaders::Program* get_program() {
		return program;
	}
	virtual color::ColoredObject* get_color() {
		return color;
	}

	virtual void bind(entity::Entity* entity) {
		get_geometry()->bind(entity);
		get_transform()->bind(entity);
		get_layer()->bind(entity);
		get_uniform()->bind(entity);
	}
	transform::NoRotationTransform transform;
	layers::ConstLayer layer;
	ModelMatrix model_matrix;
	CombinedUniforms uniforms;
	color::ColoredObject* color = &color::no_color;
};
}