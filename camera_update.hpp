#pragma once
#include <vector>
#include "player.hpp"
#include "camera.hpp"
#include "game_loop_system.hpp"

namespace camera {

struct CameraUpdate : public game_loop::Dynamic {
	CameraUpdate(): game_loop::Dynamic() {}
	~CameraUpdate() {}
	void update() {
		float diff_x = player::player.position_x - position_x;
		float diff_y = player::player.position_y - position_y;
		float d = diff_x * diff_x + diff_y * diff_y;
		if (d < 0.1)
			return;
		float v = 0.05;
		if (d > 0.4) {
			v *= 5;
		}
		position_x += diff_x * v;
		position_y += diff_y * v;
	}
};

CameraUpdate camera_update;

};