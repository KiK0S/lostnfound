#pragma once
#include "sprite.hpp"
#include "render_system.hpp"
#include "input_system.hpp"
#include "course.hpp"
#include <memory>
#include <vector>
#include "random.hpp"
#include "curve.hpp"
#include "CImg.h"


namespace map
{

std::vector<uint8_t> color_map[] = {
	{56, 91, 94, 255},
	{56, 91, 94, 255},
	{56, 91, 94, 255},
	{191, 4, 151, 255},
	// {48, 72, 74, 255},
	// {115, 86, 8, 255},
};

struct Tile : public sprite::Sprite {
	Tile(const std::string& name, int tile_type, double x, double y, double width, double height):
		sprite::Sprite(name + "_" + std::to_string(tile_type), y, x, y + height, x + width, 0), tile_type(tile_type) {}
	~Tile() {}

	bool show() {
		if (l - camera::position_x >= 1.2 || camera::position_x - r >= 1.2 || b - camera::position_y >= 1.2 || camera::position_y - t >= 1.2)
			return false;
		return true; 
	}
	int tile_type;
};

struct MapObject : public sprite::Sprite {
	MapObject(const std::string& name, bool* visible, double x, double y, double width, double height):
		sprite::Sprite(name, (y + 10) / 20.0 * 1.2 - 0.6 - 0.05, (x + 10) / 20.0 * 1.2 - 0.6 - 0.05, (y + 10) / 20.0 * 1.2 - 0.6 + 0.05, (x + 10) / 20.0 * 1.2 - 0.6 + 0.05, 102), visible(visible),
		full(sprite::Sprite(name, y, x, y + height, x + width, 2)) {}
	bool* visible;
	sprite::Sprite full;
	bool show() {
		return *visible;
	}
};

struct Map : public render::Drawable, input::Controllable {

	std::vector<std::vector<std::unique_ptr<Tile>>> tiles;
	std::vector<std::unique_ptr<MapObject>> objects;
	course::Course course;
	curve::Curve curve;
	int n, m;
	bool visible = false;

	Map(int n, int m): n(n), m(m), course(5), render::Drawable(), input::Controllable(), curve({}) {
		double width = 20.0 / n;
		double height = 20.0 / m;

		for (int i = -n/2; i < n/2; i++) {
			tiles.emplace_back(std::vector<std::unique_ptr<Tile>>());
			for (int j = -m/2; j < m/2; j++) {
				tiles.back().emplace_back(std::make_unique<Tile>("tile", rnd::getInt(0, 2), i * width, j * height, width, height));
			}
		}
		int treesCnt = 50;
		int rockCnt = 20;

		for (int i = 0; i < treesCnt; i++) {
			objects.emplace_back(std::make_unique<MapObject>("tree", &visible, rnd::getDouble(-10, 10), rnd::getDouble(-10, 10), 0.4, 0.4));
		}
		for (int i = 0; i < rockCnt; i++) {
			objects.emplace_back(std::make_unique<MapObject>("rock", &visible, rnd::getDouble(-10, 10), rnd::getDouble(-10, 10), 0.3, 0.3));
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
	~Map() {}
	int get_layer() const {
		return 101;
	}
	std::vector<float> get_pos() {
		return {
			-0.6f, -0.6f, // Vertex 1: top left
			0.6f, -0.6f, // Vertex 2: top right
			-0.6f, 0.6f, // Vertex 3: bottom left

			0.6f, -0.6f, // Vertex 4: top right
			-0.6f, 0.6f, // Vertex 5: bottom left
			0.6f, 0.6f  // Vertex 6: bottom right
		};
	}
	virtual std::vector<float> get_uv() {
		return {
				0.0f,  1.0f, // Vertex 1: top left
				1.0f,  1.0f, // Vertex 2: top right
				0.0f, 0.0f, // Vertex 3: bottom left

				1.0f,  1.0f, // Vertex 4: top right
				0.0f, 0.0f, // Vertex 5: bottom left
				1.0f, 0.0f  // Vertex 6: bottom right
			};
	}
	virtual std::vector<float> get_model_matrix() {
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	};

	GLuint texture_id = 0;
	void create_map_texture() {
		int k = 10;
		cimg_library::CImg<uint8_t> map(k * n, k * m, 1, 4);

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				for (int xi = i * k; xi < (i + 1) * k; xi++) {
					for (int yj = j * k; yj < (j + 1) * k; yj++) {
						map.draw_point(xi, yj, 0, color_map[tiles[i][j]->tile_type].data());
					}
				}
			}
		}
		auto interpolate_coords = [&](float x) {
			x = (x + 10) / 20.0;
			return int(k * n * x);
		};
		map.draw_triangle(interpolate_coords(course.controls[0].position_y + 1),
											interpolate_coords(course.controls[0].position_x),
											interpolate_coords(course.controls[0].position_y - 0.5),
											interpolate_coords(course.controls[0].position_x - 0.5),
											interpolate_coords(course.controls[0].position_y - 0.5),
											interpolate_coords(course.controls[0].position_x + 0.5),
											color_map[3].data());
		for (int i = 1; i < course.controls.size(); i++) {
			std::cout << course.controls[i].position_x << ' ' << course.controls[i].position_y << '\n'; 
			map.draw_circle(interpolate_coords(course.controls[i].position_y),
											interpolate_coords(course.controls[i].position_x),
											10,
											color_map[3].data());
		}
		for (int i = 1; i < course.controls.size(); i++) {
			draw_line(map, interpolate_coords(course.controls[i - 1].position_y),
										interpolate_coords(course.controls[i - 1].position_x),
										interpolate_coords(course.controls[i].position_y),
										interpolate_coords(course.controls[i].position_x),
										color_map[3].data(), 5);
		}

		std::vector<uint8_t> data;
		for (int i = 0; i < k * n; i++) {
			for (int j = 0; j < k * m; j++) {
				data.push_back(map.atXY(i, j, 0, 0));
				data.push_back(map.atXY(i, j, 0, 1));
				data.push_back(map.atXY(i, j, 0, 2));
				data.push_back(map.atXY(i, j, 0, 3));
			}
		}
		texture_id = render::create_texture(k * n, k * m, data.data(), GL_LINEAR);
	}
	virtual GLuint get_texture() {
		if (texture_id == 0) {
			create_map_texture();
		}
		return texture_id;
	};
	std::string get_name() const {
		return "map";
	};
	virtual bool show() {
		return visible;
	}
	render::Program* get_program() {
		return &render::texture;
	}

	void handle_user_action(SDL_Event e) {
		if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
			visible = !visible;
	}

};

Map map(100, 100);

} // namespace map
