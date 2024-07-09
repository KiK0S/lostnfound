#pragma once
#include <vector>
#include <iostream>
namespace camera {


float position_x = 0.0;
float position_y = 0.0;

std::vector<float> get_matrix(int layer) {
	if (layer >= 100 || layer < 0) {
		return {
			1.0f, 0.0f, 0.0f, -0.0f,
			0.0f, 1.0f, 0.0f, -0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f	
		};
	}
	// std::cout << position_x << ' ' << position_y << '\n';
	return {
		1.0f, 0.0f, 0.0f, -position_x,
		0.0f, 1.0f, 0.0f, -position_y,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

};