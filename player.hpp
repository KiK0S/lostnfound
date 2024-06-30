#pragma once
#include "render_system.hpp"
#include "input_system.hpp"
#include "sprite.hpp"
#include "game_loop_system.hpp"
namespace player {


struct Player : public game_loop::Dynamic, sprite::AnimatedSprite {
	Player(): game_loop::Dynamic(), sprite::AnimatedSprite("mario", -0.1, -0.1, 0.1, 0.1) {}
	~Player() {}
	double position_x = 0;
	double position_y = 0;

	void update() {

		// todo: update to velocity-based
		// todo: add stamina
		if (input::get_button_state(SDL_SCANCODE_DOWN)) {
			position_y -= 0.01;
			b -= 0.01;
			t -= 0.01;
		}

		if (input::get_button_state(SDL_SCANCODE_UP)) {
			position_y += 0.01;
			b += 0.01;
			t += 0.01;
		}

		if (input::get_button_state(SDL_SCANCODE_LEFT)) {
			position_x -= 0.01;
			l -= 0.01;
			r -= 0.01;
		}

		if (input::get_button_state(SDL_SCANCODE_RIGHT)) {
			position_x += 0.01;
			l += 0.01;
			r += 0.01;
		}
	}
};

Player player;

void init() {
	player.durations["idle"] = {0.1, 0.2, 0.1};
}

}