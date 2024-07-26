#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include "components/component.hpp"

namespace input {
struct ControllableObject;
std::vector<ControllableObject*> controllables;

struct ControllableObject : public components::Component {
	ControllableObject(): components::Component() {
		controllables.push_back(this);
	}
	virtual ~ControllableObject() {}
	virtual void handle_user_action(SDL_Event event) = 0;
};

}