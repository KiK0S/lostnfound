#pragma once
#include <vector>
#include "game_objects/player.hpp"
#include "camera.hpp"
#include "systems/definitions/dynamic_object.hpp"
#include "glm/gtx/norm.hpp"

namespace camera {

struct CameraUpdate : public dynamic::DynamicObject {
	CameraUpdate(): dynamic::DynamicObject() {}
	~CameraUpdate() {}
	void update() {
		glm::vec2 diff = player::player.pos - camera.pos;
		float d = glm::length2(diff);
		if (d < 0.1)
			return;
		float v = 0.05;
		if (d > 0.4) {
			v *= 5;
		}
		camera.pos += diff * v;
	}
};

CameraUpdate camera_update;

};