#include "chlamydia.h"
#include "json.hpp"
#if 0
//namespaces are evil but not really
using json = nlohmann::json;

void Config::GenerateDefault(std::string configFile)
{

}

void Config::Load(std::string configFile)
{
	std::ifstream cfg(configFile);
	std::stringstream buf;
	buf << cfg.rdbuf();

	buf >> config;
}
#endif