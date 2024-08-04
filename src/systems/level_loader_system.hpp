#pragma once

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include "components/entity_system.hpp"
#include "game_objects/lake.hpp"
#include "game_objects/river.hpp"
namespace level_loader {

std::vector<std::unique_ptr<entity::Entity>> entities;
void load(std::string filename) {
	std::ifstream in(filename);
	std::string s;
	while (in >> s) {
		if (s == "lake") {
			in >> s;
			int n;
			in >> n;
			std::vector<glm::vec2> points(n);
			for (int i = 0; i < n; i++) {
				in >> points[i].x >> points[i].y;
			}
			entities.push_back(lake::create_lake(s, points));
		}
		if (s == "river") {
			int n;
			in >> n;
			std::vector<glm::vec2> points(n);
			for (int i = 0; i < n; i++) {
				in >> points[i].x >> points[i].y;
			}
			river::create_river(s, points);
		}
	}
}


}