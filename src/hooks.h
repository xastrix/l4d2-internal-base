#pragma once

#include "common.h"
#include "globals.h"
#include "ui.h"
#include "sdk.h"

#include <mutex>
#include <MinHook.h>

enum _hk_list {
	HK_PRESENT,
	HK_RESET,
	HK_CREATEMOVE,
	HK_PAINTTRAVERSE,
	HK_GETSCREENASPECTRATIO,
	HK_ISGAMEPAUSED,
	maxHooks,
};

struct hook_t {
	template <typename T, int index>
	void hook(void* addr, void* target, void** orig) {
		if (m_hooked)
			return;

		m_index = index;
		m_addr = addr;
		m_src = mem::get_virtual_fn<T>(m_addr, m_index);
		m_target = target;
		m_orig = orig;

		MH_STATUS status = MH_CreateHook(m_src, m_target, m_orig);

		if (status != MH_OK) {
#ifdef _DEBUG
			printf("Failed to create %s hook\n", get_hook_name_by_index(m_index));
#endif
			return;
		}

		status = MH_EnableHook(m_src);

		if (status != MH_OK) {
#ifdef _DEBUG
			printf("Failed to enable %s hook\n", get_hook_name_by_index(m_index));
#endif
			return;
		}

#ifdef _DEBUG
		printf("[MinHook] %s hooked (%d index)\n", get_hook_name_by_index(m_index), m_index);
#endif

		m_hooked = true;
	}

	int get_index() {
		return m_index;
	}

	bool is_hooked() {
		return m_hooked;
	}
	
	void unhook() {
		if (!m_hooked)
			return;

		MH_STATUS status = MH_DisableHook(m_src);

		if (status != MH_OK) {
#ifdef _DEBUG
			printf("Failed to disable %s hook\n", get_hook_name_by_index(m_index));
#endif
			return;
		}

		status = MH_RemoveHook(m_src);

		if (status != MH_OK) {
#ifdef _DEBUG
			printf("Failed to remove %s hook\n", get_hook_name_by_index(m_index));
#endif
			return;
		}

		m_hooked = false;
	}

	const char* get_hook_name_by_index(int index) {
		switch (index) {
		case CREATE_MOVE_FN_INDEX:
			return "Create move";
		case PAINT_TRAVERSE_FN_INDEX:
			return "Paint traverse";
		case PRESENT_FN_INDEX:
			return "Present";
		case RESET_FN_INDEX:
			return "Reset";
		case GET_SCREEN_ASPECT_RATIO_FN_INDEX:
			return "Get screen aspect ratio";
		case IS_GAME_PAUSED_FN_INDEX:
			return "Is game paused";
		}
	}

private:
	int    m_index{};
	void*  m_addr{};
	void*  m_src{};
	void*  m_target{};
	void** m_orig{};
	bool   m_hooked{};
};

struct hooks {
	void initialise();
	void shutdown();

private:
	hook_t m_hooks[maxHooks]{};
};

inline hooks g_hooks{};