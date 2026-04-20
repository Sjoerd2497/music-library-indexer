/**
 *  Music Library Indexer
 *  @author Sjoerd de Jonge
 *  2026-03-28
 *
 *  Method:
 *  1. Create empty JSON.
 *  2. Loop through all files in current directory.
 *      2a. For each .aiff file:
 *          Add new object to JSON.
 *          Read file, extract ID3 frames, append frames to JSON object.
 *  3. For each subdirectory in current directory:
 *      3a. Perform step 2.
 */


#include <iostream>
#include <filesystem>
#include <fstream>
#include "aiff_reader.h"
#include "id3_parser.h"
#include "util/json.hpp"

int main() {
    const std::string project_root = PROJECT_ROOT;
    // const std::string filename = "/Users/sjoerd/Music/Unherluferlick.aiff";
    const std::string filename = project_root + "/music/sample_break.aiff";

    // Create an if-stream to open the file.
    std::ifstream fin{ filename, std::ios_base::binary };

    // Skip ifstream to the start of the ID3 tag.
    locateId3(fin);

    const nlohmann::json song = id3ToJson(fin);

    std::cout << song.dump() << std::endl;
    return 0;
}
