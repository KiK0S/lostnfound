#define cimg_display 0
#define GLM_ENABLE_EXPERIMENTAL
#include "systems/game_loop_system.hpp"
#include "systems/input_system.hpp"
#include "systems/render_system.hpp"
#include "systems/window_system.hpp"
#include "utils/file_system.hpp"
#include "rendering/sprite.hpp"
#include "world/world.hpp"
#include "game_objects/player.hpp"
#include "camera/camera_update.hpp"
#include "systems/visibility_system.hpp"
#include "systems/minimap_system.hpp"
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