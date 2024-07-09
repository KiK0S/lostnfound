#define cimg_display 0
#include "game_loop_system.hpp"
#include "input_system.hpp"
#include "render_system.hpp"
#include "window_system.hpp"
#include "file_system.hpp"
#include "sprite.hpp"
#include "map.hpp"
#include "player.hpp"
#include "camera_update.hpp"
#include "visibility_system.hpp"
#include <optional>

int main() {
	auto window = window::get_window("lostnfound");
	#ifdef __EMSCRIPTEN__
	window::get_canvas_context();
	SDL_SetWindowFullscreen(window, 0);
  SDL_SetWindowSize(window, 960, 480);
	#endif
	render::init(window);
	render::init();
	game_loop::render = [&](){
		render::start_frame();
		render::update();
		render::close_frame();
	};
	game_loop::startLoop();
}