#pragma once
#include "systems/definitions/drawable_object.hpp"
#include "glm/glm/vec2.hpp"

namespace circle {

struct Circle : public render::DrawableObject {
	Circle(glm::vec2 pos, double r, glm::vec4 color): render::DrawableObject(), pos(pos), r(r), color(color) {}
	Circle(glm::vec2 pos, double r): Circle(pos, r, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}) {}
	
	glm::vec2 pos;
	float r;

	std::vector<float> get_pos() {
		return {
		0.000000f, 0.000000f,
		0.923880f, 0.382683f,
		1.000000f, 0.000000f,

		0.000000f, 0.000000f,
		0.707107f, 0.707107f,
		0.923880f, 0.382683f,

		0.000000f, 0.000000f,
		0.382683f, 0.923880f,
		0.707107f, 0.707107f,

		0.000000f, 0.000000f,
		0.000000f, 1.000000f,
		0.382683f, 0.923880f,

		0.000000f, 0.000000f,
		-0.382683f, 0.923880f,
		0.000000f, 1.000000f,

		0.000000f, 0.000000f,
		-0.707107f, 0.707107f,
		-0.382683f, 0.923880f,

		0.000000f, 0.000000f,
		-0.923880f, 0.382683f,
		-0.707107f, 0.707107f,

		0.000000f, 0.000000f,
		-1.000000f, 0.000000f,
		-0.923880f, 0.382683f,

		0.000000f, 0.000000f,
		-0.923880f, -0.382683f,
		-1.000000f, 0.000000f,

		0.000000f, 0.000000f,
		-0.707107f, -0.707107f,
		-0.923880f, -0.382683f,

		0.000000f, 0.000000f,
		-0.382683f, -0.923880f,
		-0.707107f, -0.707107f,

		0.000000f, 0.000000f,
		0.000000f, -1.000000f,
		-0.382683f, -0.923880f,

		0.000000f, 0.000000f,
		0.382683f, -0.923880f,
		0.000000f, -1.000000f,

		0.000000f, 0.000000f,
		0.707107f, -0.707107f,
		0.382683f, -0.923880f,

		0.000000f, 0.000000f,
		0.923880f, -0.382683f,
		0.707107f, -0.707107f,

		0.000000f, 0.000000f,
		1.000000f, 0.000000f,
		0.923880f, -0.382683f
		};
	}

	std::vector<float> get_uv() {
		return {
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f};
	};
	std::vector<float> get_model_matrix() {
		return {
			r, 0.0f, 0.0f, pos.x,
			0.0f, r, 0.0f, pos.y,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	};
	glm::vec4 get_color() {
		return color;
	}
	bool show() {
		return false;
	}
	GLuint get_texture() {
		return render::get_texture("");
	};
	int get_layer() const {
		return 2;
	}
	std::string get_name() const {
		return "circle";
	}
	glm::vec4 color;
};

};