#pragma once

#include <string>
#include <vector>
#include <variant>
#include <optional>

using varType = std::variant<bool, int, float>;
using vars_t = std::vector<std::pair<std::string, varType>>;

struct vars {
	// Init var manager
	void initialise();

	// Add or update a key-value pair
	// If the key already exists, updates the value if different; otherwise, adds a new pair
	void set(const std::string& key, const varType& value);

	// Retrieve variable value by key, cast to type T
	// Returns std::optional<T> — value if type matches, nullopt if key not found or type mismatch
	template <typename T>
	std::optional<T> get_as(const std::string& key)
	{
		for (const auto& v : m_map)
		{
			if (v.first == key)
			{
				std::optional<varType> opt{ v.second };

				if (opt.has_value()) {
					if (const auto val = std::get_if<T>(&opt.value())) {
						return *val;
					}
				}
			}
		}

		return std::nullopt;
	}

	// Get the current collection of variables (key-value pairs)
	vars_t get_map();

	// Uninit var manager
	void shutdown();

private:
	vars_t m_map{};
};

inline vars g_vars{};