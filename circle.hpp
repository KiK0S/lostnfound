#pragma once
#include "render_system.hpp"

namespace circle {

struct Circle : public render::Drawable {
	Circle(double x, double y, double r): render::Drawable(), x(x), y(y), r(r) {}
	
	float x, y, r;

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
			r, 0.0f, 0.0f, x,
			0.0f, r, 0.0f, y,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	};
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
};

};