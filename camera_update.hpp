#pragma once
#include <vector>
#include "player.hpp"
#include "camera.hpp"
#include "game_loop_system.hpp"
#include "glm/gtx/norm.hpp"

namespace camera {

struct CameraUpdate : public game_loop::Dynamic {
	CameraUpdate(): game_loop::Dynamic() {}
	~CameraUpdate() {}
	void update() {
		glm::vec2 diff = player::player.pos - pos;
		float d = glm::length2(diff);
		if (d < 0.1)
			return;
		float v = 0.05;
		if (d > 0.4) {
			v *= 5;
		}
		pos += diff * v;
	}
};

CameraUpdate camera_update;

};