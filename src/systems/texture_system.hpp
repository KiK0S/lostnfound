#pragma once
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glew.h>
#include <map>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "utils/file_system.hpp"
#include "components/stateful_object.hpp"

namespace texture {

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

GLuint get_texture_impl(std::string path) {
	if (texture_data.find(path) != texture_data.end()) {
		return texture_data[path];
	}
	GLuint res;
	if (path.empty()) {
		unsigned char data[4] = {255, 255, 255, 255};
		res = create_texture(1, 1, data);
	} else {
		int width, height, nrChannels;
		std::cout << path << '\n';
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

struct OneTextureObject: public TexturedObject {
	OneTextureObject(std::string name): TexturedObject(), name(name) {}
	GLuint get_texture() {
		return get_texture_impl(file::asset(name + ".png"));
	}
	std::string name;
};

struct NoTextureObject: public TexturedObject {
	NoTextureObject(): TexturedObject() {}
	GLuint get_texture() {
		return get_texture_impl("");
	}
};
NoTextureObject no_texture;

struct AnimationTimer : public animation::AnimatedObject {
	AnimationTimer() : animation::AnimatedObject() {}

	void update(float dt) {
		time += dt;
	}
	float time = 0.0;
};

struct AnimatedTexture : public TexturedObject, states::StringStateful {
	AnimatedTexture(std::string name, std::map<std::string, std::vector<float>> data): name(name), TexturedObject(), animation(), states::StringStateful(data.begin()->first), durations(data) {}
	~AnimatedTexture() {}
	GLuint get_texture() {

		while (durations[get_state()][current_index] <= animation.time) {
			animation.time -= durations[get_state()][current_index];
			current_index += 1;
			if (current_index == durations[get_state()].size()) {
				current_index = 0;
			}
		}
		return get_texture_impl(file::asset(name + "_" + get_state() + "_" + std::to_string(current_index) + ".png"));
	}
	void set_state(std::string state) {
		if (get_state() == state) return;
		states::StringStateful::set_state(state);
		animation.time = 0.0f;
		current_index = 0;
	}

	void bind(entity::Entity* e) {
		TexturedObject::bind(e);
		animation.bind(e);
		states::StringStateful::bind(e);
	}
	AnimationTimer animation;
	std::map<std::string, std::vector<float>> durations;
	std::string name;
	size_t current_index = 0;
};

}