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

int FileHelper::write_results_to_file(std::vector<float> results)
{
    std::ofstream outFile(filename);

    // Check if the file was opened successfully
    if (!outFile) {
        std::cerr << "Error: Unable to open the file." << std::endl;
        return 1;
    }

    // Write results to the text file
    for (size_t i = 0; i < results.size(); ++i) {
        outFile << "Loop " << i + 1 << ": " << results[i] << " seconds" << std::endl;
    }

    // Close the text file
    outFile.close();

    std::cout << "Results written to results.txt successfully." << std::endl;

    return 0;
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