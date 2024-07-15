#pragma once

#include "systems/render_system.hpp"
#include "systems/definitions/blocking_object.hpp"
#include "systems/definitions/dynamic_object.hpp"
#include "systems/definitions/gpu_program.hpp"
#include "rendering/programs.hpp"
#include "game_objects/fog.hpp"
#include <string>

namespace visibility {

// extends drawable for debug purposes

struct ObstacleMap: public /*render::Drawable,*/ dynamic::DynamicObject, shaders::ShaderUniformsObject {
	ObstacleMap(): /*render::Drawable(), */dynamic::DynamicObject(), background_page("cloud"), shaders::ShaderUniformsObject({&gpu_programs::raycast_program}) {}
	render::RenderTarget transparency_texture;
	Background background_page;

	// std::vector<float> get_pos() {
	// 	return {
	// 		-1.0f, -1.0f, // Vertex 1: top left
	// 		1.0f, -1.0f, // Vertex 2: top right
	// 		-1.0f, 1.0f, // Vertex 3: bottom left

	// 		1.0f, -1.0f, // Vertex 4: top right
	// 		1.0f, 1.0f,  // Vertex 6: bottom right
	// 		-1.0f, 1.0f // Vertex 5: bottom left
	// 	};
	// }
	// virtual std::vector<float> get_uv() {
	// 	return {
	// 			0.0f,  1.0f, // Vertex 1: top left
	// 			1.0f,  1.0f, // Vertex 2: top right
	// 			0.0f, 0.0f, // Vertex 3: bottom left

	// 			1.0f,  1.0f, // Vertex 4: top right
	// 			1.0f, 0.0f,  // Vertex 6: bottom right
	// 			0.0f, 0.0f // Vertex 5: bottom left
	// 		};
	// }
	// virtual std::vector<float> get_model_matrix() {
	// 	return {
	// 		1.0f, 0.0f, 0.0f, 0.0f,
	// 		0.0f, 1.0f, 0.0f, 0.0f,
	// 		0.0f, 0.0f, 1.0f, 0.0f,
	// 		0.0f, 0.0f, 0.0f, 1.0f
	// 	};
	// };
	// virtual GLuint get_texture() {
	// 	return transparency_texture.output_texture;
	// };
	// std::string get_name() const {
	// 	return "obstacle-map";
	// };
	// int get_layer() const {
	// 	return 2;
	// }
	// virtual bool show() {
	// 	return false;
	// }
	bool have_init = false;
	void init() {
		transparency_texture = render::create_render_target(GL_R8, GL_RED);
		have_init = true;
	}
	void update() {
		if (!have_init)
			init();
		render::bind_render_target(&transparency_texture);
		glClearColor(0.0, 0.3, 0.3, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (const auto& obj : blocking_objects) {
			render::display(obj, &gpu_programs::visibility_program);
		}
		render::bind_render_target(nullptr);
	}
	void reg_uniforms(GLuint program) {		
		auto transparancyTexture = glGetUniformLocation(program, "uTransparency");
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, transparency_texture.output_texture);
		glUniform1i(transparancyTexture, 1);
	}
};

ObstacleMap obstacle_map;

}