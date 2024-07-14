#pragma once
#include "render_system.hpp"
#include "input_system.hpp"
#include "sprite.hpp"
#include "game_loop_system.hpp"
#include "camera.hpp"
#include "glm/glm/vec2.hpp"

namespace player {


struct Player : public game_loop::Dynamic, sprite::AnimatedSprite {
	Player(): game_loop::Dynamic(), sprite::AnimatedSprite("monster", -0.1, -0.1, 0.1, 0.1) {
		durations["idle"] = {0.3, 0.1, 0.1, 0.1};
		durations["runright"] = {0.05, 0.05, 0.05, 0.05, 0.05, 0.05};
		durations["runleft"] = {0.05, 0.05, 0.05, 0.05, 0.05, 0.05};
	}
	~Player() {}
	glm::vec2 pos{0.0f, 0.0f};

	double velocity = 0.02f;

	void update() {

		glm::vec2 d{0.0f, 0.0f};
		// todo: update to velocity-based
		// todo: add stamina
		if (input::get_button_state(SDL_SCANCODE_S)) {
			d.y -= 0.01;
		}

		if (input::get_button_state(SDL_SCANCODE_W)) {
			d.y += 0.01;
		}

		if (input::get_button_state(SDL_SCANCODE_A)) {
			d.x -= 0.01;
		}

		if (input::get_button_state(SDL_SCANCODE_D)) {
			d.x += 0.01;
		}
		
		double len = glm::length(d);
		double k = len <= velocity ? 1.0 : velocity / len;
		d *= k;
		
		pos += d;
		l += d.x;
		r += d.x;
		t += d.y;
		b += d.y;

		if (l == 0) {
			set_state("idle");
		} else if (d.x >= 0) {
			set_state("runright");
		} else {
			set_state("runleft");
		}
		render::raycast_start = pos - camera::pos;
	}
};

Player player;


}