#pragma once
#include "components/dynamic_object.hpp"
#include "components/init_object.hpp"
#include "components/text_object.hpp"
#include "freetype-gl/freetype-gl.h"
#include "utils/file_system.hpp"
#include "utils/arena.hpp"

namespace render {

texture_atlas_t *atlas = 0;
texture_font_t *font = 0;
std::unique_ptr<texture::IntTextureObject> text_texture;
std::map<std::string, geometry::GeometryObject*> geometries;

struct TextLoader: public init::UnInitializedObject {
	TextLoader(): init::UnInitializedObject(1) {}

	void init() {
    atlas = texture_atlas_new( 512, 512, 4 );
    std::string filename = file::asset("Vera.ttf");
    char * text = "abcdefghijklmnopqrstuvwxyz ";
		int font_idx = 0;
		std::cerr << filename << '\n';
		font = texture_font_new_from_file( atlas, 80, filename.c_str() );
		texture_font_load_glyphs( font, text );
		text_texture = std::make_unique<texture::IntTextureObject>(texture::create_texture(atlas->width, atlas->height, atlas->data, GL_RGBA, GL_NEAREST));
		for (auto text_obj : texts) {
			init(text_obj);
		}
	}
	void init(TextObject* t) {
		std::string text = t->get_text();
		auto geom = arena::create<TextGeometry>(text);
		glm::vec2 pen(0.0, 0.0);
		float max_y = 0.0;
		for (int i = 0; i < text.size(); i++) {
			char c = text[i];
			texture_glyph_t *glyph = texture_font_get_glyph( font, &c );
			if( glyph != NULL ) {
				float kerning = 0.0f;
				if( i > 0)
				{
						kerning = texture_glyph_get_kerning( glyph, &text[i - 1] );
				}
				pen.x += kerning;
				int x0  = (int)( pen.x + glyph->offset_x );
				int y0  = (int)( pen.y + glyph->offset_y );
				int x1  = (int)( x0 + glyph->width );
				int y1  = (int)( y0 + glyph->height );
				max_y = std::max(max_y, (float)y1);
				float s0 = glyph->s0;
				float t0 = glyph->t0;
				float s1 = glyph->s1;
				float t1 = glyph->t1;
				geom->pos.push_back({x0, y0});
				geom->uv.push_back({s0, t0});
				geom->pos.push_back({x0, y1});
				geom->uv.push_back({s0, t1});
				geom->pos.push_back({x1, y1});
				geom->uv.push_back({s1, t1});
				geom->pos.push_back({x0, y0});
				geom->uv.push_back({s0, t0});
				geom->pos.push_back({x1, y1});
				geom->uv.push_back({s1, t1});
				geom->pos.push_back({x1, y0});
				geom->uv.push_back({s1, t0});
				pen.x += glyph->advance_x;
			}
		}

		for (auto& v : geom->pos) {
			v.x *= 2.0 / pen.x;
			v.x -= 1.0;
			v.y *= 2.0 / max_y;
			v.y -= 1.0;
		}
		add_to_frame(geom);
		geometries[text] = geom;
	}
};

TextLoader text_loader;

}