#pragma once

#include "components/controllable_object.hpp"
#include "components/drawable_object.hpp"
#include "components/minimap_object.hpp"
#include "components/layered_object.hpp"
#include "systems/geometry_system.hpp"
#include "systems/render_system.hpp"
#include "systems/easy_drawable_system.hpp"
#include "input_system.hpp"
#include <memory>

namespace minimap {

struct MapTexture : public texture::TexturedObject, init::UnInitializedObject {
	MapTexture(): texture::TexturedObject(), init::UnInitializedObject(1) {}
	~MapTexture() {}

	render::RenderTarget render_texture;

	void init() {
		std::cout << "create_map_texture\n";
		render_texture = render::create_render_target(GL_RGBA8, GL_RGBA);
				std::cout << "create_render_target\n";

		render::bind_render_target(&render_texture);
		std::cout << "bind_render_target\n";


		glClearColor(1.0, 0.7, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (const auto& object : minimap_objects) {
			shaders::run_with_program(object->get_drawable()->get_program(), [&](GLuint p) {
				glUniform1i(glGetUniformLocation(p, "toFramebuffer"), false);
			});
			// std::cout << "minimap rendering for" << object->get_drawable()->get_geometry()->get_name() << '\n';
			render::display(object->get_drawable(), object->get_drawable()->get_program());
			shaders::run_with_program(object->get_drawable()->get_program(), [&](GLuint p) {
				glUniform1i(glGetUniformLocation(p, "toFramebuffer"), false);
			});
		}
		render::bind_render_target(nullptr);
	}

	GLuint get_texture() {
		return render_texture.output_texture;
	};
};

struct MiniMapUniforms: public shaders::ShaderUniformsObject {
	MiniMapUniforms(): shaders::ShaderUniformsObject({}) {}
	void reg_uniforms(GLuint p) {
		glUniform1i(glGetUniformLocation(p, "toView"), false);
	}
};

struct MiniMap: public render::DrawableObject {

	MiniMap(): render::DrawableObject() {
		transform.scale(glm::vec2(0.6f, 0.6f));
	}
	~MiniMap() {}


	geometry::GeometryObject* get_geometry() {
		return &geometry::quad;
	}
	virtual transform::TransformObject* get_transform() {
		return &transform;
	}
	virtual layers::LayeredObject* get_layer() {
		return &layer;
	}
	virtual texture::TexturedObject* get_texture() {
		return &texture;
	}
	virtual shaders::ShaderUniformsObject* get_uniform() {
		return &uniforms;
	}
	virtual color::ColoredObject* get_color() {
		return &color::no_color;
	}

	bool visible = false;
	virtual bool hide() {
		return !visible;
	}
	shaders::Program* get_program() {
		return &shaders::static_object_program;
	}

	layers::ConstLayer layer{100};
	render::ModelMatrix model_matrix;
	MiniMapUniforms no_view_matrix;
	render::CombinedUniforms uniforms{{&model_matrix, &no_view_matrix}};
	MapTexture texture;
	transform::NoRotationTransform transform;
};

MiniMap minimap_drawable;

struct OpenMapAction: public input::ControllableObject {
 OpenMapAction(): input::ControllableObject() {

 }
	void handle_user_action(SDL_Event e) {
		if (e.key.keysym.scancode == SDL_SCANCODE_SPACE) {
			minimap_drawable.visible = !minimap_drawable.visible;
		}
	}


};

OpenMapAction open_on_space;
entity::Entity minimap;
scene::SceneObject minimap_scene("main");

void init() {
	minimap.add(&minimap_drawable)
				 .add(&open_on_space)
				 .add(&minimap_scene)
				 .bind();
}

init::CallbackOnStart minimap_init(&init);

}