#pragma once

#include <stdint.h>
#include <string>

enum _signature_list {
	S_DEVICE_PTR,
	S_WEAPON_DATA_PTR,
	S_CLIENT_MODE_PTR,
	maxSignatures,
};

using sig_t = uint8_t*;

struct signatures {
	void initialise();

	// Get signature by index
	sig_t get_sig(_signature_list index);

private:
	// Scan signature by pattern
	sig_t scan_sig(const std::string& module_name, const std::string& signature);

private:
	sig_t m_signatures[maxSignatures];
};

inline signatures g_signatures{};