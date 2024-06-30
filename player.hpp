#pragma once
#include "render_system.hpp"
#include "input_system.hpp"
#include "sprite.hpp"

namespace player {

double position_x = 0;
double position_y = 0;
sprite::AnimatedSprite player("mario", -0.1, -0.1, 0.1, 0.1);

void update() {
	if (input::get_button_state(SDL_SCANCODE_DOWN)) {
		position_y -= 0.01;
		player.b -= 0.01;
		player.t -= 0.01;
	}

	if (input::get_button_state(SDL_SCANCODE_UP)) {
		position_y += 0.01;
		player.b += 0.01;
		player.t += 0.01;
	}

	if (input::get_button_state(SDL_SCANCODE_LEFT)) {
		position_x -= 0.01;
		player.l -= 0.01;
		player.r -= 0.01;
	}

	if (input::get_button_state(SDL_SCANCODE_RIGHT)) {
		position_x += 0.01;
		player.l += 0.01;
		player.r += 0.01;
	}
}

void init() {
	player.durations["idle"] = {0.1, 0.2, 0.1};
}

}