#pragma once
#include <vector>
#include <GL/gl.h>
#include <string>
#include <map>
#include <functional>
#include "utils/file_system.hpp"

namespace shaders {


struct ShaderUniformsObject;
struct Program;

std::vector<Program*> programs;
struct Program {
	Program() {
		programs.push_back(this);
	}
	virtual std::string vertex_shader() = 0;
	virtual std::string fragment_shader() = 0;
	virtual std::string get_name() = 0;
	virtual void reg_uniforms(GLuint program_id) = 0;
};

std::map<std::string, std::vector<ShaderUniformsObject*>> shader_uniforms;
struct ShaderUniformsObject {
	ShaderUniformsObject(std::vector<Program*> programs) {
		for (auto* program : programs)
			shader_uniforms[program->get_name()].push_back(this);
	}
	virtual void reg_uniforms(GLuint program_id) { }
};





}