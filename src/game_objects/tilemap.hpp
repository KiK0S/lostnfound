#pragma once
#include <string>
#include "game_objects/sprite.hpp"
#include "utils/arena.hpp"
namespace tilemap {


std::unique_ptr<entity::Entity> map_object(const std::string& name, double x, double y, double width, double height) {
	auto full_sprite = arena::create<sprite::Sprite>(name, glm::vec2{y - height / 2, x - width / 2}, glm::vec2{y + height / 2, x + width / 2}, 0);
	auto main_scene = arena::create<scene::SceneObject>("main");
	auto e = std::make_unique<entity::Entity>();
	e->add(full_sprite);
	e->add(main_scene);
	e->bind();
	return e;
}

struct TileMap {
	std::vector<std::unique_ptr<entity::Entity>> tiles;
	TileMap(int n, int m) {
		double width = 20.0 / n;
		double height = 20.0 / m;

		for (int i = -n/2; i < n/2; i++) {
			for (int j = -m/2; j < m/2; j++) {
				tiles.emplace_back(map_object("tile_" + std::to_string(rnd::get_int(0, 2)), i * width, j * height, width, height));
			}
		}
	}
};

}