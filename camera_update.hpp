#pragma once
#include <vector>
#include "player.hpp"
#include "camera.hpp"
#include <deque>

namespace camera {

int history_size = 100;
std::deque<float> history;

void update() {
	history.push_back(player::position_x);
	history.push_back(player::position_y);
	if (history.size() < history_size) {
		return;
	}
	position_x = history.front();
	history.pop_front();
	position_y = history.front();
	history.pop_front();
}

};