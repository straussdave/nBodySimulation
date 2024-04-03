#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class FileHelper
{
public:
	FileHelper(const std::string& fn);
	json get_planet_data();
	
	
private:
	std::string filename;
	json read_planet_data();
};