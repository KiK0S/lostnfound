#pragma once
#include <vector>
#include <iostream>
#include "glm/glm/vec2.hpp"
#include "systems/definitions/gpu_program.hpp"
#include "rendering/programs.hpp"
namespace camera {


struct Camera : public shaders::ShaderUniformsObject {
	Camera() : shaders::ShaderUniformsObject({&gpu_programs::raycast_program, &gpu_programs::bezier_program, &gpu_programs::visibility_program}) {}
	
	void reg_uniforms(GLuint program) {
		auto viewLocation = glGetUniformLocation(program, "uViewMatrix");
		std::vector<float> viewMatrixContainer = get_matrix();
		float* viewMatrix = viewMatrixContainer.data();
		glUniformMatrix4fv(viewLocation, 1, GL_TRUE, viewMatrix);
	}

	glm::vec2 pos{0.0f, 0.0f};

	std::vector<float> get_matrix() {
		return {
			1.0f, 0.0f, 0.0f, -pos.x,
			0.0f, 1.0f, 0.0f, -pos.y,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
};

Camera camera;

};