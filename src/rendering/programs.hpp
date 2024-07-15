#pragma once
#include "systems/definitions/gpu_program.hpp"
#include <functional>


namespace gpu_programs {

struct TwoShaderProgram : public shaders::Program {
	TwoShaderProgram(std::string path_vertex, std::string path_fragment, std::function<void(GLuint)> reg_uniforms): Program(), name(path_fragment + ":" + path_vertex), reg_uniforms_impl(reg_uniforms) {
		vertex_source = file::read_file(file::shader(path_vertex));
		fragment_source = file::read_file(file::shader(path_fragment));
	}
	TwoShaderProgram(std::string path_vertex, std::string path_fragment): TwoShaderProgram(path_vertex, path_fragment, [](GLuint){}) {}
	std::string vertex_shader() {
		return vertex_source;
	}
	std::string fragment_shader() {
		return fragment_source;
	}
	std::string get_name() {
		return name;
	}
	void reg_uniforms(GLuint program_id) {
		for (auto* object : shaders::shader_uniforms[get_name()]) {
			object->reg_uniforms(program_id);
		}
		reg_uniforms_impl(program_id);
	}
	std::string name;
	std::string vertex_source;
	std::string fragment_source;
	std::function<void(GLuint)> reg_uniforms_impl;
};


TwoShaderProgram static_object_program("texture_2d_v.glsl", "texture_2d_f.glsl");
TwoShaderProgram visibility_program("texture_framebuffer_2d_v.glsl", "texture_2d_f.glsl");
TwoShaderProgram raycast_program("raycast_2d_v.glsl", "raycast_2d_f.glsl");
TwoShaderProgram bezier_program("bezier_2d_v.glsl", "bezier_2d_f.glsl");


}