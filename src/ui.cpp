#include "ui.h"

#include "input.h"
#include "sdk.h"

#include <sstream>

void ui::initialise(IDirect3DDevice9* device)
{
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(g_input.get_window());

	ImGui::StyleColorsDark();

	ImGui::GetIO().IniFilename = {};
	ImGui::GetIO().LogFilename = {};
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImGui::GetStyle().WindowMinSize = ImVec2(20, 20);

	ImGui_ImplDX9_Init(device);
}

void ui::invalidate_device_objects()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void ui::create_device_objects()
{
	ImGui_ImplDX9_CreateDeviceObjects();
}

void ui::render()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.09f, 0.09f, 0.09f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.18f, 0.18f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.20f, 0.20f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_TabHovered, ImVec4(0.18f, 0.18f, 0.18f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_TabActive, ImVec4(0.20f, 0.20f, 0.20f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(1.0f, 1.0f, 1.0f, 0.67f));
		ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.16f, 0.29f, 0.48f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.16f, 0.29f, 0.48f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.35f, 0.35f, 0.35f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.41f, 0.41f, 0.41f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.26f, 0.25f, 0.25f, 1.00f));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 4));
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 0.0f);

		if (m_opened)
			draw_main_window();

		draw_watermark();

		ImGui::PopStyleVar(7);
		ImGui::PopStyleColor(20);
	}
	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void ui::draw_main_window()
{
	static char cfg_name[8] = "legit";

	ImGui::SetNextWindowSize(ImVec2(400, 155));
	ImGui::Begin("##", &m_opened,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoTitleBar);
	{
		ImGui::SetTabIndex("visuals", &m_selected_tab, 0);
		ImGui::SetTabIndex("misc", &m_selected_tab, 1);

		ImGui::SetConfigPanel(cfg_name, sizeof(cfg_name));

		ImGui::TabContent(&m_selected_tab, 0, []()
		{
			if (ImGui::BeginTabItem("Players"))
			{
				ImGui::SetCursorPosX(4);

				ImGui::Checkbox("box", "visuals->players->box");
				ImGui::SameLine();

				ImGui::SetCursorPosX(255);

				ImGui::ColorEdit3("##box->col", "visuals->players->box->col");
				ImGui::SameLine();

				if (ImGui::Button(ImGui::IsPopupOpen("##players_box_pop") ? "-" : "+", "players_box_id"))
					ImGui::OpenPopup("##players_box_pop");

				if (ImGui::BeginPopup("##players_box_pop")) {
					ImGui::Combo("type", "visuals->players->box->type", "Default\0Corner");

					ImGui::EndPopup();
				}

				ImGui::SetCursorPosX(4);

				ImGui::Checkbox("health", "visuals->players->health");
				ImGui::SameLine();

				ImGui::SetCursorPosX(255);

				ImGui::ColorEdit3("##health->col", "visuals->players->health->col");
				ImGui::SameLine();

				if (ImGui::Button(ImGui::IsPopupOpen("##players_health_pop") ? "-" : "+", "players_health_id"))
					ImGui::OpenPopup("##players_health_pop");

				if (ImGui::BeginPopup("##players_health_pop")) {
					ImGui::Combo("type", "visuals->players->health->type", "Default\0Colored Health");

					ImGui::EndPopup();
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("World"))
			{
				ImGui::SetCursorPosX(4);

				ImGui::Checkbox("spawn objects", "visuals->spawn_objects");
				ImGui::SameLine();

				ImGui::SetCursorPosX(255);

				ImGui::ColorEdit3("##spawn_objects->col", "visuals->spawn_objects->col");
				ImGui::SameLine();

				if (ImGui::Button(ImGui::IsPopupOpen("##spawn_objects_pop") ? "-" : "+", "spawn_objects_id"))
					ImGui::OpenPopup("##spawn_objects_pop");

				if (ImGui::BeginPopup("##spawn_objects_pop")) {
					ImGui::Combo("type", "visuals->spawn_objects->type", "Text\0Icon");
					ImGui::SliderFloat("distance", "visuals->spawn_objects->distance", 300.0f, 5000.0f);

					ImGui::EndPopup();
				}

				ImGui::SetCursorPosX(4);

				ImGui::Checkbox("special infected", "visuals->special_infected");
				ImGui::SameLine();

				ImGui::SetCursorPosX(255);
				ImGui::ColorEdit3("##special_infected->col", "visuals->special_infected->col");

				ImGui::EndTabItem();
			}
		});

		ImGui::TabContent(&m_selected_tab, 1, []()
		{
			if (ImGui::BeginTabItem("General"))
			{
				ImGui::SetCursorPosX(4);
				ImGui::Checkbox("auto pistol", "misc->autopistol");

				ImGui::SetCursorPosX(4);
				ImGui::Checkbox("bhop", "misc->bhop");

				ImGui::SetCursorPosX(4);
				ImGui::Checkbox("autostrafe", "misc->autostrafe");

				ImGui::EndTabItem();
			}

			if (ImGui::TabItemButton("Unload"))
				g::done = true;
		});
	}
	ImGui::End();
}

void ui::draw_watermark()
{
	std::ostringstream ss{};
	ss << "l4d2-internal-base";

	static bool v[3] = { false, false, false };

	if (v[0])
		ss << " | " << util::get_current_time();

	if (v[1])
	{
		if (g::hours_in_game > 0)
		{
			if (g::hours_in_game >= 24)
				ss << " | " + std::to_string(g::hours_in_game) + "h Playing";

			else
				ss << " | " + std::to_string(g::hours_in_game) + "h " + std::to_string(g::minutes_in_game) + "m Playing";
		}
		else {
			ss << " | " + std::to_string(g::minutes_in_game) + "m " + std::to_string(g::seconds_in_game) + "s Playing";
		}
	}

	if (v[2])
		ss << " | " << std::to_string(static_cast<int>(1.f / g_l4d2.m_player_info_mgr->get_globals()->absolute_frame_time)) << " fps";

	auto width = ImGui::CalcTextSize(ss.str().c_str()).x + 30;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(m_opened ? width : width - 20, 22));

	ImGuiWindowFlags flags = (ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar |
		                      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
		                      ImGuiWindowFlags_NoMove);

	if (m_opened)
		flags &= ~ImGuiWindowFlags_NoMove;

	ImGui::Begin("##Interface", 0, flags);
	{
		ImGui::SetCursorPos(ImVec2(4, 4));
		ImGui::Text(ss.str().c_str());

		if (m_opened)
		{
			ImGui::SameLine();

			ImGui::SetCursorPos(ImVec2(width - 19, 0));
			if (ImGui::Button(ImGui::IsPopupOpen("##InterfaceUtils") ? "-" : "+", ImVec2(20, 22)))
				ImGui::OpenPopup("##InterfaceUtils");

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 3));

			if (ImGui::BeginPopup("##InterfaceUtils")) {
				ImGui::Checkbox("Current time", &v[0]);
				ImGui::Checkbox("Time playing", &v[1]);
				ImGui::Checkbox("Show fps", &v[2]);

				ImGui::EndPopup();
			}

			ImGui::PopStyleVar();
		}
	}
	ImGui::End();

	ImGui::PopStyleVar();
}

void ui::toggle()
{
	m_opened = !m_opened;
	g_l4d2.m_surface->set_cursor_always_visible(m_opened);

	if (!g_l4d2.m_engine->is_game_paused() && g_l4d2.m_engine->is_in_game())
	{
		if (m_opened) {
			m_old_mouse_pos_x = g_input.get_mouse_pos_x();
			m_old_mouse_pos_y = g_input.get_mouse_pos_y();
		}
		else {
			RECT r{};
			if (GetWindowRect(g_input.get_window(), &r)) {
				POINT p{ m_old_mouse_pos_x, m_old_mouse_pos_y };
				
				ClientToScreen(g_input.get_window(), &p);
				SetCursorPos(p.x, p.y);
			}
		}
	}
}

void ui::shutdown()
{
	if (m_opened) {
		g_l4d2.m_surface->set_cursor_always_visible(false);
		m_opened = false;
	}

	m_old_mouse_pos_x = 0;
	m_old_mouse_pos_y = 0;

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}