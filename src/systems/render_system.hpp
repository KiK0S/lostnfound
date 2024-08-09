#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "components/drawable_object.hpp"
#include "components/dynamic_object.hpp"
#include "glm/glm/vec4.hpp"
#include "glm/glm/vec2.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "systems/gpu_program_system.hpp"
#include "systems/texture_system.hpp"


namespace render {

struct RenderTarget {
	GLuint frame_buffer;
	GLuint output_texture;
};

SDL_Window* _window;




void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

void init(SDL_Window *window) {
	_window = window;
	std::cout << "SDL_GL_CreateContext" << std::endl;

	auto *context = SDL_GL_CreateContext(window);
	std::cout << "glewInit" << std::endl;

	const unsigned char* version = glGetString(GL_VERSION);
	std::cout << "opengl version " << version << '\n';
	
	// todo: fix quad, polygon
	// glEnable(GL_CULL_FACE);

	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewStatus) << std::endl;
		throw std::runtime_error("Failed to initialize GLEW");

	}
	
	shaders::init();

	// During init, enable debug output
	#ifndef __EMSCRIPTEN__
	glEnable              ( GL_DEBUG_OUTPUT );
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback( MessageCallback, 0 );
	#endif
}
struct FrameStartSystem: public dynamic::DynamicObject {
	FrameStartSystem(): dynamic::DynamicObject(-1) {}
	void update() {
		glClearColor(0.5, 0.3, 0.3, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Enable depth testing (objects can appear behind/infront of eachother)
		// glEnable(GL_DEPTH_TEST);
		// glDepthFunc(GL_LEQUAL);

		// Enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
};

struct FrameEndSystem: public dynamic::DynamicObject {
	FrameEndSystem(): dynamic::DynamicObject(1000) {}
	void update() {
		SDL_GL_SwapWindow(_window);
	}
};

FrameStartSystem start_system;
FrameEndSystem end_system;

std::map<std::string, GLuint> vao_data;

void add_to_frame(geometry::GeometryObject* object) {
	if (vao_data.find(object->get_name()) != vao_data.end()) {
		return;
	}
	GLuint buffers[2];
	glGenBuffers(2, buffers);

	auto positionBuffer = buffers[0];
	auto uvBuffer = buffers[1];
	

	GLuint vaos[1];
	glGenVertexArrays(1, vaos);

	auto vao = vaos[0];

	glBindVertexArray(vao);
	// Position data
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 2 * object->get_size(),
							object->get_pos().data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);

	// Texture data
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 2 * object->get_size(), object->get_uv().data(),
								GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	vao_data[object->get_name()] = vao;
}


void init() {
	for (auto* geometry : geometry::geometry_objects) {
		add_to_frame(geometry);
	}
}

RenderTarget create_render_target(GLenum internalFormat, GLenum format) {
	GLuint framebuffer = 0;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	GLuint output_texture;
	glGenTextures(1, &output_texture);

	glBindTexture(GL_TEXTURE_2D, output_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, 960, 960, 0, format, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, output_texture, 0);

	GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, draw_buffers);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "BROKEN framebuffer\n";
		throw std::runtime_error("broken framebuffer");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
	return RenderTarget{framebuffer, output_texture};
}


void bind_render_target(RenderTarget* target) {
	if (target == nullptr) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, target->frame_buffer);
}


void display(DrawableObject* object, shaders::Program* program_ptr) {
	shaders::run_with_program(program_ptr, [&](GLuint program){
		glBindVertexArray(vao_data[object->get_geometry()->get_name()]);

		auto textureLocation = glGetUniformLocation(program, "uTexture");
		glActiveTexture(GL_TEXTURE0);
		auto texture_comp = object->get_texture();
		if (texture_comp == nullptr) {
			texture_comp = &texture::no_texture;
		}
		glBindTexture(GL_TEXTURE_2D, texture_comp->get_texture());
		glUniform1i(textureLocation, 0);

		auto colorLocation = glGetUniformLocation(program, "uColor");
		glUniform4fv(colorLocation, 1, glm::value_ptr(object->get_color()->get_color()));

		program_ptr->reg_uniforms(program);
		auto uniforms_comp = object->get_uniform();
		if (uniforms_comp != nullptr) {
			uniforms_comp->reg_uniforms(program);
		}

		glDrawArrays(GL_TRIANGLES, 0, object->get_geometry()->get_size());
	});

}


}