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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "utils/file_system.hpp"
#include "systems/definitions/drawable_object.hpp"
#include "glm/glm/vec4.hpp"
#include "glm/glm/vec2.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace render {

struct RenderTarget {
	GLuint frame_buffer;
	GLuint output_texture;
};

std::map<std::string, GLuint> program_ids;
SDL_Window* _window;
GLuint load_shader(std::string source, GLenum shader_type);




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

char errorLog[1024];

void init(SDL_Window *window) {
	_window = window;
	std::cout << "SDL_GL_CreateContext" << std::endl;

	auto *context = SDL_GL_CreateContext(window);
	std::cout << "glewInit" << std::endl;

	const unsigned char* version = glGetString(GL_VERSION);
	std::cout << "opengl version " << version << '\n';
	
	glEnable(GL_CULL_FACE);

	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewStatus) << std::endl;
		throw std::runtime_error("Failed to initialize GLEW");

	}
	for (shaders::Program* program_ptr : shaders::programs) {
		std::cout << "load_program loading" << std::endl;
		GLuint vertexShader = load_shader(program_ptr->vertex_shader(), GL_VERTEX_SHADER);
		GLuint fragmentShader = load_shader(program_ptr->fragment_shader(), GL_FRAGMENT_SHADER);
		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		GLint linkSuccess = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
		if (linkSuccess == GL_FALSE) {
				// fail to compile program
				glDeleteProgram(program);
				throw std::runtime_error("cant create program");
		}

		// GLint is_valid = 0;
		// glGetProgramiv(program, GL_VALIDATE_STATUS, &is_valid);
		// if (is_valid == GL_FALSE) {
		// 	GLint maxLength = 0;
		// 	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// 	glGetProgramInfoLog(program, maxLength, &maxLength, errorLog);

		// 	for (int i = 0; i < maxLength; i++) {
		// 		std::cerr << errorLog[i];
		// 	}
		// 	std::cerr << '\n';
		// 	std::cerr << "Program validation failed: " << std::endl;
		// 	glDeleteProgram(program);
		// 	throw std::runtime_error("cant create program");
		// }
		program_ids[program_ptr->get_name()] = program;
	}	
	// During init, enable debug output
	#ifndef __EMSCRIPTEN__
	glEnable              ( GL_DEBUG_OUTPUT );
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback( MessageCallback, 0 );
	#endif
}

// GL_VERTEX_SHADER
// GL_FRAGMENT_SHADER
GLuint load_shader(std::string source, GLenum shader_type) {
	std::cout << "load_shader " << shader_type << " loading" << std::endl;
	GLuint glShader = glCreateShader(shader_type);
	GLchar const *files[] = {source.c_str()};
	GLint lengths[] = {GLint(source.size())};
	glShaderSource(glShader, 1, files, lengths);
	glCompileShader(glShader);
	GLint compileSuccess = 0;
	glGetShaderiv(glShader, GL_COMPILE_STATUS, &compileSuccess);
	if (compileSuccess == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(glShader, GL_INFO_LOG_LENGTH, &maxLength);

			glGetShaderInfoLog(glShader, maxLength, &maxLength, errorLog);
			for (int i = 0; i < maxLength; i++) {
				std::cerr << errorLog[i];
			}
			std::cerr << '\n';
			std::cerr << "Compiling shader failed: " << std::endl;

			// fail to compile shader!
			glDeleteShader(glShader);
			throw std::runtime_error("can't compile shader");
	}
	std::cout << "load_shader " << shader_type << " loaded" << std::endl;
	return glShader;
}

GLuint create_texture(const int width, const int height, const void *data, GLuint filter = GL_LINEAR) {
		GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}
std::map<std::string, GLuint> texture_data;

GLuint get_texture(std::string path) {
	if (texture_data.find(path) != texture_data.end()) {
		return texture_data[path];
	}
	GLuint res;
	if (path.empty()) {
		unsigned char data[4] = {255, 255, 255, 255};
		res = create_texture(1, 1, data);
	} else {
		int width, height, nrChannels;
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
		if (data == nullptr) {
			std::cout << stbi_failure_reason() << std::endl;
			throw std::runtime_error(stbi_failure_reason());
		} else {
			res = create_texture(width, height, data);
			free(data);
		}
	}
	texture_data[path] = res;
	return res;
}

void start_frame() {
	glClearColor(0.3, 0.3, 0.3, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Enable depth testing (objects can appear behind/infront of eachother)
	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LEQUAL);

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

std::map<std::string, GLuint> vao_data;

void add_to_frame(DrawableObject* object) {
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
	std::vector<float> pos = object->get_pos();
	std::vector<float> uv = object->get_uv();

	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * pos.size(),
							pos.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);

	// Texture data
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * uv.size(), uv.data(),
								GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	vao_data[object->get_name()] = vao;
}


void init() {
	for (auto* drawable : drawables) {
		add_to_frame(drawable);
	}
	sort(drawables.begin(), drawables.end(), cmp());
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
	GLuint program = program_ids[program_ptr->get_name()];
	glUseProgram(program);
	glBindVertexArray(vao_data[object->get_name()]);

	auto projectionLocation = glGetUniformLocation(program, "uProjectionMatrix");
	auto modelLocation = glGetUniformLocation(program, "uModelMatrix");
	auto colorLocation = glGetUniformLocation(program, "uColor");

	std::vector<float> modelMatrixContainer = object->get_model_matrix();
	float* modelMatrix = modelMatrixContainer.data();

	float projectMatrix[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	glm::vec4 color = object->get_color();
	glUniformMatrix4fv(projectionLocation, 1, GL_TRUE, projectMatrix);
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, modelMatrix);
	glUniform4fv(colorLocation, 1, glm::value_ptr(color));
	auto textureLocation = glGetUniformLocation(program, "uTexture");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, object->get_texture());
	glUniform1i(textureLocation, 0);

	object->reg_uniforms(program);
	

	glDrawArrays(GL_TRIANGLES, 0, object->get_pos().size());
	glUseProgram(0);
}

void update() {
	for (auto* drawable : drawables) {
		if (!drawable->show())
			continue;
		render::display(drawable, drawable->get_program());
	}
}


void close_frame() {
  SDL_GL_SwapWindow(_window);
}

}