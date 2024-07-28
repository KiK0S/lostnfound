#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include "components/all_components.hpp"
#include "systems/systems_system.hpp"
#include "utils/file_system.hpp"
#include "world/world.hpp"
#include "game_objects/player.hpp"
#include "game_objects/lake.hpp"
#include "game_objects/river.hpp"
#include "game_objects/following_camera.hpp"
#include <optional>

int main() {
	auto window = window::get_window("lostnfound");
	#ifdef __EMSCRIPTEN__
	window::get_canvas_context();
	SDL_SetWindowFullscreen(window, 0);
  SDL_SetWindowSize(window, 960, 960);
	#endif
	render::init(window);
	render::init();
	init::init();
	scene::main.activate();
	game_loop::startLoop();
}