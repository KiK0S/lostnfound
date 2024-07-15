#pragma once
#include "rendering/sprite.hpp"
#include "systems/definitions/gpu_program.hpp"
#include "rendering/programs.hpp"
#include <GL/gl.h>

struct Background: public sprite::Sprite, shaders::ShaderUniformsObject {
	Background(const std::string& name): sprite::Sprite(name, -1.0f, -1.0f, 1.0f, 1.0f, -1), shaders::ShaderUniformsObject({&gpu_programs::raycast_program}) {}

	void reg_uniforms(GLuint program) {
		auto backgroundTexture = glGetUniformLocation(program, "uBackground");
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, get_texture());
		glUniform1i(backgroundTexture, 2);
	}

	shaders::Program* get_program() {
		return &gpu_programs::static_object_program;
	}

};