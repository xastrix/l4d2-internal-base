#pragma once

#include <string>
#include <vector>
#include <d3dx9.h>
#include <array>

#include "vars.h"

enum _font_list {
	Tahoma12px,
	Left4Dead_Icons21px,
	maxFonts,
};

enum _draw_text_flags {
	TEXT_NONE,
	TEXT_CENTER_X,
	TEXT_OUTLINE,
};

struct font_t {
	_font_list m_index{};
	int m_px{};
	std::string m_font_name{};
	uint32_t m_weight{};
	DWORD m_quality{};
};

class color_t {
public:
	color_t(int r = 255, int g = 255, int b = 255, int a = 255)
		: m_color{ r, g, b, a } {}

	explicit color_t(const std::string& var_name, int alpha = 255)
		: m_color{
			get_var_value(var_name + "->r"),
			get_var_value(var_name + "->g"),
			get_var_value(var_name + "->b"),
			alpha,
	} {}

	color_t(int alpha, const std::string& var_name)
		: m_color{
			alpha,
			get_var_value(var_name + "->r"),
			get_var_value(var_name + "->g"),
			get_var_value(var_name + "->b"),
	} {};

	D3DCOLOR get() const {
		return D3DCOLOR_RGBA(m_color[0], m_color[1], m_color[2], m_color[3]);
	}

	int get_r() const {
		return m_color[0];
	}

	int get_g() const {
		return m_color[1];
	}

	int get_b() const {
		return m_color[2];
	}

	int get_a() const {
		return m_color[3];
	}

private:
	std::array<int, 4> m_color{};

	int get_var_value(const std::string& key) const {
		auto opt = g_vars.get_as<int>(key);

		if (!opt.has_value())
			return 0;

		return opt.value();
	}
};

struct fonts {
	void initialise(IDirect3DDevice9* device, const std::vector<font_t> list);

	// Print text function
	void draw_text(const std::string& string, int x, int y, _font_list font, uint8_t flags, color_t color = color_t{ 255, 255, 255 });
	
	// Get width
	int get_text_width(_font_list font, const std::string& string);
	
	// Get height
	int get_text_height(_font_list font, const std::string& string);

	// Get font by index
	ID3DXFont* get_font(_font_list index);

	void shutdown();

private:
	std::vector<font_t> m_font_list{};
	ID3DXFont* m_fonts[maxFonts]{};
};

inline fonts g_fonts{};