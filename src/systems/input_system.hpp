#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "components/dynamic_object.hpp"
#include "components/controllable_object.hpp"

namespace input {

bool get_button_state(SDL_Scancode key) {
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	return state[key];
}

struct Input: public dynamic::DynamicObject {
	Input(): dynamic::DynamicObject() {}
	~Input(){}
	void update() {
		while (true) {
			SDL_Event event;
			if (SDL_PollEvent(&event) == 0) {
					break;
			}
			switch (event.type) {
			case SDL_KEYDOWN: {
				for (auto* controllable : controllables) {
					controllable->handle_user_action(event);
				}
				if (std::string(SDL_GetKeyName(event.key.keysym.sym)) == "Q") exit(0);
				break;
			}
			case SDL_KEYUP: {
				break;
			}
			case SDL_MOUSEWHEEL: {
				int x, y;
				SDL_GetMouseState(&x, &y);
				// handle static_cast<float>(event.motion.x), static_cast<float>(event.motion.y)
				break;
			}
			case SDL_MOUSEMOTION: {
				// handle static_cast<float>(event.motion.x), static_cast<float>(event.motion.y)
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP: {
				int x, y;
				SDL_GetMouseState(&x, &y);
				// handle static_cast<float>(event.motion.x), static_cast<float>(event.motion.y)
				break;
			}
			case SDL_QUIT:
				exit(0);
			}
		}
	}
};
Input input;

}