#pragma once
#include "components/geometry_object.hpp"
#include "components/gpu_program.hpp"
#include <memory>
#include <vector>

namespace geometry {


struct BezierCurve : public geometry::GeometryObject, shaders::ShaderUniformsObject {
	BezierCurve(std::string name, std::array<glm::vec2, 4> points): name(name), points(points), geometry::GeometryObject(), shaders::ShaderUniformsObject({}) {}
	
	int n = 30;
	std::vector<glm::vec2> get_pos() {
		std::vector<glm::vec2> result;
		for (int i = 0; i + 1 < n; ++i) {
			float t = i / float(n- 1);
			result.push_back(glm::vec2{t * 2 - 1.0f, -1.0f});   // x position (mapped to t)
			result.push_back(glm::vec2{(t + 1.0f / float(n-1)) * 2 - 1.0f, -1.0f});   // x position (mapped to t)
			result.push_back(glm::vec2{t * 2 - 1.0f, 1.0f});   // x position (mapped to t)

			result.push_back(glm::vec2{(t + 1.0f / float(n-1)) * 2 - 1.0f, -1.0f});   // x position (mapped to t)
			result.push_back(glm::vec2{(t + 1.0f / float(n-1)) * 2 - 1.0f, 1.0f});   // x position (mapped to t)
			result.push_back(glm::vec2{t * 2 - 1.0f, 1.0f});   // x position (mapped to t)
			
		}
		return result;
	}

	std::vector<glm::vec2> get_uv() {
		std::vector<glm::vec2> result;
		for (int i = 0; i + 1 < n; ++i) {
			for (int j = 0; j < 6; j++)
				result.push_back(glm::vec2{0.0f, 0.0f});
		}
		return result;
	}
	int get_size() {
		// (n - 1) * 6
		return (n - 1) * 6;
	}
	std::string name;
	std::string get_name() const {
		return name;
	}
	void reg_uniforms(GLuint program) {
		float* data = reinterpret_cast<GLfloat *>(points.data());
		glUniform2fv(glGetUniformLocation(program, "controlPoints"), 4, reinterpret_cast<GLfloat *>(points.data()));
	}
	std::array<glm::vec2, 4> points;
};

}