#include "game_loop_system.hpp"
#include "input_system.hpp"
#include "render_system.hpp"
#include "window_system.hpp"
#include "file_system.hpp"
#include "sprite.hpp"
#include "map.hpp"

#include <optional>

int main() {
	sprite::Sprite smiley("smiley", -0.5, -0.5, 0.5, 0.5);
	sprite::AnimatedSprite mario("mario", -1.0, -0.7, -0.8, -0.5, [](SDL_Event event, sprite::Sprite* obj){
		if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			obj->b -= 0.1;
			obj->t -= 0.1;
		}

		if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
			obj->b += 0.1;
			obj->t += 0.1;
		}

		if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
			obj->l -= 0.1;
			obj->r -= 0.1;
		}

		if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
			obj->l += 0.1;
			obj->r += 0.1;
		}

	});
	mario.durations["idle"] = {0.1, 0.2, 0.1};
	map::Map map(3, 3);

	auto window = window::get_window("lostnfound");
	#ifdef __EMSCRIPTEN__
	window::get_canvas_context();
	SDL_SetWindowFullscreen(window, 0);
  SDL_SetWindowSize(window, 960, 480);
	#endif
	render::init(window);
	render::start_frame();
	render::init();
	game_loop::update = [&](){
		input::poll();
		animation::update();
	};
	game_loop::render = [&](){
		render::start_frame();
		render::update();
		render::close_frame();
	};
	game_loop::startLoop();
}