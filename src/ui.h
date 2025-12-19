#pragma once

#include "renderer.h"
#include "ui_imgui.h"

struct ui {
	void initialise(IDirect3DDevice9* device);

	// Reset objects
	void invalidate_device_objects();
	
	// Create objects
	void create_device_objects();

	// Render (Init colors, styles and menu)
	void render();

	void toggle();

	bool get_menu_state() {
		return m_opened;
	}

	void set_menu_state(bool v) {
		m_opened = v;
	}

	void shutdown();

private:
	// Set up menu elements
	void draw_main_window();

	// Set up watermark elements
	void draw_watermark();

private:
	bool m_opened{};
	int m_old_mouse_pos_x{},
		m_old_mouse_pos_y{};
	int m_selected_tab{};
};

inline ui g_ui{};