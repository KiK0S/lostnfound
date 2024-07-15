#pragma once
#include <vector>
#include <string>
#include "systems/definitions/gpu_program.hpp"
#include "glm/glm/vec4.hpp"

namespace render {

struct DrawableObject;

struct cmp {
	bool operator()(DrawableObject* a, DrawableObject* b) const;
};

std::vector<DrawableObject*> drawables;

struct DrawableObject {
	DrawableObject() {
		drawables.push_back(this);
	}
	virtual ~DrawableObject() {}
	virtual std::vector<float> get_pos() = 0;
	virtual std::vector<float> get_uv() = 0;
	virtual std::vector<float> get_model_matrix() = 0;
	virtual int get_layer() const = 0;
	virtual bool show() { return true; }
	virtual shaders::Program* get_program() = 0;
	virtual GLuint get_texture() = 0;
	virtual std::string get_name() const = 0;
	virtual void reg_uniforms(GLuint id) {
		get_program()->reg_uniforms(id);
	}
	virtual glm::vec4 get_color() { return glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};	}
};

bool cmp::operator()(DrawableObject* a, DrawableObject* b) const {
	return a->get_layer() < b->get_layer();
}
}