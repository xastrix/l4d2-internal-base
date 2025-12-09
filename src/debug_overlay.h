#pragma once

#define DEBUG_OVERLAY_INTERFACE_VERSION "VDebugOverlay003"

#include "vec3.h"
#include "mem.h"

class c_debug_overlay {
public:
	/*
	* IDebugOverlay::ScreenPosition(vec3, vec3)
	* Get screen pos.
	*/
	bool screen_position(const vec3& in, vec3& out) {
		return mem::call_virtual_fn<bool, 12>(this, in, out);
	}
};