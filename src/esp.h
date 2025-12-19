#pragma once

#define NOMINMAX
#define MAX_CLIENTS 65
#define MAX_PLAYER_RENDERING_DISTANCE 1499.0f

#include "sdk.h"

struct esp {
	void render();

private:
	void calc_player_animation_progress(int index, float& anim, c_terror_player* entity);

	void update_position(int index, const vec3& pos);
	void reset_position(int index);

	void draw_player_box(int index, c_terror_player* entity);
	void draw_player_health(int index, c_terror_player* entity);

private:
	vec3  m_stored_pos[MAX_CLIENTS]{};
	float m_anim_progress[MAX_CLIENTS]{};
	bool  m_has_seen[MAX_CLIENTS]{};
	float m_alpha[MAX_CLIENTS]{};
};

inline esp g_esp{};