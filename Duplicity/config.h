#pragma once

class Config {
private:
	json config;

public:
	void Load(std::string configFile);
	void GenerateDefault(std::string configFile);
};