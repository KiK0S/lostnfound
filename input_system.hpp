#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

namespace input {

struct Controllable {
	virtual void handle_user_action(SDL_Event event) = 0;
};

std::vector<Controllable*> controllables;

void poll() {
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
			std::cout << "down " << std::string(SDL_GetKeyName(event.key.keysym.sym)) << std::endl;
			if (std::string(SDL_GetKeyName(event.key.keysym.sym)) == "Q") exit(0);
			break;
		}
		case SDL_KEYUP: {
			std::cout << "up " << std::string(SDL_GetKeyName(event.key.keysym.sym)) << std::endl;
			break;
		}
		case SDL_MOUSEWHEEL: {
			int x, y;
			SDL_GetMouseState(&x, &y);
			// handle
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
}