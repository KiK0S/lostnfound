#pragma once
#include "utils/random.hpp"
#include "glm/glm/vec2.hpp"
#include "components/drawable_object.hpp"
#include "components/init_object.hpp"
#include "components/spawner_object.hpp"
#include <functional>
#include <vector>
#include <memory>

namespace spawn {

glm::vec2 point(glm::vec2 a, glm::vec2 b, glm::vec2 c) {
	b -= a;
	c -= a;
	while (true) {
		float u = rnd::get_double(0.0, 1.0);
		float v = rnd::get_double(0.0, 1.0);
		if (u <= 0.8 && u >= 0.2 && v <= 0.8 && v >= 0.2 && v + u <= 0.8)
			return a + u * b + v * c;
	}
}

std::vector<glm::vec2> points(glm::vec2 a, glm::vec2 b, glm::vec2 c, double density) {
	double s = std::abs((b - a).x * (c - a).y - (b - a).y * (c - a).x);
	double to_spawn = s * density * rnd::get_normal(1.0, 0.3);
	double remainder = to_spawn - int(to_spawn);
	int for_spawn = int(to_spawn) + rnd::toss_coin(remainder);
	std::vector<glm::vec2> res(for_spawn);
	for (int i = 0; i < for_spawn; i++) {
		res[i] = point(a, b, c);
	}
	return res;
}

struct SpawnerSystem: public init::UnInitializedObject {
	SpawnerSystem(): init::UnInitializedObject(1) {}

	void init() {
		entity::Entity* e = get_entity();
		for (auto* obj : spawners) {
			entity::Entity* spawner_e = obj->get_entity();
			geometry::GeometryObject* geom_ptr = spawner_e->get<geometry::GeometryObject>();
			if (geom_ptr == nullptr) continue;
			for (auto rule : obj->get_rules()) {
				auto points = geom_ptr->get_pos();
				for (int i = 0; i < geom_ptr->get_size(); i += 3) {
					std::vector<glm::vec2> new_points = spawn::points(points[i], points[i + 1], points[i + 2],
																												rule.density);
					for (auto point : new_points) {
						point_objects.push_back(rule.spawn(point));
					}
				}
			}
		}
	}

	std::vector<std::unique_ptr<entity::Entity>> point_objects;

};

SpawnerSystem	spawner_system;

};