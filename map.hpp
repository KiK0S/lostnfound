#include "sprite.hpp"
#include "render_system.hpp"
#include "input_system.hpp"
#include <memory>
namespace map
{

struct Tile : public sprite::Sprite {
	Tile(const std::string& name, int tile_type, double x, double y, double width, double height):
		sprite::Sprite(name + "_" + std::to_string(tile_type), y, x, y + height, x + width), tile_type(tile_type) {}
	~Tile() {}
	int tile_type;
};

struct Map : public render::Drawable, input::Controllable {

	std::vector<std::vector<std::unique_ptr<Tile>>> tiles;
	int n, m;
	bool visible = false;

	Map(int n, int m): n(n), m(m), render::Drawable(), input::Controllable() {
		double width = 10.0 / n;
		double height = 10.0 / m;

		for (int i = -n/2; i < n/2; i++) {
			tiles.emplace_back(std::vector<std::unique_ptr<Tile>>());
			for (int j = -m/2; j < m/2; j++) {
				tiles.back().emplace_back(std::make_unique<Tile>("tile", rand() % 3, (n/2 - i) * width, j * height, width, height));
			}
		}
	}
	~Map() {}
	int get_layer() const {
		return 2;
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
	virtual GLuint get_texture() {
		std::vector<uint8_t> data;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				for (int k = 0; k < 4; k++) {
					if (k == tiles[i][j]->tile_type)
						data.push_back(0);
					else
						data.push_back(255);
				}
			}
		}

		return render::create_texture(n, m, data.data(), GL_NEAREST);
	};
	std::string get_name() const {
		return "map";
	};
	virtual bool show() {
		return visible;
	}

	void handle_user_action(SDL_Event e) {
		if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
			visible = !visible;
	}

};


} // namespace map
