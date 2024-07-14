#pragma once
#include "sprite.hpp"
#include "render_system.hpp"
#include "input_system.hpp"
#include "course.hpp"
#include <memory>
#include <vector>
#include "random.hpp"
#include "curve.hpp"
#include "tilemap.hpp"
#include "area.hpp"
#include "CImg.h"
#include "visibility.hpp"
#include "spawning_system.hpp"


namespace map
{

uint8_t color[4] =	{56, 91, 94, 255};

struct MapObject : public sprite::Sprite, visibility::BlockingObject {
	MapObject(const std::string& name, double x, double y, double width, double height):
		sprite::Sprite(name, (y + 10) / 20.0 * 1.2 - 0.6 - 0.05, (x + 10) / 20.0 * 1.2 - 0.6 - 0.05, (y + 10) / 20.0 * 1.2 - 0.6 + 0.05, (x + 10) / 20.0 * 1.2 - 0.6 + 0.05, 102),
		visibility::BlockingObject(),
		full(sprite::Sprite(name, y, x, y + height, x + width, 2)) {}
	sprite::Sprite full;
	// minimap::MiniMapObject obj;
	virtual glm::vec2 get_center_position() {
		return glm::vec2{(full.l + full.r) / 2.0, (full.b + full.t) / 2.0};
	}
	bool show() {
		return false;
	}
};

struct Map {

	std::vector<std::unique_ptr<MapObject>> objects;
	tilemap::TileMap tilemap;
	course::Course course;
	curve::Curve curve;
	area::AreaObject lake;
	int n, m;

	Map(int n, int m): n(n), m(m), course(5), tilemap(n, m), curve({}), lake("lake", {
		glm::vec2(-3.0f, 0.0f),
		glm::vec2(-2.5f, -0.1f),
		glm::vec2(-2.4f, 0.1f),
		glm::vec2(-1.0f, -0.3f),
		glm::vec2(-0.75f, -0.75f),
		glm::vec2(-1.0f, -2.0f),
		glm::vec2(-2.5f, -1.3f)
	}, glm::vec4{0.02f, 0.1f, 0.8f, 1.0f}, 
		{
			spawn::SpawningRule{
				0.7,
				[](glm::vec2 pos) {
					return std::make_unique<sprite::Sprite>("duck", pos.x - 0.1, pos.y - 0.1, pos.x + 0.1, pos.y + 0.1, 2);
				}
			}	
		}
	) {
		int treesCnt = 50;
		int rockCnt = 20;

		for (int i = 0; i < treesCnt; i++) {
			objects.emplace_back(std::make_unique<MapObject>("tree", rnd::get_double(-10, 10), rnd::get_double(-10, 10), 0.4, 0.4));
		}
		for (int i = 0; i < rockCnt; i++) {
			objects.emplace_back(std::make_unique<MapObject>("rock", rnd::get_double(-10, 10), rnd::get_double(-10, 10), 0.3, 0.3));
		}

		curve = curve::Curve({-0.75f, -0.75f,
			-0.25f,  1.0f,
			0.25f, -1.0f,
			0.75f,  0.0f,
			1.0f, 0.0f,
			1.25f, -0.5f,
			1.7f, 2.0f,
			1.8f, 1.0f,
			1.6f, 0.8f,
			1.5f, 0.9f});
	}
};

Map map(100, 100);

} // namespace map
