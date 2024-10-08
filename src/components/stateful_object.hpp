#pragma once
#include <vector>
#include <iostream>
#include "components/component.hpp"

namespace states {

struct StatefulObject;

std::vector<StatefulObject*> statefuls;

struct StatefulObject: public components::Component {
	StatefulObject(): components::Component() {
		statefuls.push_back(this);
	}
	virtual ~StatefulObject() {}

	virtual std::string get_state() = 0;
	virtual void set_state(std::string) = 0;
};

struct StringStateful: public StatefulObject {
	StringStateful(std::string state): StatefulObject(), state(state) {
	}
	virtual ~StringStateful() {}

	std::string state;
	virtual std::string get_state() {
		return state;
	}
	virtual void set_state(std::string new_state) {
		state = new_state;
	}
};

}