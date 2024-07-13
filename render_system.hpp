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
#include "camera.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "file_system.hpp"
#include "glm/glm/vec4.hpp"
#include "glm/glm/vec2.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace render {

struct Drawable;


struct Program;
std::vector<Program*> programs;
struct Program {
	Program() {
		programs.push_back(this);
	}
	virtual std::string vertex_shader() = 0;
	virtual std::string fragment_shader() = 0;
	virtual std::string get_name() = 0;
};


struct TwoShaderProgram : public Program {
	TwoShaderProgram(std::string path_vertex, std::string path_fragment): Program(), name(path_fragment) {
		vertex_source = file::read_file(file::shader(path_vertex));
		fragment_source = file::read_file(file::shader(path_fragment));
	}
	std::string vertex_shader() {
		return vertex_source;
	}
	std::string fragment_shader() {
		return fragment_source;
	}
	std::string get_name() {
		return name;
	}
	std::string name;
	std::string vertex_source;
	std::string fragment_source;
};

TwoShaderProgram texture("texture_2d_v.glsl", "texture_2d_f.glsl");
TwoShaderProgram texture_framebuffer("texture_framebuffer_2d_v.glsl", "texture_2d_f.glsl");
TwoShaderProgram raycast("raycast_2d_v.glsl", "raycast_2d_f.glsl");
TwoShaderProgram bezier("bezier_2d_v.glsl", "bezier_2d_f.glsl");

struct cmp {
	bool operator()(Drawable* a, Drawable* b) const;
};
std::vector<Drawable*> drawables;

struct Drawable {
	Drawable() {
		drawables.push_back(this);
	}
	virtual ~Drawable() {}
	virtual std::vector<float> get_pos() = 0;
	virtual std::vector<float> get_uv() = 0;
	virtual std::vector<float> get_model_matrix() = 0;
	virtual int get_layer() const = 0;
	virtual bool show() { return true; }
	virtual Program* get_program() { return &raycast; }
	virtual GLuint get_texture() = 0;
	virtual std::string get_name() const = 0;
	virtual void reg_uniforms(GLuint id) {}
	virtual glm::vec4 get_color() { return glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};	}
};

bool cmp::operator()(Drawable* a, Drawable* b) const {
	return a->get_layer() < b->get_layer();
}


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
	for (Program* program_ptr : programs) {
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
		program_ids[program_ptr->get_name()] = program;
	}	
	// During init, enable debug output
	#ifndef __EMSCRIPTEN__
	glEnable              ( GL_DEBUG_OUTPUT );
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

void add_to_frame(Drawable* object) {
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

RenderTarget create_render_target() {
	GLuint framebuffer = 0;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	GLuint output_texture;
	glGenTextures(1, &output_texture);

	glBindTexture(GL_TEXTURE_2D, output_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 960, 480, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

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

GLuint transparancy_texture;
GLuint background_texture;
std::vector<float> raycast_start;

void bind_render_target(RenderTarget* target) {
	if (target == nullptr) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, target->frame_buffer);
}


void display(Drawable* object, Program* program_ptr) {
	GLuint program = program_ids[program_ptr->get_name()];
	glUseProgram(program);
	glBindVertexArray(vao_data[object->get_name()]);

	auto viewLocation = glGetUniformLocation(program, "uViewMatrix");
	auto projectionLocation = glGetUniformLocation(program, "uProjectionMatrix");
	auto modelLocation = glGetUniformLocation(program, "uModelMatrix");
	auto colorLocation = glGetUniformLocation(program, "uColor");

	std::vector<float> viewMatrixContainer = camera::get_matrix(object->get_layer());
	float* viewMatrix = viewMatrixContainer.data();
	
	std::vector<float> modelMatrixContainer = object->get_model_matrix();
	float* modelMatrix = modelMatrixContainer.data();

	float projectMatrix[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	glm::vec4 color = object->get_color();
	glUniformMatrix4fv(viewLocation, 1, GL_TRUE, viewMatrix);
	glUniformMatrix4fv(projectionLocation, 1, GL_TRUE, projectMatrix);
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, modelMatrix);
	glUniform4fv(colorLocation, 1, glm::value_ptr(color));
	auto textureLocation = glGetUniformLocation(program, "uTexture");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, object->get_texture());
	glUniform1i(textureLocation, 0);

	if (program_ptr->get_name() == "raycast_2d_f.glsl") {
		auto playerLocation = glGetUniformLocation(program, "uStartPosition");
		glUniform2fv(playerLocation, 1, raycast_start.data());

		auto transparancyTexture = glGetUniformLocation(program, "uTransparency");
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, transparancy_texture);
		glUniform1i(transparancyTexture, 1);
		auto backgroundTexture = glGetUniformLocation(program, "uBackground");
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, background_texture);
		glUniform1i(backgroundTexture, 2);
	} else if (program_ptr->get_name() == "texture_2d_f.glsl") {
		// viewMatrixContainer[5] *= -1;
		// viewMatrixContainer[7] *= -1;
		glUniformMatrix4fv(viewLocation, 1, GL_TRUE, viewMatrix);
	}
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