#pragma once

#include "util.h"

#include <functional>

enum _button_state {
	bs_none = 1,
	bs_down,
	bs_up,
	bs_pressed,
};

struct mouse_t {
	friend struct input;

	int get_mouse_pos_x() {
		return m_mouse_pos_x;
	}

	int get_mouse_pos_y() {
		return m_mouse_pos_y;
	}

	int get_mouse_wheel_accumlate() {
		return m_wheel_accumulate;
	}

private:
	int m_mouse_pos_x{};
	int m_mouse_pos_y{};
	int m_wheel_accumulate{};
};

struct input : public mouse_t {
	void initialise(const std::pair<LPCSTR, LPCSTR>& wnd_data);
	
	// Set function on hotkey press
	void on_hotkey(unsigned int vk, std::function<void()> fn);

	void process_message(UINT m, WPARAM w, LPARAM l);

	// Trace mouse messages
	void process_mouse_message(UINT m, WPARAM w, LPARAM l);

	// Trace keyboard messages
	void process_keybd_message(UINT m, WPARAM w);

	// Convert virtual key to string
	std::string virtual_key_to_wstring(unsigned int vk);

	// Get current key state
	_button_state get_key_state(unsigned int vk);

	// Get window
	HWND get_window();

	// Get original wndproc
	WNDPROC get_wndproc();

	void shutdown();

private:
	HWND m_HWND{};
	WNDPROC m_wndproc{};
	_button_state m_states[256]{};
	std::function<void()> m_hotkeys[256]{};
};

inline input g_input{};