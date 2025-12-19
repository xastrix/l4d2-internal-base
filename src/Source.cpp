#include "interfaces.h"
#include "hooks.h"
#include "input.h"
#include "util.h"
#include "font_loader.h"
#include "signatures.h"
#include "events.h"
#ifdef _DEBUG
#include "console.h"
#endif

#include <left4dead2_icons.h>

enum _loaded_fonts {
	LF_LEFT4DEAD2_ICONS,
};

static std::vector<font_resource_t> loaded_fonts = {
	{ LF_LEFT4DEAD2_ICONS, left4dead2_icons, LEFT4DEAD2_ICONS_SZ },
};

bool __stdcall DllMain(const HMODULE instance, const int32_t reason, void*)
{
	mod_t mod{ reason };

	mod.in(DLL_PROCESS_ATTACH, [&]()
	{
		DisableThreadLibraryCalls(instance);

		auto h = CreateThread(0, 0, [](void* I) -> unsigned long __stdcall
		{
			// set up starting time
			auto start_time = std::chrono::steady_clock::now();

			// waiting for initialization of the last module
			if (util::wait_for_mod(serverBrowserDLL) == WFM_TIMEOUT)
				goto _Exit;

#ifdef _DEBUG
			// init console for debugging
			console::initialise();
#endif

			// init font loader with pointer to font list
			g_font_loader.initialise(loaded_fonts);

			// init var manager
			g_vars.initialise();

			// init input manager (trace keys, mouse etc.)
			g_input.initialise({ LEFT4DEAD2_GAME_CLASS_NAME, 0 });

			// init signature list
			g_signatures.initialise();

			// init interfaces & some pointers
			g_l4d2.initialise();

			// init event manager
			g_event_manager.initialise();

			// init hooks (based on minhooks)
			g_hooks.initialise();

			// set initialised flag
			g::initialised = true;

			// set unload key
			g_input.on_hotkey(VK_F12, []() {
				g::done = true;
			});

			// set ui key
			g_input.on_hotkey(VK_INSERT, []() {
				g_ui.toggle();
			});

#ifdef _DEBUG
			// initialization time
			printf("game hacked in %dms\n", static_cast<int>(util::get_elapsed_time(start_time)));
#endif
			auto last_minute{ 0 };
			auto last_hour{ 0 };
			while (!g::done) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));

				int total_minutes = util::get_elapsed_time(start_time) / 60000;
				int total_hours = total_minutes / 60;

				if (total_hours != last_hour) {
					last_hour = total_hours;
					
					g::hours_in_game = 0;
					g::minutes_in_game = 0;
					g::seconds_in_game = 0;

					last_minute = 0;
				}

				if (total_minutes != last_minute) {
					last_minute = total_minutes;
					g::seconds_in_game = 0;
				}
				else {
					g::seconds_in_game = (util::get_elapsed_time(start_time) % 60000) / 1000;
				}

				g::minutes_in_game = total_minutes % 60;
				g::hours_in_game = total_hours;
			}

		_Exit:
			FreeLibraryAndExitThread(static_cast<HMODULE>(I), EXIT_SUCCESS);
		}, instance, 0, 0);

		if (h)
			CloseHandle(h);
	});

	mod.in(DLL_PROCESS_DETACH, []() {
		// checking if the user has set unload flag
		if (!g::done)
			return;

		// uninit ui
		g_ui.shutdown();

		// uninit hooks
		g_hooks.shutdown();

		// uinit input manager
		g_input.shutdown();

		// uninit event manager
		g_event_manager.shutdown();

		// uninit var manager
		g_vars.shutdown();

		// free loaded font list
		g_font_loader.shutdown(loaded_fonts);

#ifdef _DEBUG
		// uninit console
		console::shutdown();
#endif
	});

	return true;
}