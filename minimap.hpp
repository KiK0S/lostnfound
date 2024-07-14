#pragma once
#include "sprite.hpp"
#include "render_system.hpp"
#include "input_system.hpp"
#include <memory>

namespace minimap {
struct MiniMapObject;

std::vector<MiniMapObject*> minimap_objects;

struct MiniMapObject {
	MiniMapObject(std::unique_ptr<render::Drawable>&& drawable): drawable(std::move(drawable)) {
		minimap_objects.push_back(this);
	}
	~MiniMapObject() {}

	
	std::unique_ptr<render::Drawable> drawable;
};

struct MiniMap: public  render::Drawable, input::Controllable {

	MiniMap(): render::Drawable(), input::Controllable() {}
	~MiniMap() {}

	bool visible = false;
	int get_layer() const {
		return 101;
	}

	std::vector<float> get_pos() {
		return {
			0.6f, -0.6f, // Vertex 2: top right
			-0.6f, -0.6f, // Vertex 1: top left
			-0.6f, 0.6f, // Vertex 3: bottom left

			0.6f, -0.6f, // Vertex 4: top right
			-0.6f, 0.6f, // Vertex 5: bottom left
			0.6f, 0.6f // Vertex 6: bottom right
		};
	}
	std::vector<float> get_uv() {
		return {
			0.0f,  1.0f, // Vertex 1: top left
			1.0f,  1.0f, // Vertex 2: top right
			0.0f, 0.0f, // Vertex 3: bottom left

			1.0f,  1.0f, // Vertex 4: top right
			1.0f, 0.0f,  // Vertex 6: bottom right
			0.0f, 0.0f // Vertex 5: bottom left
		};
	}
	std::vector<float> get_model_matrix() {
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	};
	render::RenderTarget render_texture;
	bool inited = false;
	void create_map_texture() {
		std::cout << "create_map_texture\n";
		render_texture = render::create_render_target(GL_RGBA8, GL_RGBA);
				std::cout << "create_render_target\n";

		render::bind_render_target(&render_texture);
		std::cout << "bind_render_target\n";

		// remember previous program to rollback to it as render::display will detach it
		GLint prog = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

		glClearColor(1.0, 0.7, 0.3, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (const auto& object : minimap_objects) {
			render::display(object->drawable.get(), &render::texture_screenspace_program);
		}
		std::cout << "display\n";

		render::bind_render_target(nullptr);
		std::cout << "bind_render_target\n";
		glUseProgram(prog);
		inited = true;
	}

	GLuint get_texture() {
		return render_texture.output_texture;
	};
	std::string get_name() const {
		return "mini-map";
	};
	virtual bool show() {
		if (!inited) {
			create_map_texture();
		}
		return visible;
	}
	render::Program* get_program() {
		return &render::texture_screenspace_program;
	}

	void handle_user_action(SDL_Event e) {
		if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
			visible = !visible;
	}
};

MiniMap minimap;

}