#pragma once

#include <vector>
#include "game_loop_system.hpp"
#include "random.hpp"
#include "player.hpp"

namespace course {

struct Control {
	Control(float position_x, float position_y): position_x(position_x), position_y(position_y) {}
	float position_x;
	float position_y;
};

struct Course : game_loop::Dynamic, input::Controllable {
	Course(int n) : game_loop::Dynamic() {
		controls.emplace_back(Control(0, 0));
		for (int i = 0; i < n; i++) {
			controls.emplace_back(Control(rnd::get_double(-10, 10), rnd::get_double(-10, 10)));
		}
	}
	~Course() {}
	std::vector<Control> controls;
	float control_radius = 0.05;
	uint32_t current_control = 0;

	void update() {
		if (current_control == controls.size())
			return;
		float diff_x = player::player.position_x - controls[current_control].position_x;
		float diff_y = player::player.position_y - controls[current_control].position_y;
		float d = diff_x * diff_x + diff_y * diff_y;
		if (d > control_radius)
			return;

//  todo:
//		drawText(control_number);
		current_control++;
		std::cout << "found control\n";
	}

	void handle_user_action(SDL_Event e) {
		if (e.key.keysym.scancode == SDL_SCANCODE_E) {
			float diff_x = player::player.position_x - controls[current_control].position_x;
			float diff_y = player::player.position_y - controls[current_control].position_y;
			float d = diff_x * diff_x + diff_y * diff_y;
			std::cout << d << '\n';
		}
	}
};

}