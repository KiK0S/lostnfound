#pragma once
#include "systems/input_system.hpp"
#include "systems/definitions/dynamic_object.hpp"
#include "systems/definitions/gpu_program.hpp"
#include "rendering/sprite.hpp"
#include "rendering/programs.hpp"
#include "camera/camera.hpp"
#include "glm/glm/vec2.hpp"

namespace player {


struct Player : public dynamic::DynamicObject, sprite::AnimatedSprite, shaders::ShaderUniformsObject {
	Player(): dynamic::DynamicObject(), sprite::AnimatedSprite("monster", -0.1, -0.1, 0.1, 0.1), ShaderUniformsObject({&gpu_programs::raycast_program}) {
		durations["idle"] = {0.3, 0.1, 0.1, 0.1};
		durations["runright"] = {0.05, 0.05, 0.05, 0.05, 0.05, 0.05};
		durations["runleft"] = {0.05, 0.05, 0.05, 0.05, 0.05, 0.05};
	}
	~Player() {}

	glm::vec2 pos{0.0f, 0.0f};
	glm::vec2 raycast_start;
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
		raycast_start = pos - camera::camera.pos;
	}
	void reg_uniforms(GLuint program) {
		auto playerLocation = glGetUniformLocation(program, "uStartPosition");
		glUniform2fv(playerLocation, 1, glm::value_ptr(raycast_start));
	}
};

Player player;


}