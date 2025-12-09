#include "hooks.h"

#include "esp.h"
#include "world.h"
#include "math.h"
#include "signatures.h"

static long(D3DAPI *o_present)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
static std::once_flag init_present_stuff{};
static long D3DAPI present_h(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region)
{
	std::call_once(init_present_stuff, [device]()
	{
		g_fonts.initialise(device, {
			{ Tahoma12px,          12, "Tahoma",          FW_MEDIUM, ANTIALIASED_QUALITY },
			{ left4dead_icons21px, 21, "left4dead_icons", FW_NORMAL, CLEARTYPE_QUALITY },
		});

		g_renderer.initialise(device);
		g_ui.initialise(device);
	});

	if ((g::initialised && !g::done))
	{
		g_renderer.begin_modificated_states();
		{
			g_esp.render();
			g_visuals.render();
			g_ui.render();
		}
		g_renderer.end_modificated_states();
	}

	return o_present(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

static long(D3DAPI *o_reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
static long D3DAPI reset_h(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters)
{
	if ((g::initialised && !g::done))
	{
		g_fonts.shutdown();
		g_renderer.shutdown();

		g_ui.invalidate_device_objects();
	}

	auto ret = o_reset(device, present_parameters);

	if ((g::initialised && !g::done))
	{
		if (ret == D3D_OK)
		{
			g_fonts.initialise(device, {});
			g_renderer.create_objects(device);

			g_ui.create_device_objects();
		}
	}

	return ret;
}

static bool(__stdcall *o_create_move)(float, user_cmd_t*);
static bool __stdcall create_move_h(float input_sample_frametime, user_cmd_t* cmd)
{
	/* check if cmd is corrected */
	if (!cmd || !cmd->command_number)
		return o_create_move(input_sample_frametime, cmd);

	// init local player
	g_l4d2.init_local({ g_l4d2.m_entity_list->get<c_base_player*>(g_l4d2.m_engine->get_local_player()) });

	if ((g::initialised && !g::done))
	{
		if (g_l4d2.get_local() && g_l4d2.get_local()->get_health() > 0)
		{
			const auto old_angles = cmd->viewangles;
			const auto old_forward = cmd->forwardmove;
			const auto old_side = cmd->sidemove;

			auto do_autopistol = [cmd]() {
				if (!g_l4d2.m_engine->is_connected() ||
					!g_l4d2.m_engine->is_in_game())
					return;

				static bool shoot{};
				if (shoot && (cmd->buttons & IN_ATTACK))
					cmd->buttons &= ~IN_ATTACK;

				shoot = cmd->buttons & IN_ATTACK ? true : false;
			};

			auto do_bhop = [cmd]() {
				if (!g_l4d2.m_engine->is_connected() ||
					!g_l4d2.m_engine->is_in_game())
					return;

				if (g_l4d2.get_local()->get_move_type() == MOVETYPE_LADDER ||
					g_l4d2.get_local()->get_move_type() == MOVETYPE_NOCLIP ||
					g_l4d2.get_local()->get_move_type() == MOVETYPE_OBSERVER)
					return;

				if (cmd->buttons & IN_JUMP && !(g_l4d2.get_local()->get_flags() & FL_ON_GROUND))
					cmd->buttons &= ~IN_JUMP;
			};

			auto do_autostrafe = [cmd]() {
				if (!g_l4d2.m_engine->is_connected() ||
					!g_l4d2.m_engine->is_in_game())
					return;

				if (g_l4d2.get_local()->get_move_type() == MOVETYPE_LADDER ||
					g_l4d2.get_local()->get_move_type() == MOVETYPE_NOCLIP ||
					g_l4d2.get_local()->get_move_type() == MOVETYPE_OBSERVER)
					return;

				if (cmd->buttons & IN_FORWARD || cmd->buttons & IN_BACK ||
					cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT)
					return;

				const auto velocity = g_l4d2.get_local()->get_velocity().length_2d();

				if (velocity > 1.0f && !(g_l4d2.get_local()->get_flags() & FL_ON_GROUND))
				{
					if (!cmd->mouse_x) {
						cmd->forwardmove = std::min<float>(450.0f, 5850.0f / velocity);
						cmd->sidemove = (cmd->command_number % 2) == 0 ? -450.0f : 450.0f;
					}
					else {
						cmd->sidemove = cmd->mouse_x < 0.0f ? -450.0f : 450.0f;
					}
				}
			};

			if (g_vars.get_as<bool>("misc->autopistol").value())
				do_autopistol();

			if (g_vars.get_as<bool>("misc->bhop").value())
				do_bhop();

			if (g_vars.get_as<bool>("misc->autostrafe").value())
				do_autostrafe();

			/* fix movement, uncomment if need */
			// math::correct_movement(cmd, old_angles, old_forward, old_side);
		}

		return false;
	}
}

static void(__thiscall *o_paint_traverse)(c_vgui_panel*, std::uint32_t, bool, bool);
static void __stdcall paint_traverse_h(std::uint32_t panel, bool force_repaint, bool allow_force)
{
	o_paint_traverse(g_l4d2.m_panel, panel, force_repaint, allow_force);

	static std::uint32_t focus_overlay_panel_id{ 0 };

	if (!focus_overlay_panel_id)
	{
		switch (fnv::hash(g_l4d2.m_panel->get_panel_name(panel))) {
		case fnv::hash("FocusOverlayPanel"): {
			focus_overlay_panel_id = panel;
			break;
		}
		}
	}

	if (panel == focus_overlay_panel_id)
	{
		// g_l4d2.m_surface->draw_set_color(255, 255, 255);
		// g_l4d2.m_surface->draw_filled_rect(5, 5, 40, 40);
		// ...
	}
}

static float(__thiscall *o_get_screen_aspect_ratio)(c_engine_client*, int, int);
static float __stdcall get_screen_aspect_ratio_h(int width, int height)
{
	return o_get_screen_aspect_ratio(g_l4d2.m_engine, width, height);
}

static bool(__thiscall *o_is_game_paused)(c_engine_client*);
static bool __stdcall is_game_paused_h()
{
	return o_is_game_paused(g_l4d2.m_engine);
}

void hooks::initialise()
{
	MH_Initialize();

	m_hooks[HK_PRESENT].hook<IDirect3DDevice9, PRESENT_FN_INDEX>(g_l4d2.m_device,
		present_h, reinterpret_cast<void**>(&o_present));

	m_hooks[HK_RESET].hook<IDirect3DDevice9, RESET_FN_INDEX>(g_l4d2.m_device,
		reset_h, reinterpret_cast<void**>(&o_reset));

	m_hooks[HK_CREATEMOVE].hook<c_client_mode, CREATE_MOVE_FN_INDEX>(g_l4d2.m_client_mode,
		create_move_h, reinterpret_cast<void**>(&o_create_move));

	m_hooks[HK_PAINTTRAVERSE].hook<c_vgui_panel, PAINT_TRAVERSE_FN_INDEX>(g_l4d2.m_panel,
		paint_traverse_h, reinterpret_cast<void**>(&o_paint_traverse));

	m_hooks[HK_GETSCREENASPECTRATIO].hook<c_engine_client, GET_SCREEN_ASPECT_RATIO_FN_INDEX>(g_l4d2.m_engine,
		get_screen_aspect_ratio_h, reinterpret_cast<void**>(&o_get_screen_aspect_ratio));

	m_hooks[HK_ISGAMEPAUSED].hook<c_engine_client, IS_GAME_PAUSED_FN_INDEX>(g_l4d2.m_engine,
		is_game_paused_h, reinterpret_cast<void**>(&o_is_game_paused));
}

void hooks::shutdown()
{
	for (int i{}; i < maxHooks; i++)
		m_hooks[i].unhook();

	MH_Uninitialize();
}