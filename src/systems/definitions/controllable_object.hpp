#pragma once
#include <vector>
#include <SDL2/SDL.h>

namespace input {
struct ControllableObject;
std::vector<ControllableObject*> controllables;

struct ControllableObject {
	ControllableObject() {
		controllables.push_back(this);
	}
	virtual ~ControllableObject() {}
	virtual void handle_user_action(SDL_Event event) = 0;
};

}