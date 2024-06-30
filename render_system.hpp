#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace render {

struct Drawable;

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
	virtual GLuint get_texture() = 0;
	virtual std::string get_name() const = 0;
};

bool cmp::operator()(Drawable* a, Drawable* b) const {
	return a->get_layer() < b->get_layer();
}


const std::string VERTEX_SHADER_SOURCE = R"(#version 300 es
precision mediump float;
layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aTexturePosition;

uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uProjectionMatrix;

out vec2 vTextureCoordinate;

void main() {
	gl_Position = uModelMatrix * vec4(aVertexPosition, 0, 1);
	vTextureCoordinate = aTexturePosition;
})";

const std::string FRAGMENT_SHADER_SOURCE = R"(#version 300 es
precision mediump float;

uniform sampler2D uTexture;
uniform vec4 uColor;

in vec2 vTextureCoordinate;

out vec4 outColor;
void main() {
	outColor = texture(uTexture, vTextureCoordinate);
	outColor = outColor * uColor;
})";

GLuint program;
GLuint vertexShader;
GLuint fragmentShader;
SDL_Window* _window;
GLuint load_shader(GLenum shader_type);


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

	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewStatus) << std::endl;
		exit(1);
	}
	vertexShader = load_shader(GL_VERTEX_SHADER);
	fragmentShader = load_shader(GL_FRAGMENT_SHADER);
	std::cout << "load_program loading" << std::endl;
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	GLint linkSuccess = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
	if (linkSuccess == GL_FALSE) {
			// fail to compile program
			glDeleteProgram(program);
			throw std::exception();
	}
	std::cout << "load_program loaded" << std::endl;
	// During init, enable debug output
	#ifndef __EMSCRIPTEN__
	glEnable              ( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( MessageCallback, 0 );
	#endif
}

// GL_VERTEX_SHADER
// GL_FRAGMENT_SHADER
GLuint load_shader(GLenum shader_type) {
	std::cout << "load_shader " << shader_type << " loading" << std::endl;
	GLuint glShader = glCreateShader(shader_type);
	std::string source = (shader_type == GL_VERTEX_SHADER ? VERTEX_SHADER_SOURCE : FRAGMENT_SHADER_SOURCE);
	GLchar const *files[] = {source.c_str()};
	GLint lengths[] = {GLint(source.size())};
	glShaderSource(glShader, 1, files, lengths);
	glCompileShader(glShader);
	GLint compileSuccess = 0;
	glGetShaderiv(glShader, GL_COMPILE_STATUS, &compileSuccess);
	if (compileSuccess == GL_FALSE) {
			// fail to compile shader!
			glDeleteShader(glShader);
			throw std::exception();
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.3, 0.3, 0.3, 1);

	// Enable depth testing (objects can appear behind/infront of eachother)
	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LEQUAL);

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

std::map<std::string, GLuint> vao_data;

void add_to_frame(Drawable* object) {
	
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

void display(Drawable* object) {
	if (!object->show())
		return;
	glUseProgram(program);
	glBindVertexArray(vao_data[object->get_name()]);

	auto viewLocation = glGetUniformLocation(program, "uViewMatrix");
	auto projectionLocation = glGetUniformLocation(program, "uProjectionMatrix");
	auto modelLocation = glGetUniformLocation(program, "uModelMatrix");
	auto colorLocation = glGetUniformLocation(program, "uColor");

	float viewMatrix[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	
	std::vector<float> modelMatrixContainer = object->get_model_matrix();
	float* modelMatrix = modelMatrixContainer.data();

	float projectMatrix[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	float color[4] = {1.0, 1.0, 1.0, 1.0};
	glUniformMatrix4fv(viewLocation, 1, GL_TRUE, viewMatrix);
	glUniformMatrix4fv(projectionLocation, 1, GL_TRUE, projectMatrix);
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, modelMatrix);
	glUniform4fv(colorLocation, 1, color);
	auto textureLocation = glGetUniformLocation(program, "uTexture");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, object->get_texture());
	glUniform1i(textureLocation, 0);

	glDrawArrays(GL_TRIANGLES, 0, object->get_pos().size());
}

void update() {
	for (auto* drawable : drawables) {
		render::display(drawable);
	}
}


void close_frame() {
  SDL_GL_SwapWindow(_window);
}

}