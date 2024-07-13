#pragma once
#include "render_system.hpp"
#include "sprite.hpp"
#include "delaunay-cpp/delaunay.hpp"


namespace area {

struct AreaObject : public render::Drawable {
	AreaObject(std::string name, std::vector<glm::vec2> points, glm::vec4 color): render::Drawable(), name(name), color(color) {
		std::vector<delaunay::Point<float>> data(points.size());
		for (int i = 0; i < points.size(); i++)
			data[i] = delaunay::Point<float>(points[i].x, points[i].y);
		auto res = delaunay::triangulate(data);
		this->points.resize(res.triangles.size() * 6);
		this->uv.assign(res.triangles.size() * 6, 0.0f);
		for (int i = 0; i < res.triangles.size(); i++) {
			std::cout << res.triangles[i].p0.x << ' ' << res.triangles[i].p0.y << ' ' << 
			res.triangles[i].p1.x << ' ' << res.triangles[i].p1.y << ' ' << 
			res.triangles[i].p2.x << ' ' << res.triangles[i].p2.y << '\n';
			this->points[6 * i + 0] = res.triangles[i].p0.x;
			this->points[6 * i + 1] = res.triangles[i].p0.y;
			this->points[6 * i + 2] = res.triangles[i].p2.x;
			this->points[6 * i + 3] = res.triangles[i].p2.y;
			this->points[6 * i + 4] = res.triangles[i].p1.x;
			this->points[6 * i + 5] = res.triangles[i].p1.y;
		}
	}
	std::vector<float> get_pos() {
		return points;
	}
	std::vector<float> get_uv() {
		return uv;
	}
	glm::vec4 get_color() {
		return color;
	}
	std::vector<float> get_model_matrix() {
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
	int get_layer() const {
		return 3;
	}
	GLuint get_texture() {
		return render::get_texture("");
	}
	std::string get_name() const {
		return name;
	}


	std::string name;
	glm::vec4 color;
	std::vector<float> points;
	std::vector<float> uv;
};

}