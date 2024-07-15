#pragma once

#include <vector>
#include "systems/definitions/dynamic_object.hpp"
#include "systems/definitions/controllable_object.hpp"
#include "systems/definitions/minimap_object.hpp"
#include "utils/random.hpp"
#include "game_objects/player.hpp"
#include "geometry/circle.hpp"
#include "glm/glm/vec2.hpp"
#include "glm/gtx/norm.hpp"

namespace course {

uint8_t color[4] = {191, 4, 151, 255};


struct Course : dynamic::DynamicObject, input::ControllableObject {
	Course(int n) : dynamic::DynamicObject() {
		positions.emplace_back(glm::vec2(0, 0));
		for (int i = 0; i < n; i++) {
			positions.emplace_back(glm::vec2(rnd::get_double(-10, 10), rnd::get_double(-10, 10)));
		}
		// drawables.emplace_back(std::make_unique<minimap::MiniMapObject>(std::make_unique<triangle::Triangle>(positions[0])));
		// for (int i = 1; i < n + 1; i++) {
		// 	drawables.emplace_back(std::make_unique<minimap::MiniMapObject>(positions[i] / 10.0f, control_radius)));
		// }
	}
	~Course() {}
	std::vector<glm::vec2> positions;
	std::vector<std::unique_ptr<minimap::MiniMapObject>> drawables;
	float control_radius = 0.05;
	uint32_t current_control = 0;

	void update() {
		if (current_control == positions.size())
			return;
		glm::vec2 diff = player::player.pos - positions[current_control];
		float d = glm::length2(diff);
		if (d > control_radius)
			return;

//  todo:
//		drawText(control_number);
		current_control++;
		std::cout << "found control\n";
	}

	void handle_user_action(SDL_Event e) {
		if (e.key.keysym.scancode == SDL_SCANCODE_E) {
			glm::vec2 diff = player::player.pos - positions[current_control];
			float d = glm::length2(diff);
			std::cout << d << '\n';
		}
	}
};

}