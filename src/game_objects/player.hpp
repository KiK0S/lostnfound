#pragma once
#include "systems/input_system.hpp"
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


entity::Entity* get_player();
}