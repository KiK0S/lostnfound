#pragma once
#include "render_system.hpp"
#include <memory>

namespace curve {


struct BezierCurve : public render::Drawable {
	BezierCurve(std::array<float, 8> points): render::Drawable(), points(points) {}
	

	std::vector<float> get_pos() {
		int n = 30;
		std::vector<float> result;
		for (int i = 0; i + 1 < n; ++i) {
			float t = i / float(n- 1);
			result.push_back(t * 2 - 1.0f);   // x position (mapped to t)
			result.push_back(-1.0f);   // x position (mapped to t)
			result.push_back((t + 1.0f / float(n-1)) * 2 - 1.0f);   // x position (mapped to t)
			result.push_back(-1.0f);   // x position (mapped to t)
			result.push_back(t * 2 - 1.0f);   // x position (mapped to t)
			result.push_back(1.0f);   // x position (mapped to t)
			
			result.push_back((t + 1.0f / float(n-1)) * 2 - 1.0f);   // x position (mapped to t)
			result.push_back(-1.0f);   // x position (mapped to t)
			result.push_back((t + 1.0f / float(n-1)) * 2 - 1.0f);   // x position (mapped to t)
			result.push_back(1.0f);   // x position (mapped to t)
			result.push_back(t * 2 - 1.0f);   // x position (mapped to t)
			result.push_back(1.0f);   // x position (mapped to t)
			
		}
		return result;
	}

	std::vector<float> get_uv() {
		int n = 10;
		std::vector<float> result;
		for (int i = 0; i + 1 < n; ++i) {
			for (int j = 0; j < 12; j++)
				result.push_back(0.0f);
		}
		return result;
	};
	std::vector<float> get_model_matrix() {
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	};
	bool show() {
		return true;
	}
	GLuint get_texture() {
		return render::get_texture("");
	};
	int get_layer() const {
		return 2;
	}
	std::string get_name() const {
		return "curve";
	}
	render::Program* get_program() {
		return &render::bezier;
	}
	void reg_uniforms(GLuint program) {
		Drawable::reg_uniforms(program);
		glUniform2fv(glGetUniformLocation(program, "controlPoints"), 4, points.data());
	}
	std::array<float, 8> points;
};

struct Curve {
	std::vector<std::unique_ptr<BezierCurve>> curves;
	Curve(std::vector<float> points) {
		float prev_x = points.size() > 2 ? points[0] : 0;
		float prev_y = points.size() > 2 ? points[1] : 0;

		for (int i = 2; i + 6 <= points.size(); i += 4) {
			float next_x = i + 10 <= points.size() ? (points[i + 2] + points[i + 4]) / 2 : points[i + 4];
			float next_y = i + 10 <= points.size() ? (points[i + 3] + points[i + 5]) / 2 : points[i + 5];
			curves.push_back(std::make_unique<BezierCurve>(std::array{prev_x, prev_y, points[i], points[i + 1],
													 points[i + 2], points[i + 3], next_x, next_y}));
			prev_x = next_x;
			prev_y = next_y;
		}
	}
};

}