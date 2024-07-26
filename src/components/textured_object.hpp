#pragma once
#include <vector>
#include <GL/gl.h>
#include "components/component.hpp"

namespace texture {

struct TexturedObject;

std::vector<TexturedObject*> textured;

struct TexturedObject: public components::Component {
	TexturedObject(): components::Component() {
		textured.push_back(this);
	}
	virtual ~TexturedObject() {}

	virtual GLuint get_texture() = 0;
};

}