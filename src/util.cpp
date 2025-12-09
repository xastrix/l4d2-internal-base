#include "util.h"

#include <ctime>

_wait_for_module util::wait_for_mod(const _module_list module_index, int ms)
{
	const auto timeout_ms{ TIMEOUT_FOR_WAIT_MODULES };
	auto waited_ms{ 0 };

	while (!GetModuleHandleA(g::module_list[module_index].c_str()))
	{
		if (waited_ms >= timeout_ms)
			return WFM_TIMEOUT;

		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		waited_ms += ms;
	}

	return WFM_OK;
}

std::string util::xor_encrypt_decrypt(const std::string& data, const std::string& key)
{
	std::string result{ data };

	for (int i{}; i < data.size(); i++) {
		result[i] = data[i] ^ key[i % key.size()];
	}

	return result;
}

std::string util::remove_chars_from_string(std::string string, const std::string& chars)
{
	size_t pos{ string.find(chars) };

	if (pos != std::string::npos)
		string.erase(pos, chars.length());

	return string;
}

std::string util::get_current_time()
{
	std::time_t now = std::time(nullptr);
	std::tm*    local_time = std::localtime(&now);

	char buf[16];
	sprintf(buf, "%02d:%02d:%02d",
		local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

	return buf;
}

long long util::get_elapsed_time(const std::chrono::steady_clock::time_point start_time)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count();
}