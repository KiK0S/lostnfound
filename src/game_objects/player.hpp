#pragma once
#include "game_objects/sprite.hpp"
#include "systems/input_system.hpp"
#include "components/entity_system.hpp"
#include "systems/touchscreen_system.hpp"
#include "components/dynamic_object.hpp"
#include "components/stateful_object.hpp"
#include "components/gpu_program.hpp"
#include "glm/glm/vec2.hpp"

namespace player {


struct KeyboardMovement : public dynamic::DynamicObject {
	KeyboardMovement(): dynamic::DynamicObject() {}
	~KeyboardMovement(){}
	double velocity = 0.02f;


	void update() {
		entity::Entity* e = get_entity();
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

		d += touchscreen::touchscreen_system.joystick_info;

		
		double len = glm::length(d);
		double k = len <= velocity ? 1.0 : velocity / len;
		d *= k;

		transform::TransformObject* t = e->get<transform::TransformObject>();
		t->translate(d);

		states::StatefulObject* a = e->get<states::StatefulObject>();

		if (len == 0) {
			a->set_state("idle");
		} else if (d.x >= 0) {
			a->set_state("runright");
		} else {
			a->set_state("runleft");
		}
	}
};

KeyboardMovement player_control;



scene::SceneObject player_scene("main");


struct RayCastPosition: public shaders::ShaderUniformsObject {
	RayCastPosition(): ShaderUniformsObject({&shaders::raycast_program}) {}
	~RayCastPosition(){}
	void reg_uniforms(GLuint program) {
		entity::Entity* e = get_entity();
		transform::TransformObject* t = e->get<transform::TransformObject>();
		glm::vec2 raycast_start = t->get_pos();
		auto playerLocation = glGetUniformLocation(program, "uStartPosition");
		glUniform2fv(playerLocation, 1, glm::value_ptr(raycast_start));
	}
};

RayCastPosition raycast_provider;

entity::Entity player;


sprite::AnimatedSprite player_sprite("monster", {-0.1f, -0.1f}, {0.1f, 0.1f}, 2, {
		{"idle", {0.3, 0.1, 0.1, 0.1}},
		{"runright", {0.05, 0.05, 0.05, 0.05, 0.05, 0.05}},
		{"runleft", {0.05, 0.05, 0.05, 0.05, 0.05, 0.05}}
	});

void init() {
	std::cout << "init player\n";
	player.add(&player_sprite)
				.add(&raycast_provider)
				.add(&player_control)
				.add(&player_scene)
				.bind();
}
init::CallbackOnStart player_init(&init);

}