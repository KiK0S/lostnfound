#pragma once
#include <vector>
#include <iostream>
#include "glm/glm/vec2.hpp"
namespace camera {


glm::vec2 pos{0.0f, 0.0f};

std::vector<float> get_matrix(int layer) {
	return {
		1.0f, 0.0f, 0.0f, -pos.x,
		0.0f, 1.0f, 0.0f, -pos.y,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

};