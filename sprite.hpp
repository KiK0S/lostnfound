#pragma once
#include "render_system.hpp"
#include "file_system.hpp"
#include "input_system.hpp"
#include "animation_system.hpp"
#include "functional"

namespace sprite {
	struct Sprite : public render::Drawable, input::Controllable {

		Sprite(std::string name, float l, float t, float r, float b, std::function<void(SDL_Event, Sprite*)>&& f, int layer): name(name), l(l), t(t), r(r), b(b), user_callback(std::move(f)), layer(layer), render::Drawable(), input::Controllable() {}
		Sprite(std::string name, float l, float t, float r, float b): Sprite(name, l, t, r, b, [](SDL_Event, Sprite*) {}, 1) {}
		Sprite(std::string name, float l, float t, float r, float b, int layer): Sprite(name, l, t, r, b, [](SDL_Event, Sprite*) {}, layer) {}
		~Sprite() {}
		std::vector<float> get_pos() {
			return {
				-1.0f, -1.0f, // Vertex 1: top left
				1.0f, -1.0f, // Vertex 2: top right
				-1.0f, 1.0f, // Vertex 3: bottom left

				1.0f, -1.0f, // Vertex 4: top right
				-1.0f, 1.0f, // Vertex 5: bottom left
				1.0f, 1.0f  // Vertex 6: bottom right
			};
		}
		std::vector<float> get_uv() {
			return {
				0.0f,  1.0f, // Vertex 1: top left
				1.0f,  1.0f, // Vertex 2: top right
				0.0f, 0.0f, // Vertex 3: bottom left

				1.0f,  1.0f, // Vertex 4: top right
				0.0f, 0.0f, // Vertex 5: bottom left
				1.0f, 0.0f  // Vertex 6: bottom right
			};
		}

		// [-1, 1] -> [l, r]
		// +1
		// [0, 2]
		// / (2)
		// [0, 1]
		// * (r - l)
		// [0, r - l]
		// + l
		std::vector<float> get_model_matrix() {
			return {
				(r - l) / 2.0f, 0.0f, 0.0f, l + (r - l) / 2.0f,
				0.0f, (b - t) / 2.0f, 0.0f, t + (b - t) / 2.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}
		GLuint get_texture() {
			return render::get_texture(file::asset(get_name() + ".png"));
		}
		std::string get_name() const {
			return name;
		}
		void handle_user_action(SDL_Event e) {
			user_callback(e, this);
		}
		int get_layer() const {
			return layer;
		}
		float l, t, r, b;
		std::string name;
		int layer;
		std::function<void(SDL_Event, Sprite*)> user_callback;
	};

	struct AnimatedSprite : public Sprite, animation::Animated {

		AnimatedSprite(std::string name, float l, float t, float r, float b, std::function<void(SDL_Event, Sprite*)>&& f): Sprite(name, l, t, r, b, std::move(f), 1), animation::Animated() {}
		AnimatedSprite(std::string name, float l, float t, float r, float b): AnimatedSprite(name, l, t, r, b, [](SDL_Event, Sprite*) {}) {}
		~AnimatedSprite() {}
		GLuint get_texture() {
			while (durations[get_state()][current_index] <= time) {
				time -= durations[get_state()][current_index];
				current_index += 1;
				if (current_index == durations[get_state()].size()) {
					current_index = 0;
				}
			}
			return render::get_texture(file::asset(get_name() + "_" + get_state() + "_" + std::to_string(current_index) + ".png"));
		}
		void update(float dt) {
			time += dt;
		}
		void set_state(std::string state) {
			if (this->state == state)
				return;
			this->state = state;
			time = 0.0f;
			current_index = 0;
		}
		std::string get_state() { return state; }
		std::string state = "idle";
		std::map<std::string, std::vector<float>> durations;
		size_t current_index = 0;
		float time = 0;
	};
}