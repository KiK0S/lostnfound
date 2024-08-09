#pragma once
#include <vector>
#include "components/component.hpp"


namespace spawn {

struct SpawnerObject;

std::vector<SpawnerObject*> spawners;


struct SpawningRule {
	double density;
	std::function<entity::Entity*(glm::vec2)> spawn;
};

struct SpawnerObject: public components::Component {
	SpawnerObject(): components::Component() {
		spawners.push_back(this);
	}
	virtual ~SpawnerObject() {}

	virtual std::vector<SpawningRule> get_rules() = 0;
};

struct SpawnerRuleContainer: public SpawnerObject {
	SpawnerRuleContainer(std::vector<SpawningRule> rules): SpawnerObject(), rules(rules) {}

	std::vector<SpawningRule> get_rules() {
		return rules;
	}
	std::vector<SpawningRule> rules;
};
}