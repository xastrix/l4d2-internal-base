#include "esp.h"

#include "vars.h"
#include "math.h"
#include "renderer.h"

void esp::render()
{
	if (!g_l4d2.m_engine->is_connected())
		return;

	if (!g_l4d2.get_local())
		return;

	for (int i{}; i < MAX_CLIENTS; i++)
	{
		auto entity = g_l4d2.m_entity_list->get<c_terror_player*>(i);

		if (!entity || !entity->get_health() || i == g_l4d2.m_engine->get_local_player()) {
			reset_position(i);
			continue;
		}

		float& anim{ m_anim_progress[i] };
		calc_player_animation_progress(i, anim, entity);

		if (g_vars.get_as<bool>("visuals->players->box").value())
			draw_player_box(i, entity);

		if (g_vars.get_as<bool>("visuals->players->health").value())
			draw_player_health(i, entity);
	}
}

void esp::draw_player_box(int index, c_terror_player* entity)
{
	auto class_list = entity->get_client_class();

	if (!class_list)
		return;

	box bbox{};
	if (!math::get_player_bbox(entity, bbox))
		return;

	if (class_list->m_class_id == CTerrorPlayer ||
		class_list->m_class_id == SurvivorBot)
	{
		auto outline_col = color_t{ 3, 3, 3, static_cast<int>(255 * m_alpha[index]) };
		auto col = color_t{ "visuals->players->box->col", static_cast<int>(255 * m_alpha[index]) };

		switch (g_vars.get_as<int>("visuals->players->box->type").value()) {
		case 0: {
			g_renderer.draw_line(bbox.x - 1, bbox.y - 1, bbox.x + bbox.w + 1, bbox.y - 1, 1.0f, outline_col);
			g_renderer.draw_line(bbox.x - 1, bbox.y + bbox.h + 1, bbox.x + bbox.w + 1, bbox.y + bbox.h + 1, 1.0f, outline_col);
			g_renderer.draw_line(bbox.x - 1, bbox.y - 1, bbox.x - 1, bbox.y + bbox.h + 1, 1.0f, outline_col);
			g_renderer.draw_line(bbox.x + bbox.w + 1, bbox.y - 1, bbox.x + bbox.w + 1, bbox.y + bbox.h + 1, 1.0f, outline_col);

			g_renderer.draw_line(bbox.x, bbox.y, bbox.x + bbox.w, bbox.y, 1.0f, col);
			g_renderer.draw_line(bbox.x, bbox.y + bbox.h, bbox.x + bbox.w, bbox.y + bbox.h, 1.0f, col);
			g_renderer.draw_line(bbox.x, bbox.y, bbox.x, bbox.y + bbox.h, 1.0f, col);
			g_renderer.draw_line(bbox.x + bbox.w, bbox.y, bbox.x + bbox.w, bbox.y + bbox.h, 1.0f, col);
			break;
		}
		case 1: {
			g_renderer.draw_corner_box(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, 3, 5, outline_col);
			g_renderer.draw_corner_box(bbox.x, bbox.y, bbox.w, bbox.h, 3, 5, col);
			break;
		}
		}
	}
}

void esp::draw_player_health(int index, c_terror_player* entity)
{
	auto class_list = entity->get_client_class();

	if (!class_list)
		return;

	box bbox{};
	if (!math::get_player_bbox(entity, bbox))
		return;

	if (class_list->m_class_id == CTerrorPlayer ||
		class_list->m_class_id == SurvivorBot)
	{
		const auto hp = entity->get_health() > 100 ? 100 : entity->get_health();

		if (hp)
		{
			const auto pixel_value = hp * (bbox.h + 1) / 100;

			auto outline_col = color_t{ 3, 3, 3, static_cast<int>(255 * m_alpha[index]) };

			switch (g_vars.get_as<int>("visuals->players->health->type").value()) {
			case 0: {
				g_renderer.draw_filled_rect(bbox.x - 6, bbox.y - 1, 4, bbox.h + 3, outline_col);
				g_renderer.draw_filled_rect(bbox.x - 5, bbox.y + (bbox.h + 1) - pixel_value, 2, pixel_value,
					color_t("visuals->players->health->col", 255 * m_alpha[index]));
				break;
			}
			case 1: {
				g_renderer.draw_filled_rect(bbox.x - 6, bbox.y - 1, 4, bbox.h + 3, outline_col);
				g_renderer.draw_filled_rect(bbox.x - 5, bbox.y + (bbox.h + 1) - pixel_value, 2, pixel_value,
					color_t(99, std::min(255, hp * 225 / 100), 0, 255 * m_alpha[index]));
				break;
			}
			}

			for (int i{}; i < 9; i++)
				g_renderer.draw_line(bbox.x - 5, bbox.y + i * ((bbox.h + 1) / 9.0f) - 1.0f, bbox.x - 3,
					bbox.y + i * ((bbox.h + 1) / 9.0f) - 1.0f, 0.0f, outline_col);
		}
	}
}

void esp::calc_player_animation_progress(int index, float& anim, c_terror_player* entity)
{
	auto rate = g_l4d2.m_player_info_mgr->get_globals()->absolute_frame_time * 1.0f / 0.5f;

	if (entity->get_origin().distance_to(g_l4d2.get_local()->get_origin()) < MAX_PLAYER_RENDERING_DISTANCE) {
		update_position(index, entity->get_abs_origin());

		anim = anim > 0.0f ? std::clamp(anim + rate, 0.0f, 1.0f) : 0.5f;
		m_has_seen[index] = true;
	}
	else {
		if (anim < 0.30f)
			rate *= 0.02f;

		anim = std::clamp(anim -= rate, 0.0f, 1.0f);

		if (m_anim_progress[index] <= 0.0f) {
			m_has_seen[index] = false;
		}
	}

	m_alpha[index] = anim;
}

void esp::update_position(int index, const vec3& pos)
{
	m_stored_pos[index] = pos;

	if (m_anim_progress[index] > 0.0f && m_anim_progress[index] <= 0.30f && m_has_seen[index])
		m_anim_progress[index] = 0.30f;
}

void esp::reset_position(int index)
{
	m_has_seen[index] = false;
	m_anim_progress[index] = 0.0f;
}