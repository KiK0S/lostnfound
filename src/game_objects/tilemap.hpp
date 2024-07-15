#pragma once
#include <string>
#include "rendering/sprite.hpp"
#include "camera/camera.hpp"

namespace tilemap {

struct Tile : public sprite::Sprite {
	Tile(const std::string& name, int tile_type, double x, double y, double width, double height):
		sprite::Sprite(name + "_" + std::to_string(tile_type), y, x, y + height, x + width, 0), tile_type(tile_type) {}
	~Tile() {}
	int tile_type;
};


struct TileMap {
	std::vector<std::unique_ptr<Tile>> tiles;
	TileMap(int n, int m) {
		double width = 20.0 / n;
		double height = 20.0 / m;

		for (int i = -n/2; i < n/2; i++) {
			for (int j = -m/2; j < m/2; j++) {
				tiles.emplace_back(std::make_unique<Tile>("tile", rnd::get_int(0, 2), i * width, j * height, width, height));
			}
		}
	}
};

}