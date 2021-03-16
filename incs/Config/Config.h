#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <map>
#include "VirtualHost.hpp"

void init_config(const std::string& raw, std::list<VirtualHost>& vhosts);

#endif
