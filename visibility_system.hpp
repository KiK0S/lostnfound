#pragma once

#include "player.hpp"
#include "map.hpp"
#include "render_system.hpp"
#include "game_loop_system.hpp"
#include "circle.hpp"
#include "sprite.hpp"
#include "visibility.hpp"
#include <string>

namespace visibility {

struct Background: public sprite::Sprite, game_loop::Dynamic {
	Background(const std::string& name): sprite::Sprite(name, -1.0f, -1.0f, 1.0f, 1.0f, -1), game_loop::Dynamic() {}
	
	void update() {
		render::background_texture = get_texture();
	}
	virtual render::Program* get_program() {
		return &render::texture;
	}

};

struct ObstacleMap: public render::Drawable, game_loop::Dynamic {
	ObstacleMap(): render::Drawable(), game_loop::Dynamic(), background_page("cloud") {
		for (const auto& obj : blocking_objects) {
			circles.emplace_back(std::make_unique<circle::Circle>(obj->get_center_position(), obj->get_radius()));
		}
	}
	render::RenderTarget transparency_texture;
	Background background_page;

	std::vector<std::unique_ptr<circle::Circle>> circles;

	std::vector<float> get_pos() {
		return {
			-1.0f, -1.0f, // Vertex 1: top left
			1.0f, -1.0f, // Vertex 2: top right
			-1.0f, 1.0f, // Vertex 3: bottom left

			1.0f, -1.0f, // Vertex 4: top right
			1.0f, 1.0f,  // Vertex 6: bottom right
			-1.0f, 1.0f // Vertex 5: bottom left
		};
	}
	virtual std::vector<float> get_uv() {
		return {
				0.0f,  1.0f, // Vertex 1: top left
				1.0f,  1.0f, // Vertex 2: top right
				0.0f, 0.0f, // Vertex 3: bottom left

				1.0f,  1.0f, // Vertex 4: top right
				1.0f, 0.0f,  // Vertex 6: bottom right
				0.0f, 0.0f // Vertex 5: bottom left
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
		return transparency_texture.output_texture;
	};
	std::string get_name() const {
		return "obstacle-map";
	};
	int get_layer() const {
		return 2;
	}
	virtual bool show() {
		return false;
	}
	bool have_init = false;
	void init() {
		transparency_texture = render::create_render_target();
		have_init = true;
	}
	void update() {
		if (!have_init)
			init();
		render::bind_render_target(&transparency_texture);
		glClearColor(0.0, 0.3, 0.3, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (const auto& circle : circles) {
			render::display(circle.get(), &render::texture_framebuffer);
		}
		render::bind_render_target(nullptr);
		render::transparancy_texture = get_texture();
	}
};

ObstacleMap obstacle_map;

}