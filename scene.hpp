#pragma once
#include "render_system.hpp"
#include <vector>


namespace scene {
	
std::vector<render::Drawable*> drawables;

void init() {
	for (auto* drawable : drawables) {
		render::add_to_frame(drawable);
	}
}

void update() {
	for (auto* drawable : drawables) {
		render::display(drawable);
	}
}

};