#pragma once
#include "render_system.hpp"
#include "input_system.hpp"
#include "sprite.hpp"
#include "game_loop_system.hpp"
namespace player {


struct Player : public game_loop::Dynamic, sprite::AnimatedSprite {
	Player(): game_loop::Dynamic(), sprite::AnimatedSprite("monster", -0.1, -0.1, 0.1, 0.1) {
		durations["idle"] = {0.3, 0.1, 0.1, 0.1};
		durations["runright"] = {0.05, 0.05, 0.05, 0.05, 0.05, 0.05};
		durations["runleft"] = {0.05, 0.05, 0.05, 0.05, 0.05, 0.05};
	}
	~Player() {}
	double position_x = 0;
	double position_y = 0;

	double velocity = 0.02f;

	void update() {

		double dx = 0;
		double dy = 0;
		// todo: update to velocity-based
		// todo: add stamina
		if (input::get_button_state(SDL_SCANCODE_DOWN)) {
			dy -= 0.01;
		}

		if (input::get_button_state(SDL_SCANCODE_UP)) {
			dy += 0.01;
		}

		if (input::get_button_state(SDL_SCANCODE_LEFT)) {
			dx -= 0.01;
		}

		if (input::get_button_state(SDL_SCANCODE_RIGHT)) {
			dx += 0.01;
		}
		
		double d = sqrt(dx * dx + dy * dy);
		double k = d <= velocity ? 1.0 : velocity / d;
		dx *= k;
		dy *= k;
		
		l += dx;
		r += dx;
		t += dy;
		b += dy;
		position_x += dx;
		position_y += dy;

		if (d == 0) {
			set_state("idle");
		} else if (dx >= 0) {
			set_state("runright");
		} else {
			set_state("runleft");
		}
	}
};

Player player;


}