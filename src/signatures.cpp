#include "signatures.h"
#include "util.h"

void signatures::initialise()
{
	m_signatures[S_DEVICE_PTR] = scan_sig(g::module_list[shaderapidx9DLL], "A1 ? ? ? ? 8B 08 6A 03") + 1;
	m_signatures[S_WEAPON_DATA_PTR] = scan_sig(g::module_list[clientDLL], "0F B7 ? ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3");
	m_signatures[S_CLIENT_MODE_PTR] = scan_sig(g::module_list[clientDLL], "89 04 B5 ? ? ? ? E8") + 3;
}

sig_t signatures::get_sig(_signature_list index)
{
	return m_signatures[index];
}

sig_t signatures::scan_sig(const std::string& module_name, const std::string& signature)
{
	const auto handle = GetModuleHandleA(module_name.c_str());

	if (!handle)
		return {};

	const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(handle);
	const auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(handle) + dos_header->e_lfanew);

	const auto pattern_bytes = [signature]() -> std::vector<int> {
		std::vector<int> bytes{};

		auto start = const_cast<char*>(signature.c_str());
		auto end = const_cast<char*>(signature.c_str()) + signature.size();

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;

				if (*current == '?')
					++current;

				bytes.push_back(-1);
			}
			else {
				bytes.push_back(std::strtoul(current, &current, 16));
			}
		}
		return bytes;
	}();

	const auto scan_bytes = reinterpret_cast<uint8_t*>(handle);

	const auto sz = pattern_bytes.size();
	const auto bl = pattern_bytes.data();

	const auto sz_img = nt_headers->OptionalHeader.SizeOfImage;

	for (int i{}; i < sz_img - sz; i++) {
		auto found = true;

		for (auto j = 0; j < sz; ++j) {
			if (scan_bytes[i + j] != bl[j] && bl[j] != -1) {
				found = false;
				break;
			}
		}

		if (found)
			return &scan_bytes[i];
	}

	return {};
}