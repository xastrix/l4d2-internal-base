#include "netvars.h"

#include "client_class.h"
#include "sdk.h"
#include "fnv.h"

#include <unordered_map>

using netvar_key_value_map = std::unordered_map<uint32_t, uintptr_t>;
using netvar_table_map = std::unordered_map<uint32_t, netvar_key_value_map>;

static std::pair<std::string, std::string> read_netvar_ss(const std::string& netvar);
static void add_props_for_table(
	netvar_table_map& table_map,
	const uint32_t table_name_hash,
	const std::string& table_name,
	recv_table_t* table,
	const size_t child_offset = 0
);
static void initialize_props(netvar_table_map& table_map);

uintptr_t netvars::get_netvar_address(const std::string& netvar)
{
	static netvar_table_map map{};

	if (map.empty())
		initialize_props(map);

	auto table{ read_netvar_ss(netvar).first };
	auto prop{ read_netvar_ss(netvar).second };

	if (map.find(fnv::hash(table.c_str())) == map.end())
		return 0;

	auto& table_map{ map.at(fnv::hash(table.c_str())) };

	if (table_map.find(fnv::hash(prop.c_str())) == table_map.end())
		return 0;

	return table_map.at(fnv::hash(prop.c_str()));
}

static std::pair<std::string, std::string> read_netvar_ss(const std::string& netvar)
{
	std::pair<std::string, std::string> ret{};

	std::string cols{ "::" };
	size_t pos{ netvar.find(cols) };
	if (pos == std::string::npos)
		return ret;

	ret.first = netvar.substr(0, pos);
	ret.second = netvar.substr(pos + cols.size());

	return ret;
}

static void add_props_for_table(
	netvar_table_map& table_map,
	const uint32_t table_name_hash,
	const std::string& table_name,
	recv_table_t* table,
	const size_t child_offset
) {
	for (int i{}; i < table->m_props_num; i++) {
		auto& prop = table->m_props[i];

		auto name = std::string{ prop.m_var_name };

		if (prop.m_data_table && prop.m_elements > 0) {
			if (name.substr(0, 1) == "0")
				continue;

			add_props_for_table(table_map, table_name_hash, table_name,
				prop.m_data_table, prop.m_offset + child_offset);
		}

		if (name.substr(0, 1) != "m")
			continue;

		const auto name_hash = fnv::hash(prop.m_var_name);
		const auto offset = static_cast<uintptr_t>(prop.m_offset) + child_offset;

		table_map[table_name_hash][name_hash] = offset;
	}
}

static void initialize_props(netvar_table_map& table_map)
{
	for (auto c = g_l4d2.m_client->get_all_classes(); c; c = c->m_next)
	{
		const auto table = reinterpret_cast<recv_table_t*>(c->m_recv_table);
		const auto table_name = table->m_net_table_name;
		const auto table_name_hash = fnv::hash(table_name);

		if (table == nullptr)
			continue;

		add_props_for_table(table_map, table_name_hash, table_name, table);
	}
}