#pragma once

#define ENGINE_VGUI_VERSION "VEngineVGui001"

#include "mem.h"

class c_engine_vgui {
public:
	/*
	* IEngineVGui::GetPanelName(unsigned int)
	* Get panel name.
	*/
	const char* get_panel_name(std::uint32_t id) {
		return mem::call_virtual_fn<const char*, 36>(this, id);
	}
};