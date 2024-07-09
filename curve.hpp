#pragma once
#include "render_system.hpp"

namespace curve {


struct Curve : public render::Drawable {
	Curve(): render::Drawable() {}
	

	std::vector<float> get_pos() {
		int n = 10;
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
			result.push_back(t * 2 - 1.0f);   // x position (mapped to t)
			result.push_back(1.0f);   // x position (mapped to t)
			result.push_back((t + 1.0f / float(n-1)) * 2 - 1.0f);   // x position (mapped to t)
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
};

}