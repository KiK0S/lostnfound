#pragma once
#include <vector>
#include "glm/glm/vec2.hpp"
#include "glm/glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "components/component.hpp"

namespace transform {

struct TransformObject;

std::vector<TransformObject*> transforms;

struct TransformObject: public components::Component {
	TransformObject(): components::Component() {
		transforms.push_back(this);
	}
	virtual ~TransformObject() {}

	virtual glm::vec2 get_pos() {
		return glm::vec2(get_model_matrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	virtual void translate(glm::vec2) = 0;
	virtual void scale(glm::vec2) = 0;
	virtual glm::mat4 get_model_matrix() = 0;
	virtual void rotate(float angle) = 0;
};


struct NoRotationTransform: public transform::TransformObject {
	NoRotationTransform(): transform::TransformObject(), pos(0.0f, 0.0f), scale_(1.0f, 1.0f) {}
	NoRotationTransform(glm::vec2 top_left, glm::vec2 bottom_right): NoRotationTransform() {
		scale((bottom_right - top_left) / 2.0f);
		translate(top_left + scale_);
	}
	virtual ~NoRotationTransform() {}

	glm::vec2 pos;
	glm::vec2 scale_;
	virtual glm::vec2 get_pos() {
		return pos;
	}
	virtual void translate(glm::vec2 x) {
		pos += x;
	}
	virtual void scale(glm::vec2 x) {	
		scale_ *= x;
	}
	virtual glm::mat4 get_model_matrix() {
		return {
			scale_.x, 0.0f, 0.0f, pos.x,
			0.0f, scale_.y, 0.0f, pos.y,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
	virtual void rotate(float angle) {}
};


struct Transform2d: public TransformObject {
	Transform2d(): TransformObject() {}
	Transform2d(glm::vec2 top_left, glm::vec2 bottom_right): Transform2d() {
		scale(bottom_right - top_left);
		translate(top_left);
	}
	glm::mat4 get_model_matrix() {
		return mat;
	}
	void translate(glm::vec2 v) {
		mat = glm::translate(mat, glm::vec3(v, 0.0f));
	}
	void scale(glm::vec2 v) {
		mat = glm::scale(mat, glm::vec3(v, 1.0f));
	}
	void rotate(float angle) {
		mat = glm::rotate(mat, angle, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	glm::mat4 mat;
};
}