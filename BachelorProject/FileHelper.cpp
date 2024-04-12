#include "FileHelper.h"

/// <summary>
/// Constructor
/// </summary>
/// <param name="fn"></param>
FileHelper::FileHelper(const std::string& fn)
{
    filename = fn;
}

/// <summary>
/// gets planet data from file
/// </summary>
/// <returns>json object with planet data if successful, empty json if unsuccessful</returns>
json FileHelper::get_planet_data()
{
    return read_planet_data();
}

/// <summary>
/// Reads content of the planet data file and stores it in json object
/// </summary>
/// <returns>true if successful, false if unsuccessful</returns>
json FileHelper::read_planet_data()
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return json();
    }
    json jsonData;
    try {
        file >> jsonData;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: Failed to read JSON from file " << filename << ": " << e.what() << std::endl;
        file.close();
        return json();
    }

    file.close();
    return jsonData;
}