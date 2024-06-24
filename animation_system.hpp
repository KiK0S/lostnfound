#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <optional>	
#include <GL/gl.h>
#include "render_system.hpp"

namespace animation {

struct Animated {
	virtual void update(float dt) = 0;
	virtual void set_state(std::string state) = 0; 
	virtual std::string get_state() = 0; 
};

std::vector<Animated*> animateds;

std::optional<std::chrono::time_point<std::chrono::system_clock>> m_time = {};

void update() {
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	if (m_time == std::nullopt) {
		m_time = now;
		return;
	}
	float dt = 1.0f * std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_time).count() / 1000.0f;
	m_time = now;

	for (auto* animated : animateds) {
		animated->update(dt);
	}
}

};